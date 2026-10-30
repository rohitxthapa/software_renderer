#ifndef _STATE_TYPE_
#define _STATE_TYPE_

struct System_state{
    bool running;
};

struct Render_state {
    int render; // when it has more features it or other flags may be used to
    // define rasterization , ray casting , ray tracing , wireframe , solid view etc
    bool rasterization;
    bool wireframe;
    float fov;
    float near_plane;
    float far_plane;
    int width;
    int height;
};

struct Input_state {
    bool key_w;
    bool key_a;
    bool key_s;
    bool key_d;
    bool key_space;
    bool key_shift;

    bool mouse_lock;
    bool mouse_left_key;
    bool mouse_right_key;
    float mousex , mousey;
    float rel_mousex , rel_mousey;
};



#endif
