#ifndef _CUSTOM_TYPES_
#define _CUSTOM_TYPES_

#include <vector>

struct Vec_2{
        float x , y ;
};

struct Vec_3{
    float x , y , z;

    Vec_3 add (const Vec_3& other) {
    return {(x+other.x),(y+other.y),(z+other.z)};
    }

    Vec_3 sub (const Vec_3& other) {
        return {x-other.x,y-other.y,z-other.z};
    }

    Vec_3 cross (const Vec_3& other) {
        return {(y*other.z - z*other.y),(z*other.x - x*other.z),(x*other.y - y*other.x)};
    }

    float dot (const Vec_3& other) {
        return (x*other.x + y*other.y + z*other.z);
    }

    Vec_3 scalar_multi (float n) {
        return {(n*x),(n*y),(n*z)};
    }
};

struct Face_indices{
    int a , b , c;
};
struct Triangle_indices{
    Face_indices indices[3];
};

struct Model_data {
    // right now i am paliing to only store the following 3 elements but there ar eotehrs too liek faces lines points
    std::vector<Vec_3> vertices;
    std::vector<Vec_2> UV_coord;
    std::vector<Vec_3> normals;

    std::vector<std::vector<Face_indices>> faces;

    std::vector<Triangle_indices> triangle_indices;
};

struct matrix_4x4{
    float _11=1,_12=0,_13=0,_14=0,_21=0,_22=1,_23=0,_24=0,_31=0,_32=0,_33=1,_34=0,_41=0,_42=0,_43=0,_44=1;

    Vec_3 transformation(Vec_3 co_ord){
        float x , y , z;
        x = _11*x + _12*y + _13*z + _14;
        y = _21*x + _22*y + _23*z + _24;
        z = _31*x + _32*y + _33*z + _34;
        return (Vec_3){x,y,z};
    }
};
#endif
