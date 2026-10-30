#include "../include/camera.hpp"


void Camera::init(){
    position = {0,0,1};
    forward = {0,0,-1};
    up = {0,1,0};
    right = {1,0,0};
    speed_per_sec = 3;

    yaw = -PI/2.0f;
    pitch = 0.0f;

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
    if(input_state.mouse_lock) {
        yaw   += input_state.rel_mousex * (PI / 180.0f) * (render_state.fov / render_state.width);
        pitch -= input_state.rel_mousey * (PI / 180.0f) * (render_state.fov / render_state.height); // Subtracted to invert standard mouse direction

        const float HALF_PI = (PI / 2.0f) - 0.001f; // Slightly less than 90 degrees to avoid gimbal lock
        if (pitch > HALF_PI)  pitch = HALF_PI;
        if (pitch < -HALF_PI) pitch = -HALF_PI;

        forward.x = std::cos(yaw) * std::cos(pitch);
        forward.y = std::sin(pitch);
        forward.z = std::sin(yaw) * std::cos(pitch);
        forward = normalize(forward);

        Float3 global_up = {0.0f, 1.0f, 0.0f};
        right = normalize(cross(forward, global_up));

        up = cross(right, forward);
    }

}

void Camera::close(){}
