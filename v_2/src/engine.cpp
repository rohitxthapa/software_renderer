#include "../include/engine.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_timer.h>

void Engine::init(){
    interface.init();
    resource_manager.init();
    camera.init();
    renderer.init(interface.get_window_size(),camera.get_fovy_near_far_plane());

    delta = 16.66666667;
}

void Engine::run(){

bool running = true;
Uint64 start_time = SDL_GetPerformanceCounter();
SDL_Event events;

helper.init();
while(running) {
    while(SDL_PollEvent(&events)){
        // maybe i should put this part and input handle in another file and
        // keep a state machine
        switch(events.type){
            case SDL_EVENT_QUIT :
                running = false;
                break;
        }
    }

    renderer.clear();
    renderer.render(camera,resource_manager);

    helper.calculate_delta();
    delta = helper.get_delta();

    interface.update(renderer.get_framebuffer(),delta,camera);
    interface.run();

}

}

void Engine::close(){
    renderer.close();
    camera.close();
    resource_manager.close();
    interface.close();
}
