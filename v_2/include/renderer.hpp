#ifndef _RENDERER_
#define _RENDERER_

#include <cstdint>
#include <vector>
#include <algorithm>
#include "camera.hpp"
#include "custom_math.hpp"
#include "resource_manager.hpp"
#include "state_type.hpp"

class Renderer {
    private:
        std::vector<uint32_t> framebuffer;
        float aspect_ratio;
        Float2 plane_dim_near , plane_dim_far;
        Float4x4 projection_matrix;
        Model_data mmodel;
        Render_state render_state;
        Camera *camera;
        Resource_manager *resource_manager;

        inline void draw_pixel(int x , int y,uint32_t color);
        void draw_line(int x0 ,int y0 ,int x1 ,int y1,uint32_t color);
        std::vector<Float4> to_view_space(Model_data &pmodel);
        void clipping(std::vector<Float4> &vertices,std::vector<Triangle> &triangles);
        void viewport(std::vector<Float4> &vertices);
        void wireframe();
        void rasterization();
        void update(Render_state &r_state);

    public:
        void init(Render_state &r_state,Camera *camera,Resource_manager *resource_manager);
        void clear(uint32_t color = 0xFFFFFFFF);
        void render(Render_state &render_state);

        std::vector<uint32_t> get_framebuffer();

        void close();

};


#endif
