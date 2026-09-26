#ifndef _RENDERER_
#define _RENDERER_

#include <cstdint>
#include <vector>
#include <algorithm>
#include "camera.hpp"
#include "custom_math.hpp"
#include "resource_manager.hpp"

class Renderer {
    private:
        int width , height;
        std::vector<uint32_t> framebuffer;
        float fovy;
        float aspect_ratio;
        float near_plane , far_plane;
        Float2 plane_dim_near , plane_dim_far;
        Float4x4 projection_matrix;
        Model_data mmodel;

        inline void draw_pixel(int x , int y,uint32_t color);
        void draw_line(int x0 ,int y0 ,int x1 ,int y1,uint32_t color);
        std::vector<Float4> to_view_space(Model_data &pmodel,Camera &camera);
        void clipping(std::vector<Float4> vertices,std::vector<Triangle> triangles);
        void viewport(std::vector<Float4> vertices);
        void wireframe();
        void rasterization();

    public:
        void init(Int2 win_size,Float3 fovy_near_far_plane);
        void clear(uint32_t color = 0xFFFFFFFF);
        void render(Camera &camera,Resource_manager &res_manager);

        std::vector<uint32_t> get_framebuffer();

        void close();

};


#endif
