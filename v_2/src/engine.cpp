#include "../include/engine.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_timer.h>

void Engine::init(){
    window_interface.init();
    resource.init();
    renderer.init();
    camera.init();
    user_interface.init();

    delta = 16.66666667;
}

void Engine::run(){

bool running = true;
Uint64 start_time = SDL_GetPerformanceCounter();
SDL_Event events;

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
    }

    renderer.clear();
    renderer.render();


}


}

void Engine::close(){
    camera.close();
    renderer.close();
    resource.close();
    user_interface.close();
    window_interface.close();
}
