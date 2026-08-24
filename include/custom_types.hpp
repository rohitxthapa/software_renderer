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
struct Mat_4 {
    float m[4][4] = {0};

    Mat_4 mat_multi(Mat_4 other){
        Mat_4 result;
        for(int i = 0 ; i < 4 ; i++ ){
            for(int j = 0 ; j < 4 ; j++){
                float sum = 0.0f;
                for(int k = 0 ; k < 4 ; k++){
                    sum += m[i][k]*other.m[k][j];
                }
                result.m[i][j] = sum ;
            }
        }
        return result;
    }

    Vec_3 transformation(Vec_3 point){
        Mat_4 res = mat_multi({point.x,0,0,0,point.y,0,0,0,point.z,0,0,0,1,0,0,0});
        if(res.m[3][0] != 0.0f){
            float w = res.m[3][0];
            // here we are dividing it by w for perspecitve
            return {res.m[0][0]/w,res.m[1][0]/w,res.m[2][0]/w};
        }
        return {res.m[0][0],res.m[1][0],res.m[2][0]};
    }
};

#endif
