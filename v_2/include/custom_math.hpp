#ifndef _CUSTOM_MATH_
#define _CUSTOM_MATH_

#include <cmath>
#include <array>

// used ai for math library, but i will look at the inner working of the fuctions

struct Int3{
    int x , y , z;
};

struct Float2 {
    float x , y;
};

constexpr float PI = 3.14159265358979323846f;
inline float radians(float degrees) { return degrees * (PI / 180.0f); }

struct Float3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;

    Float3() = default;
    Float3(float x, float y, float z) : x(x), y(y), z(z) {}
    Float3(float v) : x(v), y(v), z(v) {}

    Float3 operator+(const Float3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Float3 operator-(const Float3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Float3 operator*(float s) const { return {x * s, y * s, z * s}; }
    Float3 operator/(float s) const { return {x / s, y / s, z / s}; }
};

inline float length(const Float3& v) { return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
inline Float3 normalize(const Float3& v) {
    float len = length(v);
    return len > 0.0f ? v * (1.0f / len) : Float3{0,0,0};
}
inline float dot(const Float3& a, const Float3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
inline Float3 cross(const Float3& a, const Float3& b) {
    return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
}

struct Float4 {
    float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

    Float4() = default;
    Float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Float4(const Float3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
};

// row major Float4x4
struct Float4x4 {
    std::array<float, 16> m{};
    Float4x4() = default;

    static Float4x4 identity() {
        Float4x4 mat;
        mat.m[0] = 1.0f; mat.m[5] = 1.0f; mat.m[10] = 1.0f; mat.m[15] = 1.0f;
        return mat;
    }
};

inline Float4x4 mul(const Float4x4& a, const Float4x4& b) {
    Float4x4 out;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            float sum = 0.0f;
            for (int i = 0; i < 4; ++i) {
                sum += a.m[row * 4 + i] * b.m[i * 4 + col];
            }
            out.m[row * 4 + col] = sum;
        }
    }
    return out;
}

inline Float4 mul(const Float4x4& mat, const Float4& v) {
    return {
        mat.m[0]*v.x + mat.m[1]*v.y + mat.m[2]*v.z  + mat.m[3]*v.w,
        mat.m[4]*v.x + mat.m[5]*v.y + mat.m[6]*v.z  + mat.m[7]*v.w,
        mat.m[8]*v.x + mat.m[9]*v.y + mat.m[10]*v.z + mat.m[11]*v.w,
        mat.m[12]*v.x + mat.m[13]*v.y + mat.m[14]*v.z + mat.m[15]*v.w
    };
}

inline Float4x4 get_translate_matrix(const Float4x4& m, const Float3& v) {
    Float4x4 out = m;
    out.m[3]  += m.m[0]*v.x + m.m[1]*v.y + m.m[2]*v.z;
    out.m[7]  += m.m[4]*v.x + m.m[5]*v.y + m.m[6]*v.z;
    out.m[11] += m.m[8]*v.x + m.m[9]*v.y + m.m[10]*v.z;
    out.m[15] += m.m[12]*v.x + m.m[13]*v.y + m.m[14]*v.z;
    return out;
}

// implement
// roation
// scaling
// here , but i am not doing that right now because i dont know if we will pass degree , radion
// or i have heard something call quaternion is used and is better ( idk how and what is it but i will implement it later)

inline Float4x4 get_view_matrix(const Float3& eye, const Float3& f, const Float3& up) {
    Float3 s = normalize(cross(f, up));
    Float3 u = cross(s, f);

    Float4x4 out = Float4x4::identity();
    out.m[0] = s.x;  out.m[1] = s.y;  out.m[2] = s.z;  out.m[3] = -dot(s, eye);
    out.m[4] = u.x;  out.m[5] = u.y;  out.m[6] = u.z;  out.m[7] = -dot(u, eye);
    out.m[8] = -f.x; out.m[9] = -f.y; out.m[10] = -f.z; out.m[11] = dot(f, eye);

    return out;
}

inline Float4x4 get_perspective_matrix(float fovRad, float aspect, float nearZ, float farZ) {
    float tanHalfFov = std::tan(fovRad / 2.0f);
    Float4x4 out;

    out.m[0] = 1.0f / (aspect * tanHalfFov);
    out.m[5] = -1.0f / tanHalfFov;
    out.m[10] = farZ / (nearZ - farZ);
    out.m[11] = (nearZ * farZ) / (nearZ - farZ);
    out.m[14] = -1.0f;

    return out;
}

#endif
