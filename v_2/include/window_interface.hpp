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
    void init();
    Int2 get_window_size();
    void display(std::vector<uint32_t> framebuffer);
    void add_fps_to_title(int fps);
    void close();
};

#endif
