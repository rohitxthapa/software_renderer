#include "../include/camera.hpp"


void Camera::init(){
    position = {1,2,3};
    forward = {0,0,1};
    up = {0,1,0};
    right = {1,0,0};
    speed_per_sec = 3;
    matrix.m = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    fovy = 90;
    near_plane = 0.1f;
    far_plane = 100.0f;
}

Float3 Camera::get_fovy_near_far_plane(){
    return {fovy,near_plane,far_plane};
}

void Camera::close(){}
