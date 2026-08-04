#ifndef INCLUDE_VECTOR_VECTOR_H
#define INCLUDE_VECTOR_VECTOR_H

#include "Vector/Geometry.h"

/* ************************************************************************** */
/* ********************** DEFINITION & INITIALIZATION *********************** */
/* ************************************************************************** */

typedef struct Vector2 {
    geo_t x;
    geo_t y;
} Vec2;

typedef struct Vector3 {
    geo_t x;
    geo_t y;
    geo_t z;
} Vec3;

#define VEC2_INIT(_x, _y)                                                                          \
    (Vec2) {                                                                                       \
        .x = GEO_FROM_FLOAT(_x), .y = GEO_FROM_FLOAT(_y)                                           \
    }

#define VEC3_INIT(_x, _y, _z)                                                                      \
    (Vec3) {                                                                                       \
        .x = GEO_FROM_FLOAT(_x), .y = GEO_FROM_FLOAT(_y), .z = GEO_FROM_FLOAT(_z)                  \
    }

/**
 * Initializes a Vec3
 */
static inline Vec3 vec(geo_t x, geo_t y, geo_t z) {
    return (Vec3){
        .x = x,
        .y = y,
        .z = z,
    };
}

/**
 * Calculates the element-wise sum of two Vec3
 */
static inline Vec3 Vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = geo_add(a.x, b.x),
        .y = geo_add(a.y, b.y),
        .z = geo_add(a.z, b.z),
    };
}

static inline Vec3 Vec3_add_fast(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

static inline Vec3 Vec3_addScaled(Vec3 a, Vec3 b, geo_t s) {
    geo_t x = geo_mul(b.x, s);
    x       = geo_add(a.x, x);
    geo_t y = geo_mul(b.y, s);
    y       = geo_add(a.y, y);
    geo_t z = geo_mul(b.z, s);
    z       = geo_add(a.z, z);
    return (Vec3){ .x = x, .y = y, .z = z };
}

static inline Vec3 Vec3_addScaled_fast(Vec3 a, Vec3 b, geo_t s) {
    geo_t x = geo_mul(b.x, s) + a.x;
    geo_t y = geo_mul(b.y, s) + a.y;
    geo_t z = geo_mul(b.z, s) + a.z;
    return (Vec3){ .x = x, .y = y, .z = z };
}

/**
 * Calculates the element-wise difference between two Vec3
 */
static inline Vec3 Vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){
        .x = geo_sub(a.x, b.x),
        .y = geo_sub(a.y, b.y),
        .z = geo_sub(a.z, b.z),
    };
}

static inline Vec3 Vec3_sub_fast(Vec3 a, Vec3 b) {
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
        .x = geo_mul(a.x, b.x),
        .y = geo_mul(a.y, b.y),
        .z = geo_mul(a.z, b.z),
    };
}

/**
 * Calculates the cross product of two Vec3
 */
static inline Vec3 Vec3_cross(Vec3 v1, Vec3 v2) {
    geo_t x = geo_mul(v1.y, v2.z);
    x       = geo_sub(x, geo_mul(v1.z, v2.y));
    geo_t y = geo_mul(v1.z, v2.x);
    y       = geo_sub(y, geo_mul(v1.x, v2.z));
    geo_t z = geo_mul(v1.x, v2.y);
    z       = geo_sub(z, geo_mul(v1.y, v2.x));
    return (Vec3){ .x = x, .y = y, .z = z };
}

/**
 * Calculates the dot product of two Vec3
 */
static inline geo_t Vec3_dot(Vec3 a, Vec3 b) {
    geo_t c = geo_mul(a.x, b.x);
    c       = geo_add(c, geo_mul(a.y, b.y));
    return geo_add(c, geo_mul(a.z, b.z));
}

static inline geo_t Vec3_dot_fast(Vec3 a, Vec3 b) {
    geo_t c = geo_mul(a.x, b.x);
    c       = geo_add_fast(c, geo_mul(a.y, b.y));
    return geo_add_fast(c, geo_mul(a.z, b.z));
}

/**
 * Scales each axis of a Vec3 by a factor `s`
 */
static inline Vec3 Vec3_scale(Vec3 v, geo_t s) {
    return (Vec3){
        .x = geo_mul(v.x, s),
        .y = geo_mul(v.y, s),
        .z = geo_mul(v.z, s),
    };
}

/**
 * Calculates the length of a Vec3
 */
static inline geo_t Vec3_length(Vec3 v) {
    return geo_sqrt(Vec3_dot(v, v));
}

/**
 * Calculates the inverse length of a Vec3
 */
static inline geo_t Vec3_invlength(Vec3 v) {
    return geo_rsqrt(Vec3_dot(v, v));
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
static inline Vec3 Vec3_reflect(Vec3 d, Vec3 n, geo_t cosI) {
    return Vec3_addScaled_fast(d, n, geo_mul_exp_2(cosI, 1));
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
static inline int Vec3_refract(Vec3 d, Vec3 n, geo_t cosI, geo_t eta, Vec3* refracted) {
    geo_t eta2  = geo_mul(eta, eta);
    geo_t cosI2 = geo_mul(cosI, cosI);
    geo_t sin2T = geo_mul(eta2, geo_sub_fast(GEO_ONE, cosI2));

    if (sin2T > GEO_ONE) return 0;

    geo_t cosT = geo_sqrt(geo_sub_fast(GEO_ONE, sin2T));
    geo_t k    = geo_sub_fast(geo_mul(eta, cosI), cosT);

    *refracted = Vec3_addScaled_fast(Vec3_scale(d, eta), n, k);
    return 1;
}

#endif /* ifndef INCLUDE_VECTOR_VECTOR_H */
