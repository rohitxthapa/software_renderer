#include "../include/interface.hpp"
#include <SDL3/SDL_events.h>
#include <cstdint>
#include <vector>


void Interface::init(){
    width = 1280;
    height = 720;
    title = "renderer_fps_00000000";

    if(!SDL_Init(SDL_INIT_VIDEO)) std::cerr<<"SDL couldn't initialize";

    window = SDL_CreateWindow(title.data(),width,height,0);
    if(window == nullptr) std::cerr<<"window == nullptr"<<std::endl;

    surface = SDL_GetWindowSurface(window);
    if(surface == nullptr) std::cerr<<"surface == nullptr"<<std::endl;

    window_interface.init(window,surface,width,height);
    user_interface.init(window,width,height);
    running = true;
}

Int2 Interface::get_window_size(){
    return {width,height};
}

void Interface::update(std::vector<uint32_t> framebuffer,double delta,Camera camera){
    this->framebuffer = framebuffer;
    this->camera = camera;
    this->delta = delta;
    fps = (1000/delta);

}

void Interface::update_camera(){
    if(camera.lock){
        // here will come the part that revolve the world around camera
    }
    // if(input_state.key_w){}
}

void Interface::run(){
    while(SDL_PollEvent(&events)){
        if(events.type == SDL_EVENT_QUIT) running = false ;
        switch(events.type){
            case SDL_EVENT_QUIT :
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED :
                running = false;
                break;
            case SDL_EVENT_KEY_DOWN :
                std::cout<<"key pressed"<<std::endl;
                break;
        }
    }
    add_fps_to_title();
    window_interface.display(framebuffer);
    user_interface.run(framebuffer);
    input_state = user_interface.get_input_state();

    update_camera();
}

void Interface::add_fps_to_title(){
    for(int i = 20 ; i > 12 ; i--){
        int temp = fps % 10;
        fps /= 10;
        title[i] = (char)temp + 48;
    }
    SDL_SetWindowTitle(window,title.data());
}

void Interface::close(){
    window_interface.close();
    // user_interface.close();
}
