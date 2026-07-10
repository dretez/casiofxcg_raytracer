#ifndef INCLUDE_UNITVECTOR_H
#define INCLUDE_UNITVECTOR_H

#include <math.h>

#include "Fixed.h"
#include "Vector/FixedVector.h"

typedef struct {
    q0_15 x, y;
} UnitVec2;

#define VEC2_FROM_FLOAT(x, y) ((UnitVec2){ Q0_15_FROM_FLOAT(x), Q0_15_FROM_FLOAT(y) })

typedef struct {
    q0_15 x, y, z;
} UnitVec3;

/**
 * Initializes a UnitVec3
 */
static inline UnitVec3 unitvec(q0_15 x, q0_15 y, q0_15 z) {
    return (UnitVec3){ x, y, z };
}

/**
 * Converts a Vec3 that has already been normalized into a UnitVec3
 */
static inline UnitVec3 UnitVec3_from_normalvec3(Vec3 v) {
    return (UnitVec3){
        .x = q16_15_to_q0_15(v.x),
        .y = q16_15_to_q0_15(v.y),
        .z = q16_15_to_q0_15(v.z),
    };
}

/**
 * Converts a Vec3 into a UnitVec3
 */
static inline UnitVec3 UnitVec3_from_vec3(Vec3 v) {
    return UnitVec3_from_normalvec3(Vec3_normalize(v));
}

/**
 * Converts a UnitVec3 into a Vec3
 */
static inline Vec3 UnitVec3_to_vec3(UnitVec3 uv) {
    return vec(uv.x, uv.y, uv.z);
}

/**
 * Returns a copy of a UnitVec3 with every axis flipped
 */
static inline UnitVec3 UnitVec3_neg(UnitVec3 uv) {
    return (UnitVec3){
        .x = -uv.x,
        .y = -uv.y,
        .z = -uv.z,
    };
}

/**
 * Calculates the dot product of 2 UnitVec3
 */
static inline q16_15 UnitVec3_dot(UnitVec3 uv1, UnitVec3 uv2) {
    q16_15 x = q16_15_mul(uv1.x, uv2.x);
    q16_15 y = q16_15_mul(uv1.y, uv2.y);
    q16_15 z = q16_15_mul(uv1.z, uv2.z);
    return q16_15_add(x, q16_15_add(y, z));
}

/* ************************************************************************** */
/* ******************************** VEC3 OPS ******************************** */
/* ************************************************************************** */

/**
 * Adds a UnitVec3 to a regular Vec3
 */
static inline Vec3 UnitVec3_add(UnitVec3 uv, Vec3 v) {
    return (Vec3){
        .x = q16_15_add(uv.x, v.x),
        .y = q16_15_add(uv.y, v.y),
        .z = q16_15_add(uv.z, v.z),
    };
}

/**
 * Scales a UnitVec3 into a regular Vec3
 */
static inline Vec3 UnitVec3_scale(UnitVec3 uv, q16_15 s) {
    return (Vec3){
        .x = q16_15_mul(uv.x, s),
        .y = q16_15_mul(uv.y, s),
        .z = q16_15_mul(uv.z, s),
    };
}

/**
 * Calculates the cross product of two UnitVec3
 */
static inline Vec3 UnitVec3_cross(UnitVec3 a, UnitVec3 b) {
    return (Vec3){
        .x = q16_15_sub(q16_15_mul(a.y, b.z), q16_15_mul(a.z, b.y)),
        .y = q16_15_sub(q16_15_mul(a.z, b.x), q16_15_mul(a.x, b.z)),
        .z = q16_15_sub(q16_15_mul(a.x, b.y), q16_15_mul(a.y, b.x)),
    };
}

/**
 * Calculates the cross product of two Vec3 and normalizes it to a UnitVec3
 */
static inline UnitVec3 Vec3_crossNormalize(Vec3 a, Vec3 b) {
    return UnitVec3_from_vec3(Vec3_cross(a, b));
}

/**
 * Calculates the cross product of two UnitVec3 and normalizes it to a UnitVec3
 */
static inline UnitVec3 UnitVec3_crossNormalize(UnitVec3 a, UnitVec3 b) {
    return UnitVec3_from_vec3(UnitVec3_cross(a, b));
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
static inline UnitVec3 UnitVec3_reflect(UnitVec3 d, UnitVec3 n) {
    q16_15 ddotn = UnitVec3_dot(d, n) << 1;
    return (UnitVec3){
        .x = q16_15_to_q0_15(q16_15_sub(d.x, q16_15_mul(ddotn, n.x))),
        .y = q16_15_to_q0_15(q16_15_sub(d.y, q16_15_mul(ddotn, n.y))),
        .z = q16_15_to_q0_15(q16_15_sub(d.z, q16_15_mul(ddotn, n.z))),
    };
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
static inline int UnitVec3_refract(UnitVec3 d, UnitVec3 n, q16_15 eta, Vec3* refracted) {
    q16_15 cosI  = -UnitVec3_dot(n, d);
    q16_15 eta2  = q16_15_mul(eta, eta);
    q16_15 cosI2 = q16_15_mul(cosI, cosI);
    q16_15 sin2T = q16_15_mul(eta2, (Q16_15_ONE - cosI2));

    if (sin2T > Q16_15_ONE) return 0;

    q16_15 cosT  = q16_15_sqrt(Q16_15_ONE - sin2T);
    q16_15 k     = q16_15_mul(eta, cosI) - cosT;

    refracted->x = q16_15_add(q16_15_mul(d.x, eta), q16_15_mul(n.x, k));
    refracted->y = q16_15_add(q16_15_mul(d.y, eta), q16_15_mul(n.y, k));
    refracted->z = q16_15_add(q16_15_mul(d.z, eta), q16_15_mul(n.z, k));
    return 1;
}

#endif /* ifndef INCLUDE_UNITVECTOR_H */
