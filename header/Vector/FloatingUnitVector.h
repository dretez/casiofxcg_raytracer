#ifndef INCLUDE_UNITVECTOR_H
#define INCLUDE_UNITVECTOR_H

#include <math.h>

#include "Vector/FloatingVector.h"

/* ************************************************************************** */
/* ********************** DEFINITION & INITIALIZATION *********************** */
/* ************************************************************************** */

typedef struct UnitVector2 {
    float x;
    float y;
} UnitVec2;

typedef struct UnitVector3 {
    float x;
    float y;
    float z;
} UnitVec3;

/**
 * Initializes a UnitVec3
 */
static inline UnitVec3 unitvec(float x, float y, float z) {
    return (UnitVec3){
        .x = x,
        .y = y,
        .z = z,
    };
}

/**
 * Converts a Vec3 that has already been normalized into a UnitVec3.
 * This functions does no checks regarding the normalization of the provided
 * vector, and should only be used when the provided vector is absolutely
 * guaranteed to be already normalized.
 *
 * Otherwise refer to `UnitVec3_from_vec3()`
 */
static inline UnitVec3 UnitVec3_from_normalvec3(Vec3 v) {
    return (UnitVec3){
        .x = v.x,
        .y = v.y,
        .z = v.z,
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

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

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
static inline float UnitVec3_dot(UnitVec3 uv1, UnitVec3 uv2) {
    return uv1.x * uv2.x + uv1.y * uv2.y + uv1.z * uv2.z;
}

/**
 * Adds a UnitVec3 to a regular Vec3
 */
static inline Vec3 UnitVec3_add(UnitVec3 uv, Vec3 v) {
    return (Vec3){
        .x = uv.x + v.x,
        .y = uv.y + v.y,
        .z = uv.z + v.z,
    };
}

/**
 * Scales a UnitVec3 into a regular Vec3
 */
static inline Vec3 UnitVec3_scale(UnitVec3 uv, float s) {
    return (Vec3){
        .x = uv.x * s,
        .y = uv.y * s,
        .z = uv.z * s,
    };
}

/**
 * Calculates the cross product of two UnitVec3
 */
static inline Vec3 UnitVec3_cross(UnitVec3 a, UnitVec3 b) {
    return (Vec3){
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x,
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
    float ddotn = UnitVec3_dot(d, n);
    return (UnitVec3){
        .x = d.x - (ddotn * n.x),
        .y = d.y - (ddotn * n.y),
        .z = d.z - (ddotn * n.z),
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
static inline int UnitVec3_refract(UnitVec3 d, UnitVec3 n, float eta, UnitVec3* refracted) {
    float cosI  = -UnitVec3_dot(n, d);
    float eta2  = eta * eta;
    float cosI2 = cosI * cosI;
    float sin2T = eta2 * (1.0f - cosI2);

    if (sin2T > 1.0f) return 0;

    float cosT = sqrtf(1.0f - sin2T);
    float k    = (eta * cosI) - cosT;

    *refracted = UnitVec3_from_vec3(Vec3_add(UnitVec3_scale(d, eta), UnitVec3_scale(n, k)));
    return 1;
}

#endif /* ifndef INCLUDE_UNITVECTOR_H */
