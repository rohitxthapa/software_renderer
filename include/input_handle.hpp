#ifndef _INPUT_HANDLE_
#define _INPUT_HANDLE_

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <cmath>

#include "view_system.hpp"
#include "custom_types.hpp"

void handle_input(Camera& camera,float delta,float mouse_lock){

    float x , y ;
    float delta_in_sec = delta/1000;
    if(mouse_lock){
        // SDL_MouseButtonFlags mouseState = SDL_GetRelativeMouseState(&x, &y);
        SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&x,&y);
        if(!(x == camera.old_mouse_pos.x && y == camera.old_mouse_pos.y)){}
            camera.change = camera.change | 0x00000002;
            float dx , dy ;
            dx = x - camera.old_mouse_pos.x;
            dy = y - camera.old_mouse_pos.y;
            double alpha = dx * (90.0/960);
            double beta = dy * (90.0/540);
            alpha = alpha * (3.141592653/180);
            beta = beta * (3.141592653/180);

            camera.direction = (camera.direction.scalar_multi(std::cos(alpha))).add(camera.right.scalar_multi(std::sin(alpha)));
            camera.direction = (camera.direction.scalar_multi(std::cos(beta))).add(camera.up.scalar_multi(std::sin(beta)));

        camera.old_mouse_pos.x = x ;
        camera.old_mouse_pos.y = y ;
    }

    camera.right = camera.direction.cross({0,1,0});
    if(camera.right.x == 0 && camera.right.y == 0 && camera.right.z == 0){
        camera.right = camera.direction.cross({0,0,1});
    }
   camera.up = camera.direction.cross(camera.right);


    const bool* keys = SDL_GetKeyboardState(NULL);
    if(keys[SDL_SCANCODE_W]){
        // camera.position = camera.position.add((camera.direction.scalar_multi(delta_in_sec)).scalar_multi(camera.speed_per_sec));
        camera.position = camera.position.add(camera.direction.scalar_multi(delta_in_sec * camera.speed_per_sec));
        camera.change = camera.change | 0x00000001;
    }
    if(keys[SDL_SCANCODE_S]){
        camera.position = camera.position.sub(camera.direction.scalar_multi(delta_in_sec * camera.speed_per_sec));
        camera.change = camera.change | 0x00000001;
    }
    if(keys[SDL_SCANCODE_A]){
        camera.position = camera.position.sub(camera.right.scalar_multi(delta_in_sec * camera.speed_per_sec));
        camera.change = camera.change | 0x00000001;
    }
    if(keys[SDL_SCANCODE_D]){
        camera.position = camera.position.add(camera.right.scalar_multi(delta_in_sec * camera.speed_per_sec));
        camera.change = camera.change | 0x00000001;
    }
    if(keys[SDL_SCANCODE_SPACE]){
        camera.position = camera.position.add(camera.up.scalar_multi(delta_in_sec * camera.speed_per_sec));
        camera.change = camera.change | 0x00000001;
    }
    if(keys[SDL_SCANCODE_LSHIFT]){
        camera.position = camera.position.sub(camera.up.scalar_multi(delta_in_sec * camera.speed_per_sec));
        camera.change = camera.change | 0x00000001;
    }
}



#endif
