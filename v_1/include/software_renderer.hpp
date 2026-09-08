#ifndef _SOFTWARE_RENDERER_
#define _SOFTWARE_RENDERER_

#include "custom_types.hpp"
#include "view_system.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <cmath>

struct Software_renderer{
    public :
        int width = 640 , height=480;
        std::vector<uint32_t> framebuffer;
        SDL_Surface* surface;
        Vec_2 plane;
        float FOVy = 80;
        float aspect_ratio;
        Vec_2 near_plane_dimention,far_plane_dimention;
        Mat_4 projection_matrix;
        Model_data f_model ;


        void init(int width , int height,SDL_Window* window){
            this->width = width;
            this->height = height;
            framebuffer.resize(width * height);

            surface = SDL_GetWindowSurface(window);
            if(!surface){
                std::cerr<<"sdl_getwindowsurface failed : " << SDL_GetError()<<std::endl;
            }
            FOVy = 60;
            aspect_ratio = (float)width/height;
            plane.x = 0.1f;
            plane.y = 100.0f;

            float fov_y_rad = FOVy * (3.14159265f / 180.0f);

            near_plane_dimention.y = 2.0f * plane.x * tanf(fov_y_rad/2.0f);
            near_plane_dimention.x = near_plane_dimention.y * aspect_ratio;

            far_plane_dimention.y = 2.0f * plane.y * tanf(fov_y_rad/2.0f);
            far_plane_dimention.x = far_plane_dimention.y * aspect_ratio;

            projection_matrix.m[0][0]= (2*plane.x)/near_plane_dimention.x;
            projection_matrix.m[1][1] = (2*plane.x)/near_plane_dimention.y;
            projection_matrix.m[2][2] = -(plane.y )/(plane.y - plane.x);
            projection_matrix.m[2][3] = -(plane.y*plane.x) / (plane.y - plane.x);
            projection_matrix.m[3][2] = -1.0f;

        }

        void clear(uint32_t color = 0xFFffffff){
            std::fill(framebuffer.begin(),framebuffer.end(),color);
        }
        void draw_pixel(int y, int x){
        if((x < 0 || x >= width)||(y < 0 || y >= height)) return;
            framebuffer[y * width + x] = 0xFF00FFFF;
        }

        void draw_line(int x0 , int y0 , int x1 , int y1){
            int dx = std::abs(x1 - x0);
            int dy = std::abs(y1 - y0);

            int sx = (x0 < x1) ? 1 : -1;
            int sy = (y0 < y1) ? 1 : -1;

            int err = dx - dy;

            while (true) {
                draw_pixel(x0, y0);
                if (x0 == x1 && y0 == y1) break;

                int e2 = 2 * err;

                if (e2 > -dy) {
                    err -= dy;
                    x0 += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y0 += sy;
                }
            }
        }

        void render(Model_data& model,Camera& camera){
            f_model.vertices.clear();
            f_model.triangle_indices.clear();
            //for this we assume the object lies in center of world space liek in model space
            // so we inplement mode to world later if we were to update the renderer
            std::vector<Vec_3> vertices = to_view_space(model , camera);

            clipping(vertices,model.triangle_indices);
            f_model.faces = model.faces;

            viewport(vertices);

            // rasterization();
            wireframe();
        }

        std::vector<Vec_3> to_view_space(Model_data& model,Camera& camera){
            std::vector<Vec_3> vertices;
            camera.t_matrix.m[0][0] = camera.t_matrix.m[1][1] = camera.t_matrix.m[2][2] = camera.t_matrix.m[3][3] = 1.0f ;
            camera.t_matrix.m[0][3] = -camera.position.x;
            camera.t_matrix.m[1][3] = -camera.position.y;
            camera.t_matrix.m[2][3] = -camera.position.z;
            Vec_3 temp1 = camera.right , temp2 = camera.up , temp3 = camera.direction;
            Mat_4 matrix = {temp1.x,temp1.y,temp1.z,0,temp2.x,temp2.y,temp2.z,0,temp3.x,temp3.y,temp3.z,0,0,0,0,1};

            matrix = matrix.mat_multi(camera.t_matrix);
            matrix = projection_matrix.mat_multi(matrix);

            for(Vec_3 vertex:model.vertices){
                // Vec_3 temp = matrix.transformation(vertex);
                // if(temp.z >= 0){
                vertices.push_back(matrix.transformation(vertex));
                // std::cout<<vertex.x<<std::endl;
                // }
            }
            return vertices;
        }

