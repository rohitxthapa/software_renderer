#include "../include/resource_manager.hpp"
#include "../include/parser.hpp"

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
    std::filesystem::path file_path = "../assets/cube.obj";
    parser.parse(file_path,this->models);
}
void Resource_manager::run(){}
void Resource_manager::close(){}
