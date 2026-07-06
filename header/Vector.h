#ifndef INCLUDE_VECTOR_H
#define INCLUDE_VECTOR_H

#include <math.h>

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    float x, y, z;
} Vec3;

static inline Vec3 vec(float x, float y, float z) {
    return (Vec3){x, y, z};
}

static inline Vec3 Vec3_add(Vec3 a, Vec3 b) {
    return vec(a.x + b.x, a.y + b.y, a.z + b.z);
}
static inline Vec3 Vec3_sub(Vec3 a, Vec3 b) {
    return vec(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline Vec3 Vec3_scale(Vec3 v, float s) {
    return vec(v.x * s, v.y * s, v.z * s);
}
static inline float Vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
static inline Vec3 Vec3_mul(Vec3 a, Vec3 b) {
    return vec(a.x * b.x, a.y * b.y, a.z * b.z);
}
static inline Vec3 Vec3_cross(Vec3 v1, Vec3 v2) {
    return vec(v1.y * v2.z - v1.z * v2.y,
               v1.z * v2.x - v1.x * v2.z,
               v1.x * v2.y - v1.y * v2.x);
}

static inline float Vec3_length(Vec3 v) {
    return sqrtf(Vec3_dot(v, v));
}
static inline Vec3 Vec3_normalize(Vec3 v) {
    float invLen = 1.0f / Vec3_length(v);
    return Vec3_scale(v, invLen);
}

#endif /* ifndef INCLUDE_VECTOR_H */
