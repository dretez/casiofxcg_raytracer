#ifndef INCLUDE_VECTOR_H
#define INCLUDE_VECTOR_H

#include "Fixed.h"

/* ************************************************************************** */
/* ********************** DEFINITION & INITIALIZATION *********************** */
/* ************************************************************************** */

typedef struct Vector2 {
    q31_32 x;
    q31_32 y;
} Vec2;

typedef struct Vector3 {
    q31_32 x;
    q31_32 y;
    q31_32 z;
} Vec3;

#define VEC2_INIT(_x, _y)                                                                    \
    (Vec2) {                                                                                       \
        .x = Q31_32_FROM_FLOAT(_x), .y = Q31_32_FROM_FLOAT(_y)                                     \
    }

#define VEC3_INIT(_x, _y, _z)                                                                \
    (Vec3) {                                                                                       \
        .x = Q31_32_FROM_FLOAT(_x), .y = Q31_32_FROM_FLOAT(_y), .z = Q31_32_FROM_FLOAT(_z)         \
    }

/**
 * Initializes a Vec3
 */
static inline Vec3 vec(q31_32 x, q31_32 y, q31_32 z) {
    return (Vec3){
        .x = x,
        .y = y,
        .z = z,
    };
}

/**
 * Initializes a Vec3 with floats
 */
static inline Vec3 vec_from_float(float x, float y, float z) {
    return (Vec3){
        .x = q31_32_from_float(x),
        .y = q31_32_from_float(y),
        .z = q31_32_from_float(z),
    };
}

/**
 * Calculates the element-wise sum of two Vec3
 */
static inline Vec3 Vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = q31_32_add(a.x, b.x),
        .y = q31_32_add(a.y, b.y),
        .z = q31_32_add(a.z, b.z),
    };
}

/**
 * Calculates the element-wise difference between two Vec3
 */
static inline Vec3 Vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = q31_32_sub(a.x, b.x),
        .y = q31_32_sub(a.y, b.y),
        .z = q31_32_sub(a.z, b.z),
    };
}

/**
 * Calculates the element-wise product of two Vec3
 */
static inline Vec3 Vec3_mul(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = q31_32_mul(a.x, b.x),
        .y = q31_32_mul(a.y, b.y),
        .z = q31_32_mul(a.z, b.z),
    };
}

/**
 * Calculates the cross product of two Vec3
 */
static inline Vec3 Vec3_cross(Vec3 v1, Vec3 v2) {
    return (Vec3){
        .x = q31_32_sub(q31_32_mul(v1.y, v2.z), q31_32_mul(v1.z, v2.y)),
        .y = q31_32_sub(q31_32_mul(v1.z, v2.x), q31_32_mul(v1.x, v2.z)),
        .z = q31_32_sub(q31_32_mul(v1.x, v2.y), q31_32_mul(v1.y, v2.x)),
    };
}

/**
 * Calculates the dot product of two Vec3
 */
static inline q16_15 Vec3_dot(Vec3 a, Vec3 b) {
    return q31_32_add(q31_32_mul(a.x, b.x), q31_32_add(q31_32_mul(a.y, b.y), q31_32_mul(a.z, b.z)));
}

/**
 * Scales each axis of a Vec3 by a factor `s`
 */
static inline Vec3 Vec3_scale(Vec3 v, q31_32 s) {
    return (Vec3){
        .x = q31_32_mul(v.x, s),
        .y = q31_32_mul(v.y, s),
        .z = q31_32_mul(v.z, s),
    };
}

/**
 * Calculates the length of a Vec3
 */
static inline q16_15 Vec3_length(Vec3 v) {
    return q31_32_sqrt(Vec3_dot(v, v));
}

/**
 * Calculates the inverse length of a Vec3
 */
static inline q16_15 Vec3_invlength(Vec3 v) {
    return q31_32_rsqrt(Vec3_dot(v, v));
}

/**
 * Normalizes a Vec3 to length ~1
 */
static inline Vec3 Vec3_normalize(Vec3 v) {
    return Vec3_scale(v, Vec3_invlength(v));
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
    return Vec3_sub(d, Vec3_scale(n, Q31_32_TWO * Vec3_dot(d, n)));
}

/**
 * Calculates a reflection direction given an incoming direction and a surface
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
static inline int Vec3_refract(Vec3 d, Vec3 n, q31_32 eta, Vec3* refracted) {
    q31_32 cosI  = -Vec3_dot(n, d);
    q31_32 eta2  = q31_32_mul(eta, eta);
    q31_32 cosI2 = q31_32_mul(cosI, cosI);
    q31_32 sin2T = q31_32_sub(eta2, q31_32_sub(Q31_32_ONE, cosI2));

    if (sin2T > Q31_32_ONE) return 0;

    q31_32 cosT = q31_32_sqrt(Q31_32_ONE - sin2T);
    q31_32 k    = q31_32_sub(q31_32_mul(eta, cosI), cosT);

    *refracted = Vec3_add(Vec3_scale(d, eta), Vec3_scale(n, k));
    return 1;
}

#endif /* ifndef INCLUDE_VECTOR_H */
