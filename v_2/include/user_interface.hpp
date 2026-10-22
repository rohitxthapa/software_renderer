#ifndef _USER_INTERFACE_
#define _USER_INTERFACE_

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <cstdint>
#include <string>
#include <vector>
#include "state_type.hpp"

class User_interface {
    private:
        SDL_Window *window ;
        int width;
        int height;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Event events;

    public:
        Input_state *input_state;
        Render_state *render_state;
        System_state *system_state;

        void init(SDL_Window *window,int w , int h,Input_state *i_state,Render_state *r_state,System_state *s_state);
        void clear_input();
        void handle_input();
        void run(const std::vector<uint32_t> &framebuffer);
        void close();
};

#endif
