#ifndef _ENGINE_
#define _ENGINE_

#include <iostream>
#include <SDL3/SDL.h>
#include <string>

#include "include/software_renderer.hpp"
#include "include/obj_parser.hpp"
#include "include/custom_types.hpp"
#include "include/resource_manager.hpp"

class Engine{
    private:
        Software_renderer renderer;
        Obj_parser parser;
        Resource_manager res_manager;
        SDL_Window* window;
        int window_width = 1280, window_height = 720 ;
        std::string title = "software_renderer_fps_000000.000";
        float delta;

    public:
        void parse(std::filesystem::path& file_path){
            try{res_manager.model = parser.parse(file_path);}
            catch(std::exception& e){
                std::cerr<<"error : "<<e.what()<<std::endl;
            }
        }

        int init(){
            if(!SDL_Init(SDL_INIT_VIDEO)){
                std::cerr<<"SDL couldn't initialize";
            }
            window = SDL_CreateWindow(title.data(),window_width,window_height,0);
            if(window == nullptr){
                std::cerr<<"window couldn't be created";
            }


            return 0;
        }


};

#endif
