#ifndef _USER_INTERFACE_
#define _USER_INTERFACE_

#include <SDL3/SDL.h>
#include <string>

struct Input_state {
    bool key_w;
    bool key_a;
    bool key_s;
    bool key_d;
    bool key_space;
    bool key_shift;
    bool key_tab;

    bool mouse_left_key;
    bool mouse_right_key;
    float mousex , mousey;
    float rel_mousex , rel_mousey;

};

class User_interface {
    private:
        SDL_Window* window ;
        SDL_Renderer* renderer;
        int win_width;
        int win_height;
        Input_state state {0};

    public:
        void init();
        Input_state get_input_state();
        void handle_input();
        void run();
        void close();
};

#endif
