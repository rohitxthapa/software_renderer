#include <SDL3/SDL_init.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include <cstring>


int main(){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        std::cerr<<"SDL couldn't initialize"<<std::endl;
        return 1;
    }

    int width = 1920 , height = 1080;
    SDL_Window* window = SDL_CreateWindow("rasterization",width,height,0);
    if(window == nullptr){
        std::cerr<<"window count be created"<<std::endl;
        return 1;
    }
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    if(surface == nullptr){
        std::cerr<<"surface couldnt be create"<<std::endl;
        return 1;
    }

    std::vector<Uint32> framebuffer(height * width,0xFFFFFFFF);

    // since we are taking 1000 and 800 dimention let take three traingles
    // T1 500 , 200  T2 800 , 600 T3 300 , 500
    int mx = 500 , my = 200 , nx = 800 , ny = 600 , ox = 300 , oy = 500 ;
    for(int i = 0 ; i < height ; i++){
        for(int j = 0 ; j < width ; j++){
            float x = j + 0.5 , y = i + 0.5 ;

            float a = (x - mx)*(ny - my) - (y - my)*(nx - mx);
            float b = (x - nx)*(oy - ny) - (y - ny)*(ox - nx);
            float c = (x - ox)*(my - oy) - (y - oy)*(mx - ox);

            if((a >= 0 && b >= 0 && c >= 0) || ( a <= 0 && b <= 0 && c <= 0)){
                framebuffer.at(i*width + j) = 0xFF0000FF;
            }

        }
    }

    if(surface->pitch == width*4){
        std::memcpy(surface->pixels,framebuffer.data(),width*height*4);
    }else{
        std::cerr<<"surface pitch and width bytes diff"<<std::endl;
        for(int i = 0 ; i < height ; i++){
            void* dst = (Uint8*)surface->pixels + (surface->pitch*i);
            void* src = framebuffer.data() + i * width * 4;
        }
    }

    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);

    SDL_Quit();
    return 0;
}
