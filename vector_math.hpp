#ifndef _VECTOR_MATH_
#define _VECTOR_MATH_

struct Vec_2{
    public:
        float x , y ;
};

struct Vec_3{
    public:
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

// the following is not needed right now , but we may use vector 4 in cases where we want to move the world/model around in the space
//
// struct Vec_4{
//     public:
//         float x = 0 , y = 0 , z = 0 , w = 1 ;

//         Vec_4 (float x , float y , float z , float w = 1.0f ) : x(x) , y(y) , z(z) , w(w) {}
// };
#endif
