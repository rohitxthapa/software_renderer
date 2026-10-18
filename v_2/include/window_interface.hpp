#ifndef _WINDOW_INTERFACE_
#define _WINDOW_INTERFACE_

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include "custom_math.hpp"

class Window_interface {
private:
    SDL_Window *window;
    SDL_Surface *surface;

    int width , height ;
    std::string title;

public:
    void init(SDL_Window *window,SDL_Surface *surface,int w,int h);
    void display(std::vector<uint32_t> framebuffer);
    void close();
};

#endif
