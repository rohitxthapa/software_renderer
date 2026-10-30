#include "../include/engine.hpp"
#include <cstdlib>
#include <iostream>

Input_state default_input_state = {0,0,0,0,0,0,0,0,0,0,0,0,0};
Render_state default_render_state = {0,1,0,90,0.1,100,1280,720};
System_state default_system_state = {1};

void Engine::init(){
    input_state = default_input_state;
    render_state = default_render_state;
    system_state = default_system_state;

    resource_manager.init();
    camera.init();

    interface.init(&default_input_state,&default_render_state,&default_system_state,&resource_manager);
    renderer.init(render_state,&camera,&resource_manager);

    delta = 10;
}

void Engine::run(){
helper.init();
while(system_state.running) {
    renderer.clear();
    renderer.render(render_state);

    interface.update(renderer.get_framebuffer(),delta);
    resource_manager.run();
    interface.run();
    interface.get_states(&input_state,&render_state,&system_state);

    camera.update(input_state,render_state,delta);

    // std::cout<<camera.position.x<<" "<<camera.position.y<<" "<<camera.position.z<<std::endl;
    helper.calculate_delta();
    helper.wait();
    delta = helper.get_delta();
    // std::cerr << "Frame Time : "<< helper.frame_time <<" Fps :"<< helper.fps << std::endl;
}

}

void Engine::close(){
    renderer.close();
    camera.close();
    resource_manager.close();
    interface.close();
}
