#ifndef _USER_INTERFACE_
#define _USER_INTERFACE_

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "resource_manager.hpp"
#include "state_type.hpp"
#include "../dependency/portable_file_dialogs/portable-file-dialogs.h"

class User_interface {
    private:
        SDL_Window *window ;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Event events;
        Resource_manager *resource_manager;
        std::unique_ptr<pfd::open_file> file_selection = nullptr;

    public:
        Input_state *input_state;
        Render_state *render_state;
        System_state *system_state;

        void init(SDL_Window *window,Resource_manager *resource_manager,Input_state *i_state,Render_state *r_state,System_state *s_state);
        void clear_input();
        void handle_input();
        void run(const std::vector<uint32_t> &framebuffer);
        void close();
};

#endif
