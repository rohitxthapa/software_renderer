#ifndef _INTERFACE_
#define _INTERFACE_

#include <SDL3/SDL.h>
#include "camera.hpp"
#include "custom_math.hpp"
#include "user_interface.hpp"
#include "window_interface.hpp"

#include <cstdint>
#include <iostream>

class Interface{
    private :
        uint8_t key_state;
        Float2 mouse_pos_rel;
        Window_interface window_interface;
        User_interface user_interface;
        std::vector<uint32_t> framebuffer;
        double delta;
        int fps;
        Camera camera;
        Input_state input_state;
        SDL_Event events;
        SDL_Window *window;
        SDL_Surface *surface;
        int width , height;
        std::string title;

    public :
        bool running;
        void init();

        Int2 get_window_size();

        void update_camera();

        void update(std::vector<uint32_t> framebuffer,double delta,Camera camera);

        void run();

        void add_fps_to_title();

        void close();

};


#endif
