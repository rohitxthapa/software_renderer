#include "../include/resource_manager.hpp"
#include "../include/parser.hpp"
#include "../include/custom_math.hpp"

#include <unordered_map>
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

void Resource_manager::init(){}
void Resource_manager::run(){}
void Resource_manager::close(){}
