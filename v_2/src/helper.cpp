#include "../include/helper.hpp"
#include <SDL3/SDL_timer.h>

void Helper::init(){
    delta = 0;
    start = SDL_GetPerformanceCounter();
}

void Helper::calculate_delta(){
    end = SDL_GetPerformanceCounter();
    delta = ((double)end - start)/SDL_GetPerformanceFrequency();
    start = end;
}

double Helper::get_delta(){
    return delta;
}
