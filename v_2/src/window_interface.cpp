#include "../include/window_interface.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>


void Window_interface::init(){
    width = 1280;
    height = 720;
    title = "renderer_fps_00000000";

    if(!SDL_Init(SDL_INIT_VIDEO)){
        // we are putting th sdl init here , but since we will be using sdl for input too
        // so if we are going to do multi_threading the thread that initializes should
        // access the input handling too , i think it should be that way
        // or we should  use the main thread to initialize the apis or
        // library or drivers
        std::cerr<<"SDL couldn't initialize";
    }

    window = SDL_CreateWindow(title.data(),width,height,0);
    if(window == nullptr) std::cerr<<"window couldn't be created"<<std::endl;

    surface = SDL_GetWindowSurface(window);
}

void Window_interface::add_fps_to_title(int fps){
    for(int i = 20 ; i > 12 ; i--){
        int temp = fps % 10;
        fps /= 10;
        title[i] = (char)temp + 48;
    }
}

void Window_interface::close(){
    SDL_DestroySurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
