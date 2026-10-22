#include "../include/camera.hpp"


void Camera::init(){
    position = {1,1,50};
    forward = {0,0,-1};
    up = {0,1,0};
    right = {1,0,0};
    speed_per_sec = 3;
    matrix.m = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
}

void Camera::update(Input_state &input_state,Render_state &render_state,float delta){
    float delta_in_sec = delta/1000;
    if(input_state.key_w){
        position = position + forward * (delta_in_sec * speed_per_sec);
    }
    if(input_state.key_s){
        position = position - forward * (delta_in_sec * speed_per_sec);
    }
    if(input_state.key_a){
        position = position - right * (delta_in_sec * speed_per_sec);
    }
    if(input_state.key_d){
        position = position + right * (delta_in_sec * speed_per_sec);
    }
    if(input_state.key_space){
        position = position + up * (delta_in_sec * speed_per_sec);
    }
    if(input_state.key_shift){
        position = position - up * (delta_in_sec * speed_per_sec);
    }
    if(input_state.mouse_lock){
        float alpha = input_state.rel_mousex * (PI/180) * (render_state.fov/render_state.width);
        float beta = input_state.rel_mousey * (PI/180) * (render_state.fov/render_state.height);
        forward = forward * std::cos(alpha) + right * std::sin(alpha);
        forward = forward * std::cos(beta) + up * std::sin(beta);
    }
}

void Camera::close(){}