        void clipping(std::vector<Vec_3> model,std::vector<Triangle_indices> tri_ind){
            std::vector<Vec_3> vertices;
            for(Triangle_indices T : tri_ind){
                bool clip = false;
                for(Face_indices i : T.indices){
                    int ind = i.a;
                    if(model.at(ind).x < -1.0 || 1.0 < model.at(ind).x) clip = true;
                    if(model.at(ind).y < -1.0 || 1.0 < model.at(ind).y) clip = true;
                    if(model.at(ind).z < 0.0 || 1.0 < model.at(ind).z) clip = true;
                }
                if(clip){
                    f_model.triangle_indices.push_back(T);
                }
            }
        }

        void viewport(std::vector<Vec_3> vertices){
            for(Vec_3 V:vertices){
                float x = (V.x + 1)/2 ;
                float y = (1 - V.y)/2;
                float z = V.z;

                // std::cout<<x<<" "<<y<<" "<<z<<std::endl;

                // std::cout<<x<<" "<<y<<std::endl;
                f_model.vertices.push_back({x * width,y * height, z});
            }
        }

        void wireframe(){
            for(Triangle_indices T:f_model.triangle_indices){

                auto& A = f_model.vertices.at(T.indices[0].a);
                auto& B = f_model.vertices.at(T.indices[1].a);
                auto& C = f_model.vertices.at(T.indices[2].a);

                draw_line(A.x,A.y,B.x,B.y);
                draw_line(B.x,B.y,C.x,C.y);
                draw_line(C.x,C.y,A.x,A.y);

            }

        }

        void rasterization(){

            for(Triangle_indices T:f_model.triangle_indices){
                Vec_3 V0, V1, V2;
                V0 = f_model.vertices.at(T.indices[0].a);
                V1 = f_model.vertices.at(T.indices[1].a);
                V2 = f_model.vertices.at(T.indices[2].a);

                int min_x , max_x , min_y , max_y ;
                min_x = std::min({std::floor(V0.x),std::floor(V1.x),std::floor(V2.x)});
                max_x = std::max({std::ceil(V0.x),std::ceil(V1.x),std::ceil(V2.x)});
                min_y = std::min({std::floor(V0.y),std::floor(V1.y),std::floor(V2.y)});
                max_y = std::max({std::ceil(V0.y),std::ceil(V1.y),std::ceil(V2.y)});

                min_x = std::max(min_x, 0);
                max_x = std::min(max_x, width);
                min_y = std::max(min_y, 0);
                max_y = std::min(max_y, height);

                // std::cout<<min_x<<" "<<max_x<<" "<<min_y<<" "<<max_y<<std::endl;
                for(int jy = min_y ; jy < max_y ; jy++){
                    for(int ix = min_x ; ix < max_x ; ix++ ){
                        float px = ix+0.5 , py = jy+0.5;

                        float E01 = (px - V0.x)*(V1.y - V0.y) - (py - V0.y)*(V1.x - V0.x);
                        float E12 = (px - V1.x)*(V2.y - V1.y) - (py - V1.y)*(V2.x - V1.x);
                        float E20 = (px - V2.x)*(V0.y - V2.y) - (py - V2.y)*(V0.x - V2.x);

                        // std::cout<<E01<<" "<<E12<<" "<<E20<<std::endl;
                        if((E01 >=0 && E12 >=0&& E20 >= 0) || (E01 <= 0 && E12 <= 0 && E20 <= 0)){
                            // std::cout<<"pixel"<<std::endl;
                            draw_pixel(jy,ix);
                        }
                    }
                }
            }
        }

        // we might add one more function for raycasting



};
#endif
