#ifndef INCLUDE_VECTOR_H
#define INCLUDE_VECTOR_H

#include "Fixed.h"

/* ************************************************************************** */
/* ********************** DEFINITION & INITIALIZATION *********************** */
/* ************************************************************************** */

typedef struct Vector2 {
    float x;
    float y;
} Vec2;

typedef struct {
    q16_15 x;
    q16_15 y;
    q16_15 z;
} Vec3;

/**
 * Initializes a Vec3
 */
static inline Vec3 vec(q16_15 x, q16_15 y, q16_15 z) {
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
        .x = q16_15_from_float(x),
        .y = q16_15_from_float(y),
        .z = q16_15_from_float(z),
    };
}

/**
 * Calculates the element-wise sum of two Vec3
 */
static inline Vec3 Vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = q16_15_add(a.x, b.x),
        .y = q16_15_add(a.y, b.y),
        .z = q16_15_add(a.z, b.z),
    };
}

/**
 * Calculates the element-wise difference between two Vec3
 */
static inline Vec3 Vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = q16_15_sub(a.x, b.x),
        .y = q16_15_sub(a.y, b.y),
        .z = q16_15_sub(a.z, b.z),
    };
}

/**
 * Calculates the element-wise product of two Vec3
 */
static inline Vec3 Vec3_mul(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = q16_15_mul(a.x, b.x),
        .y = q16_15_mul(a.y, b.y),
        .z = q16_15_mul(a.z, b.z),
    };
}

/**
 * Calculates the cross product of two Vec3
 */
static inline Vec3 Vec3_cross(Vec3 v1, Vec3 v2) {
    return (Vec3){
        .x = ((i64)v1.y * (i64)v2.z - (i64)v1.z * (i64)v2.y) >> Q16_15_SHIFT,
        .y = ((i64)v1.z * (i64)v2.x - (i64)v1.x * (i64)v2.z) >> Q16_15_SHIFT,
        .z = ((i64)v1.x * (i64)v2.y - (i64)v1.y * (i64)v2.x) >> Q16_15_SHIFT,
    };
}

/**
 * Calculates the dot product of two Vec3
 */
static inline q16_15 Vec3_dot(Vec3 a, Vec3 b) {
    return ((i64)a.x * (i64)b.x + (i64)a.y * (i64)b.y + (i64)a.z * (i64)b.z) >> Q16_15_SHIFT;
}

/**
 * Scales each axis of a Vec3 by a factor `s`
 */
static inline Vec3 Vec3_scale(Vec3 v, q16_15 s) {
    return (Vec3){
        .x = q16_15_mul(v.x, s),
        .y = q16_15_mul(v.y, s),
        .z = q16_15_mul(v.z, s),
    };
}

/**
 * Calculates the length of a Vec3
 */
static inline q16_15 Vec3_length(Vec3 v) {
    return q16_15_sqrt(Vec3_dot(v, v));
}

/**
 * Calculates the inverse length of a Vec3
 */
static inline q16_15 Vec3_invlength(Vec3 v) {
    return q16_15_rsqrt(Vec3_dot(v, v));
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
    return Vec3_sub(d, Vec3_scale(n, 2.0 * Vec3_dot(d, n)));
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
static inline int refract(Vec3 d, Vec3 n, q16_15 eta, Vec3* refracted) {
    q16_15 cosI  = -Vec3_dot(n, d);
    q16_15 eta2  = q16_15_mul(eta, eta);
    q16_15 cosI2 = q16_15_mul(cosI, cosI);
    q16_15 sin2T = q16_15_sub(eta2, q16_15_sub(Q16_15_ONE, cosI2));

    if (sin2T > Q16_15_ONE) return 0;

    q16_15 cosT = q16_15_sqrt(Q16_15_ONE - sin2T);
    q16_15 k    = q16_15_sub(q16_15_mul(eta, cosI), cosT);

    *refracted = Vec3_add(Vec3_scale(d, eta), Vec3_scale(n, k));
    return 1;
}

#endif /* ifndef INCLUDE_VECTOR_H */
