#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include "parser.hpp"
#include "custom_math.hpp"

#include <unordered_map>
#include <vector>

struct Face{
    // struct for face , face can be Xgon , so i am using vector;
    std::vector<int> vertex_indices;
    std::vector<int> UV_indices;
    std::vector<int> normal_indices;
};

struct Triangle{
    // truct to store indices for triangles
    Int3 vertex_indices;
    Int3 UV_indices;
    Int3 normal_indices;
};

struct Mesh_data{
        // this will store the mesh data
    std::vector<Float3> vertices;
    std::vector<Float2> UV_coord;
    std::vector<Float3> normals;

    std::vector<Face> faces;
    std::vector<Triangle> triangles;
    // i am storing both faces and triangles , but i should only store one , triangles for games , faces for 3d viewing , modeling

    void clear();
};

struct animation_and_skeleton{
    // here we will store the animation data , or the rigging data

   void clear();
};


struct Model_data {
    Mesh_data model_mesh; // vector because we have load multiple mdoels
    bool have_animation_or_skeleton ;
    animation_and_skeleton model_ani ;

    void clear();

};

class Resource_manager{
  public :
    std::unordered_map<std::string,Model_data> models;

    void init();
    void run();
    void close();
};

#endif
