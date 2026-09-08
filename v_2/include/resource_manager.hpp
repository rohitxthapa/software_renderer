#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include "parser.hpp"
#include "custom_math.hpp"

#include <vector>

struct Triangles_indices{
    Int3 indices[3];
};

struct Obj{
    std::vector<Float3> vertices;
    std::vector<Float3> UV_coord;
    std::vector<Float3> normals;

    std::vector<std::vector<Int3>> faces;
    std::vector<Triangles_indices> triangles;

    void clear(){
        vertices.clear();
        UV_coord.clear();
        normals.clear();
        for(std::vector<Int3> face : faces) faces.clear();
        faces.clear();
        triangles.clear();
    }

};

struct Model_data {
    // later we may build to store other type then just .obj too
    std::vector<Obj*> obj_models; // vector because we have load multiple mdoels


};

class Resource_manager{
  public :
    Model_data models;

};

#endif
