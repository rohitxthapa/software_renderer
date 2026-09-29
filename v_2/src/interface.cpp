#include "../include/interface.hpp"
#include <cstdint>
#include <vector>


void Interface::init(){
    window_interface.init();
    user_interface.init();
    running = true;
}

Int2 Interface::get_window_size(){
    return window_interface.get_window_size();
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
    if(input_state.key_w){}
}

void Interface::loop(){
    while(SDL_PollEvent(&events)){
        switch(events.type){
            case SDL_EVENT_QUIT :
                running = false;
                break;
        }
    }

}

void Interface::run(){
    loop();
    user_interface.run();
    input_state = user_interface.get_input_state();
    window_interface.add_fps_to_title(fps);
    window_interface.display(framebuffer);

    update_camera();
}

void Interface::close(){
    window_interface.close();
    user_interface.close();
}
