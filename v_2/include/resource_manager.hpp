#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include "parser.hpp"
#include "custom_math.hpp"

#include <vector>

struct Face{
    std::vector<int> vertex_indices;
    std::vector<int> UV_indices;
    std::vector<int> normal_indices;

    void clear(){
        vertex_indices.clear();
        UV_indices.clear();
        normal_indices.clear();
    }
};

struct Triangle{
    Int3 vertex_indices;
    Int3 UV_indices;
    Int3 normal_indices;
};

struct Mesh_data{
        // this will store the mesh data , at first i was thinking of one strct for one data type but this is the way ,
        // later i will make another struct for animation or skeletion , if i do so
    std::vector<Float3> vertices;
    std::vector<Float2> UV_coord;
    std::vector<Float3> normals;

    std::vector<Face> faces;
    std::vector<Triangle> triangles;

    void clear(){
        vertices.clear();
        UV_coord.clear();
        normals.clear();
        for(Face face : faces) face.clear();
        faces.clear();
        triangles.clear();
    }
};

struct anime_skeleton{
    // here we will store the animation data , or the rigging data

   void clear(){}
};


struct Model_data {
    Mesh_data static_model; // vector because we have load multiple mdoels
    anime_skeleton dynamic_model;

    void clear(){
        // if htere are other model then their clear should also be here
        static_model.clear();
        dynamic_model.clear();
    }

};

class Resource_manager{
  public :
    std::vector<Model_data> models;

    void init(){}


    void close(){}
};

#endif
