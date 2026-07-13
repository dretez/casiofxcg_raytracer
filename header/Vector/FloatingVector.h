#ifndef INCLUDE_VECTOR_H
#define INCLUDE_VECTOR_H

#include <math.h>

/* ************************************************************************** */
/* ********************** DEFINITION & INITIALIZATION *********************** */
/* ************************************************************************** */

typedef struct Vector2 {
    float x;
    float y;
} Vec2;

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vec3;

/**
 * Initializes a Vec3
 */
static inline Vec3 vec(float x, float y, float z) {
    return (Vec3){
        .x = x,
        .y = y,
        .z = z,
    };
}

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

/**
 * Calculates the element-wise sum of two Vec3
 */
static inline Vec3 Vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

/**
 * Calculates the element-wise difference between two Vec3
 */
static inline Vec3 Vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
}

/**
 * Calculates the element-wise product of two Vec3
 */
static inline Vec3 Vec3_mul(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z,
    };
}

/**
 * Calculates the cross product of two Vec3
 */
static inline Vec3 Vec3_cross(Vec3 v1, Vec3 v2) {
    return (Vec3){
        .x = v1.y * v2.z - v1.z * v2.y,
        .y = v1.z * v2.x - v1.x * v2.z,
        .z = v1.x * v2.y - v1.y * v2.x,
    };
}

/**
 * Calculates the dot product of two Vec3
 */
static inline float Vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * Scales each axis of a Vec3 by a factor `s`
 */
static inline Vec3 Vec3_scale(Vec3 v, float s) {
    return (Vec3){
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s,
    };
}

/**
 * Calculates the length of a Vec3
 */
static inline float Vec3_length(Vec3 v) {
    return sqrtf(Vec3_dot(v, v));
}

/**
 * Normalizes a Vec3 to length 1
 */
static inline Vec3 Vec3_normalize(Vec3 v) {
    return Vec3_scale(v, 1.0f / Vec3_length(v));
}

/**
 * Returns a copy of a Vec3 with every axis flipped
 */
static inline Vec3 Vec3_neg(Vec3 uv) {
    return (Vec3){
        .x = -uv.x,
        .y = -uv.y,
        .z = -uv.z,
    };
}

/* ************************************************************************** */
/* ********************************* LIGHT ********************************** */
/* ************************************************************************** */

/**
 * Calculates a reflection direction given an incoming direction and a surface
 * normal.
 * @param d incoming direction
 * @param n surface normal
 */
static inline Vec3 Vec3_reflect(Vec3 d, Vec3 n) {
    return Vec3_sub(d, Vec3_scale(n, 2.0 * Vec3_dot(d, n)));
}

/**
 * Calculates a refraction direction given an incoming direction and a surface
 * normal.
 * @param d incoming direction
 * @param n surface normal
 * @param eta relative refraction index (n_incomingMedium / n_refractionMedium)
 * @param [out]refracted pointer to where the resulting refracted direction must
 *                       be written to
 *
 * @return true if refraction occurs
 *         false if total internal reflection occurs
 */
static inline int Vec3_refract(Vec3 d, Vec3 n, float cosI, float eta, Vec3* refracted) {
    float eta2  = eta * eta;
    float cosI2 = cosI * cosI;
    float sin2T = eta2 * (1.0f - cosI2);

    if (sin2T > 1.0f) return 0;

    float cosT = sqrtf(1.0f - sin2T);
    float k    = (eta * cosI) - cosT;

    *refracted = Vec3_add(Vec3_scale(d, eta), Vec3_scale(n, k));
    return 1;
}

#endif /* ifndef INCLUDE_VECTOR_H */
