#ifndef _SOFTWARE_RENDERER_
#define _SOFTWARE_RENDERER_

#include "custom_types.hpp"
#include "view_system.hpp"

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
            projection_matrix.m[2][2] = -(plane.y + plane.x)/(plane.y - plane.x);
            projection_matrix.m[2][3] = -(2*plane.y*plane.x) / (plane.x * plane.y);
            projection_matrix.m[3][2] = -1.0f;

        }

        void clear(uint32_t color = 0xFFffffff){
            std::fill(framebuffer.begin(),framebuffer.end(),color);
        }

        void render(Model_data& model,Camera& camera){
            //for this we assume the object lies in center of world space liek in model space
            // so we inplement mode to world later if we were to update the renderer
            std::vector<Vec_3> view_vertices = to_view_space(model , camera);
            for(Vec_3 v:view_vertices){
                std::cerr<<v.x<<" "<<v.y<<" "<<v.z;
            }

        }

        std::vector<Vec_3> to_view_space(Model_data& model,Camera& camera){
            std::vector<Vec_3> view_vertices;

            camera.t_matrix.m[0][3] = -camera.position.x;
            camera.t_matrix.m[1][3] = -camera.position.y;
            camera.t_matrix.m[2][3] = -camera.position.z;
            Vec_3 temp1 = camera.right , temp2 = camera.up , temp3 = camera.direction;
            Mat_4 matrix = {temp1.x,temp1.y,temp1.z,0,temp2.x,temp2.y,temp2.z,0,temp3.x,temp3.y,temp3.z,0,0,0,0,1};

            matrix = matrix.mat_multi(camera.t_matrix);
            projection_matrix = projection_matrix.mat_multi(matrix);

            for(Vec_3 vertex:model.vertices){
                view_vertices.push_back(camera.t_matrix.transformation(vertex));
            }
            return view_vertices;
        }

        //culling will happen later more like implemented later
        void rasterization(){

        }
        // we might add one more function for raycasting



};
#endif
