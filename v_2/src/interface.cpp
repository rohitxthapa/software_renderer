#include "../include/interface.hpp"
#include <cstdint>
#include <vector>


void Interface::init(){
    window_interface.init();
    user_interface.init();
}

Int2 Interface::get_window_size(){
    return window_interface.get_window_size();
}

void Interface::update(std::vector<uint32_t> framebuffer,double delta,Camera camera){
    this->framebuffer = framebuffer;
    this->camera = camera;
    this->delta = delta;
    delta = 16.6666667;
    fps = (1000/delta);

}

void Interface::update_camera(){
    if(camera.lock){
        // here will come the part that revolve the world around camera
    }
    if(input_state.key_w){}
}

void Interface::run(){
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
