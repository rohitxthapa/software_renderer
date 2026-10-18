#include "../include/helper.hpp"
#include <SDL3/SDL_timer.h>

void Helper::init(){
    delta = 0;
    start = SDL_GetPerformanceCounter();
    frame_time_100 = 10;
    frame_time_60 = 16.666666666666668;
    frame_time_30 = 33.333333333333336;
    wait_frame_time = frame_time_100;

}

void Helper::calculate_delta(){
    end = SDL_GetPerformanceCounter();
    delta = ((double)end - start)/SDL_GetPerformanceFrequency();
    start = end;
    fps = 1000/delta;
    frame_time = delta;
}

void Helper::wait(){
    if(delta < wait_frame_time){
    SDL_Delay(wait_frame_time - delta);
    delta = wait_frame_time;
    fps = 1000/delta;
    }
}

double Helper::get_delta(){
    return delta;
}
