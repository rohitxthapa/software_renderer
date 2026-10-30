#include "../include/resource_manager.hpp"
#include "../include/parser.hpp"

#include <iostream>
#include <vector>

void Mesh_data::clear(){
    vertices.clear();
    UV_coord.clear();
    normals.clear();
    faces.clear();
    triangles.clear();
}

void animation_and_skeleton::clear(){}

void Model_data::clear(){
// if htere are other model then their clear should also be here
    model_mesh.clear();
    model_ani.clear();
}

void Resource_manager::init(){
}

void Resource_manager::queue_model_load(std::filesystem::path &path){
    load_queue.push(path);
}
void Resource_manager::run(){
    if(!load_queue.empty()){
        std::filesystem::path next_file = load_queue.front();
        load_queue.pop();

        std::cout<<"Loading asset : "<<next_file<<std::endl;
        parser.parse(next_file,models);
    }
}
void Resource_manager::close(){
    for(auto& model : models){
        model.second.clear();
    }
    models.clear();
}
