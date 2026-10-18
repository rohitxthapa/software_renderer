#include "../include/window_interface.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>


void Window_interface::init(SDL_Window *window,SDL_Surface *surface,int w , int h){
    this->window = window;
    this->surface = surface;
    width = w;
    height = h;
}



void Window_interface::display(std::vector<uint32_t> framebuffer){
    if(surface->pitch == width*4){
        std::memcpy(surface->pixels,framebuffer.data(),width*height*4);
    }else{
        std::cerr<<"surface pitch is differnet from width bytes"<<std::endl;
        for(int i = 0 ; i < height ; i++ ){
            void* dst = (uint8_t*)surface->pixels + (surface->pitch*i);
            void* src = framebuffer.data() + i * width * 4;
            std::memcpy(dst,src,width*4);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void Window_interface::close(){
    SDL_DestroySurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
