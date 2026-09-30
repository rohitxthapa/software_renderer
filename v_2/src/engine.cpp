#include "../include/engine.hpp"
#include <iostream>

void Engine::init(){
    interface.init();
    resource_manager.init();
    camera.init();
    renderer.init(interface.get_window_size(),camera.get_fovy_near_far_plane());

    delta = 16.66666667;
}

void Engine::run(){

helper.init();
while(interface.running) {
    renderer.clear();
    renderer.render(camera,resource_manager);

    interface.update(renderer.get_framebuffer(),delta,camera);
    interface.run();


    helper.calculate_delta();
    delta = helper.get_delta();
    helper.wait();
}

}

void Engine::close(){
    renderer.close();
    camera.close();
    resource_manager.close();
    interface.close();
}
