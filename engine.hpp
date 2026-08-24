#ifndef _ENGINE_
#define _ENGINE_

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <string>
#include <cstring>

#include "include/software_renderer.hpp"
#include "include/obj_parser.hpp"
#include "include/custom_types.hpp"
#include "include/resource_manager.hpp"
#include "include/view_system.hpp"
#include "include/input_handle.hpp"

class Engine{
    private:
        int window_width = 1280, window_height = 720 ;
        Software_renderer renderer;
        Obj_parser parser;
        Resource_manager res_manager;
        Camera camera;
        SDL_Window* window;
        SDL_Surface* surface;
        std::string title = "renderer_fps_00000000";
        double delta = 16.66667;
        bool mouse_lock = false;

    public:
        void parse(std::filesystem::path& file_path){
            try{res_manager.model = parser.parse(file_path);}
            catch(std::exception& e){
                std::cerr<<"error : "<<e.what()<<std::endl;
            }
        }

        void init(){
            if(!SDL_Init(SDL_INIT_VIDEO)){
                std::cerr<<"SDL couldn't initialize";
            }
            window = SDL_CreateWindow(title.data(),window_width,window_height,0);
            if(window == nullptr){
                std::cerr<<"window couldn't be created";
            }
            surface = SDL_GetWindowSurface(window);
            if(!surface){
                std::cerr<<"SDL_GetWindowSurface failed: "<<SDL_GetError()<<std::endl;
            }
            renderer.init(window_width,window_height,window);

        }

        void run(){
            bool running = true;
            SDL_Event events;
            Uint64 start_time = SDL_GetPerformanceCounter();

            while(running) {

                while(SDL_PollEvent(&events)){
                    switch(events.type){
                        case SDL_EVENT_QUIT :
                            running = false;
                            break;
                        case SDL_EVENT_MOUSE_BUTTON_DOWN :
                            if(!mouse_lock){
                            SDL_SetWindowRelativeMouseMode(window,true);
                            mouse_lock = true;
                            }
                            break;
                        case SDL_EVENT_KEY_DOWN :
                            if(events.key.key == SDLK_ESCAPE){
                            if(mouse_lock){
                                SDL_SetWindowRelativeMouseMode(window,false);
                                mouse_lock = false;
                            }
                            }
                            break;
                        case SDL_EVENT_WINDOW_FOCUS_LOST:
                            if (mouse_lock) {
                                SDL_SetWindowRelativeMouseMode(window, false);
                                mouse_lock = false;
                            }
                            break;
                        }
                    }

                handle_input(camera,delta,mouse_lock);

                renderer.clear();
                renderer.render(res_manager.get_model_refrence(),camera);

                if(surface->pitch == window_width*4){
                    std::memcpy(surface->pixels,renderer.framebuffer.data(),window_width*window_height*4);
                }else{
                    std::cerr<<"surface pitch different from width bytes";
                    for(int i = 0 ; i < window_height ; i++ ){
                        void* dst = (Uint8*)surface->pixels + (surface->pitch*i);
                        void* src = renderer.framebuffer.data() + i * window_width * 4;
                        std::memcpy(dst,src,window_width*4);
                    }
                }

                SDL_UpdateWindowSurface(window);

                Uint64 current_time = SDL_GetPerformanceCounter();
                delta = ((double)(current_time - start_time))/SDL_GetPerformanceFrequency();
                if(delta<16.66667){
                    SDL_Delay(16.66667-delta);
                    delta = 16.66667;
                }
                start_time = current_time;
                add_fps_to_title();
            }
        }

        void add_fps_to_title(){
            int fps = 1000/delta;
            int x = 10;
            for(int i = 20 ; i > 12 ; i-- ) {
                int temp = fps % 10;
                fps /= 10 ;
                title[i] = (char)temp + 48;
            }
            SDL_SetWindowTitle(window,title.data());
        }

        void close(){

        }


};

#endif
