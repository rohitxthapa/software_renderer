#include "../include/interface.hpp"
#include <SDL3/SDL_events.h>
#include <cstdint>
#include <vector>
#include <cstdint>
#include <cstring>

void Interface::init(Input_state *i_state,Render_state *r_state,System_state *s_state){
    title = "renderer_fps_00000000";
    input_state = *i_state;
    render_state = *r_state;
    system_state = *s_state;

    if(!SDL_Init(SDL_INIT_VIDEO)) std::cerr<<"SDL couldn't initialize";

    window = SDL_CreateWindow(title.data(),render_state.width,render_state.height,0);
    if(window == nullptr) std::cerr<<"window == nullptr"<<std::endl;

    // surface = SDL_GetWindowSurface(window);
    // if(surface == nullptr) std::cerr<<"surface == nullptr"<<std::endl;

    user_interface.init(window,render_state.width,render_state.height,&input_state,&render_state,&system_state);
}

void Interface::get_states(Input_state *i_state,Render_state *r_state,System_state *s_state){
    *i_state = input_state;
    *r_state = render_state;
    *s_state = system_state;

    input_state = {0};
    render_state.changed = false;
}

void Interface::update(std::vector<uint32_t> framebuffer,double delta){
    this->framebuffer = framebuffer;
    this->delta = delta;
    fps = (1000/delta);

}

void Interface::run(){
    add_fps_to_title();
    user_interface.run(framebuffer);
}


void Interface::add_fps_to_title(){
    for(int i = 20 ; i > 12 ; i--){
        int temp = fps % 10;
        fps /= 10;
        title[i] = (char)temp + 48;
    }
    SDL_SetWindowTitle(window,title.data());
}

// void Interface::surface_display(std::vector<uint32_t> framebuffer){
    //     // this is so if i could create full fetched renderer , use that renderer for imgui too and use surface to display
//     if(surface->pitch == width*4){
//         std::memcpy(surface->pixels,framebuffer.data(),width*height*4);
//     }else{
//         std::cerr<<"surface pitch is differnet from width bytes"<<std::endl;
//         for(int i = 0 ; i < height ; i++ ){
//             void* dst = (uint8_t*)surface->pixels + (surface->pitch*i);
//             void* src = framebuffer.data() + i * width * 4;
//             std::memcpy(dst,src,width*4);
//         }
//     }
//     SDL_UpdateWindowSurface(window);
// }

void Interface::close(){
    user_interface.close();
    // SDL_DestroySurface(surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
