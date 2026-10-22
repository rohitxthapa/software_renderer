#ifndef _INTERFACE_
#define _INTERFACE_

#include <SDL3/SDL.h>
#include "custom_math.hpp"
#include "user_interface.hpp"
#include "state_type.hpp"

#include <cstdint>
#include <iostream>

class Interface{
    private :
        uint8_t key_state;
        Float2 mouse_pos_rel;
        User_interface user_interface;
        std::vector<uint32_t> framebuffer;
        double delta;
        int fps;
        Input_state input_state;
        Render_state render_state;
        System_state system_state;
        SDL_Window *window;
        // SDL_Surface *surface;
        std::string title;

    public :
        void init(Input_state *i_state,Render_state *r_state,System_state *s_state);

        void get_states(Input_state *i_state,Render_state *r_state,System_state *s_state);

        void update(std::vector<uint32_t> framebuffer,double delta);

        void run();

        void add_fps_to_title();

        // void surface_display(std::vector<uint32_t> framebuffer);

        void close();

};


#endif
