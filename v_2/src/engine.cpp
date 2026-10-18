#include "../include/engine.hpp"
#include <iostream>

void Engine::init(){
    interface.init();
    resource_manager.init();
    camera.init();
    renderer.init(interface.get_window_size(),camera.get_fovy_near_far_plane());

    delta = 10;
}

void Engine::run(){

helper.init();
while(interface.running) {
    renderer.clear();
    renderer.render(camera,resource_manager);

    interface.update(renderer.get_framebuffer(),delta,camera);
    interface.run();


    helper.calculate_delta();
    helper.wait();
    delta = helper.get_delta();
    helper.frame_time *= 10000000000.0;
    helper.frame_time /= 10000000000.0;
    std::cerr << "Frame Time : "<< helper.frame_time <<" Fps :"<< helper.fps << std::endl;
}

}

void Engine::close(){
    renderer.close();
    camera.close();
    resource_manager.close();
    interface.close();
}
