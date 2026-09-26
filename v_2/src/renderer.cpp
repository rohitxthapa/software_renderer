#include "../include/renderer.hpp"
#include <complex>
#include <cstdint>

void Renderer::init(Int2 win_size,Float3 fovy_near_far_plane){
    this->width = win_size.x;
    this->height = win_size.y;
    framebuffer.resize(width * height);

    fovy = fovy_near_far_plane.x ;
    aspect_ratio = (float)width/height;

    near_plane = fovy_near_far_plane.y;
    far_plane = fovy_near_far_plane.z;

    float fov_y_rad = fovy * (PI / 180.0f);

    plane_dim_near.y = 2.0f * near_plane * tanf(fov_y_rad/2.0f);
    plane_dim_near.x = plane_dim_near.y * aspect_ratio;

    plane_dim_far.y = 2.0f * far_plane * tanf(fov_y_rad/2.0f);
    plane_dim_far.x = plane_dim_far.y * aspect_ratio;

    projection_matrix.m[0] = (2*near_plane)/plane_dim_near.x;
    projection_matrix.m[5] = (2*near_plane)/plane_dim_near.y;
    projection_matrix.m[10] = -(far_plane)/(far_plane - near_plane);
    projection_matrix.m[11] = -(far_plane*near_plane)/(far_plane - near_plane);
    projection_matrix.m[14] = - 1.0f;
}

void Renderer::clear(uint32_t color){
    std::fill(framebuffer.begin(),framebuffer.end(),color);
}

void inline Renderer::draw_pixel(int x , int y, uint32_t color = 0xFF000000){
    framebuffer[y * width + x] = color;
}

void Renderer::draw_line(int x0, int y0, int x1, int y1, uint32_t color = 0xFFFF0000){
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
        vertices.push_back(mul(vmatrix,{vertex,0}));
    }
    return vertices;
}

void Renderer::clipping(std::vector<Float4> vertices,std::vector<Triangle> triangles){
    for(Triangle T : triangles){
        bool noclip = false;
        int ind[3];
        ind[0] = T.vertex_indices.x;
        ind[1] = T.vertex_indices.y;
        ind[2] = T.vertex_indices.z;
        for( int i = 0 ; i < 3 ; i++ ){
        if(vertices.at(ind[i]).x < -1.0 || 1.0 < vertices.at(ind[i]).x) noclip = true;
        if(vertices.at(ind[i]).y < -1.0 || 1.0 < vertices.at(ind[i]).y) noclip = true;
        if(vertices.at(ind[i]).z < 0.0 || 1.0 < vertices.at(ind[i]).z) noclip = true;
        }
        if(noclip){
            mmodel.model_mesh.triangles.push_back(T);
        }
    }
}

void Renderer::viewport(std::vector<Float4> vertices){
    for(Float4 V : vertices){
        float x = (V.x + 1)/2;
        float y = (1 - V.y)/2;
        float z = V.z;

        mmodel.model_mesh.vertices.push_back({x * width , y * height , z});
    }
}

void Renderer::wireframe(){
    for(Triangle T : mmodel.model_mesh.triangles){
        auto& A = mmodel.model_mesh.vertices.at(T.vertex_indices.x);
        auto& B = mmodel.model_mesh.vertices.at(T.vertex_indices.y);
        auto& C = mmodel.model_mesh.vertices.at(T.vertex_indices.z);

        draw_line(A.x,A.y,B.x,B.y);
        draw_line(B.x,B.y,C.x,C.y);
        draw_line(C.x,C.y,A.x,A.y);

    }
}

void Renderer::rasterization(){

    for(Triangle T:mmodel.model_mesh.triangles){
        Float3 V0, V1, V2;
        V0 = mmodel.model_mesh.vertices.at(T.vertex_indices.x);
        V1 = mmodel.model_mesh.vertices.at(T.vertex_indices.y);
        V2 = mmodel.model_mesh.vertices.at(T.vertex_indices.z);

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
                    draw_pixel(ix,jy);
                }
            }
        }
    }
}

void Renderer::render(Camera &camera,Resource_manager &res_manager){
    // here lets take only first model , i still have to put the postion datamember in mesh_data class and also i should
    // put dta members that stores the minimum and maximum x and y or maybe the positon could do the part for culling
    // before transformation
    Model_data pmodel = res_manager.models.at(0);
    mmodel.model_mesh.clear();

    std::vector<Float4> vertices = to_view_space(pmodel , camera);

    clipping(vertices,pmodel.model_mesh.triangles);

    viewport(vertices);

    rasterization();
    wireframe();


}

std::vector<uint32_t> Renderer::get_framebuffer(){
    return framebuffer;
}
