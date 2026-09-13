#ifndef _WINDOW_INTERFACE_
#define _WINDOW_INTERFACE_

#include <SDL3/SDL.h>
#include <string>
#include <iostream>

class Window_interface {
private:
    SDL_Window *window;
    SDL_Surface *surface;

    int width , height ;
    std::string title;

public:
    void init();
    void add_fps_to_title(int fps);
    void close();
};

#endif
