#include "../include/renderer.hpp"
#include <cstdint>
#include <iostream>

void Renderer::init(Render_state &r_state){
    render_state = r_state;
    framebuffer.resize(render_state.width * render_state.height);

    aspect_ratio = (float)render_state.width/render_state.height;

    float fov_y_rad = render_state.fov * (PI / 180.0f);

    plane_dim_near.y = 2.0f * render_state.near_plane * tanf(fov_y_rad/2.0f);
    plane_dim_near.x = plane_dim_near.y * aspect_ratio;

    plane_dim_far.y = 2.0f * render_state.far_plane * tanf(fov_y_rad/2.0f);
    plane_dim_far.x = plane_dim_far.y * aspect_ratio;

    projection_matrix.m[0] = (2*render_state.near_plane)/plane_dim_near.x;
    projection_matrix.m[5] = (2*render_state.near_plane)/plane_dim_near.y;
    projection_matrix.m[10] = -(render_state.far_plane)/(render_state.far_plane - render_state.near_plane);
    projection_matrix.m[11] = -(render_state.far_plane*render_state.near_plane)/(render_state.far_plane - render_state.near_plane);
    projection_matrix.m[14] = - 1.0f;
}

void Renderer::clear(uint32_t color){
    std::fill(framebuffer.begin(),framebuffer.end(),color);
}

void inline Renderer::draw_pixel(int x , int y, uint32_t color = 0x00FF00FF){
    // we are checking be cause our device coordinates are in float and when we convert them to int there is some problem
    if((y < render_state.height && x < render_state.width) && (y >= 0 && x >= 0)) framebuffer[y * render_state.width + x] = color;
}

void Renderer::draw_line(int x0, int y0, int x1, int y1, uint32_t color = 0xFF0000FF){
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        draw_pixel(x0,y0,color);
        if(x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if ( e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

std::vector<Float4> Renderer::to_view_space(Model_data &pmodel, Camera &camera){
    std::vector<Float4> vertices;

    Float4x4 vmatrix = get_view_matrix(camera.position,camera.forward);
    vmatrix = mul(projection_matrix,vmatrix);

    for(Float3 vertex:pmodel.model_mesh.vertices){
        vertices.push_back(mul(vmatrix,{vertex,1.0f}));
    }
    return vertices;
}

void Renderer::clipping(std::vector<Float4> &vertices,std::vector<Triangle> &triangles){
    for(Triangle T : triangles){
        bool noclip = false;
        int ind[3];
        ind[0] = T.vertex_indices.x;
        ind[1] = T.vertex_indices.y;
        ind[2] = T.vertex_indices.z;
        for( int i = 0 ; i < 3 ; i++ ){
        if(vertices[ind[i]].x < -1.0 || 1.0 < vertices[ind[i]].x) noclip = true;
        if(vertices[ind[i]].y < -1.0 || 1.0 < vertices[ind[i]].y) noclip = true;
        if(vertices[ind[i]].z < 0.0 || 1.0 < vertices[ind[i]].z) noclip = true;
        }
        if(noclip){
            mmodel.model_mesh.triangles.push_back(T);
        }
    }
}

void Renderer::viewport(std::vector<Float4> &vertices){
    for(Float4 V : vertices){
        float x = (V.x + 1)/2;
        float y = (1 - V.y)/2;
        float z = V.z;

        mmodel.model_mesh.vertices.push_back({x * render_state.width , y * render_state.height , z});
    }
}

void Renderer::wireframe(){
    for(Triangle &T : mmodel.model_mesh.triangles){
        auto& A = mmodel.model_mesh.vertices[T.vertex_indices.x];
        auto& B = mmodel.model_mesh.vertices[T.vertex_indices.y];
        auto& C = mmodel.model_mesh.vertices[T.vertex_indices.z];

        draw_line(std::round(A.x),std::round(A.y),std::round(B.x),std::round(B.y));
        draw_line(std::round(B.x),std::round(B.y),std::round(C.x),std::round(C.y));
        draw_line(std::round(C.x),std::round(C.y),std::round(A.x),std::round(A.y));

    }
}

void Renderer::rasterization(){

    for(Triangle &T:mmodel.model_mesh.triangles){
        Float3 V0, V1, V2;
        V0 = mmodel.model_mesh.vertices[T.vertex_indices.x];
        V1 = mmodel.model_mesh.vertices[T.vertex_indices.y];
        V2 = mmodel.model_mesh.vertices[T.vertex_indices.z];

        int min_x , max_x , min_y , max_y ;
        min_x = std::min({std::floor(V0.x),std::floor(V1.x),std::floor(V2.x)});
        max_x = std::max({std::ceil(V0.x),std::ceil(V1.x),std::ceil(V2.x)});
        min_y = std::min({std::floor(V0.y),std::floor(V1.y),std::floor(V2.y)});
        max_y = std::max({std::ceil(V0.y),std::ceil(V1.y),std::ceil(V2.y)});

        min_x = std::max(min_x, 0);
        max_x = std::min(max_x, render_state.width);
        min_y = std::max(min_y, 0);
        max_y = std::min(max_y, render_state.height);


        float dx01 = (V1.x - V0.x);
        float dy01 = (V1.y - V0.y);
        float dx02 = (V2.x - V1.x);
        float dy02 = (V2.y - V1.y);
        float dx03 = (V0.x - V2.x);
        float dy03 = (V0.y - V2.y);

        for(int jy = min_y ; jy < max_y ; jy++){
            for(int ix = min_x ; ix < max_x ; ix++ ){
                float px = ix+0.5 , py = jy+0.5;

                float E01 = (px - V0.x) * dy01 - (py - V0.y) * dx01 ;
                float E12 = (px - V1.x) * dy02 - (py - V1.y) * dx02 ;
                float E20 = (px - V2.x) * dy03 - (py - V2.y) * dx03 ;

                if((E01 >=0 && E12 >=0&& E20 >= 0) || (E01 <= 0 && E12 <= 0 && E20 <= 0)){
                    draw_pixel(ix,jy);
                }
            }
        }
    }
}
void Renderer::update(Render_state &r_state){
    if(r_state.changed) init(r_state);
    // for now lets juts put r_state.changed as flag
    // but we count have conpared now and then values but i think if we use
    // changed flag less data read maybe
}

void Renderer::render(Camera &camera,Resource_manager &res_manager,Render_state &r_state){
    // here lets take only first model , i still have to put the postion datamember in mesh_data class and also i should
    // put dta members that stores the minimum and maximum x and y or maybe the positon could do the part for culling
    // before transformation
    update(r_state);
    if(r_state.rasterization || r_state.wireframe || r_state.changed){
        Model_data pmodel = res_manager.models["Cube"];
        mmodel.model_mesh.clear();

        mmodel.model_mesh.vertices.reserve(pmodel.model_mesh.vertices.size());
        mmodel.model_mesh.triangles.reserve(pmodel.model_mesh.triangles.size());

        std::vector<Float4> vertices = to_view_space(pmodel , camera);

        clipping(vertices,pmodel.model_mesh.triangles);

        viewport(vertices);

        if(r_state.rasterization)rasterization();
        if(r_state.wireframe)wireframe();
    }

}

std::vector<uint32_t> Renderer::get_framebuffer(){
    return framebuffer;
}

void Renderer::close(){}
