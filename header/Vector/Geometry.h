#ifndef INCLUDE_VECTOR_GEOMETRY
#define INCLUDE_VECTOR_GEOMETRY

#include "Color.h"
#include "Fixed/Q31_32.h"

typedef q31_32 geo_t;

#define GEO_ZERO 0
#define GEO_HALF Q31_32_HALF
#define GEO_ONE Q31_32_ONE
#define GEO_TWO Q31_32_TWO

#define GEO_MIN Q31_32_MIN
#define GEO_MAX Q31_32_MAX

#define GEO_FROM_FLOAT(_x) Q31_32_FROM_FLOAT(_x)

static inline geo_t geo_from_float(float x) {
    return GEO_FROM_FLOAT(x);
}

static inline geo_t geo_abs(geo_t x) {
    return x < 0 ? -x : x;
}

static inline color_t geo_to_color(geo_t x) {
    return x <= GEO_ZERO ? 0 : x >= GEO_ONE ? COLOR_ONE : (color_t)(x >> 16);
}

static inline color_acc_t geo_to_color_acc(geo_t x) {
    return x <= GEO_ZERO ? 0 : (color_acc_t)(x >> 16);
}

static inline geo_t color_to_geo(color_t x) {
    return ((geo_t)x) << 16;
}

static inline geo_t geo_add(geo_t a, geo_t b) {
    return q31_32_add(a, b);
}

static inline geo_t geo_sub(geo_t a, geo_t b) {
    return q31_32_sub(a, b);
}

static inline geo_t geo_mul(geo_t a, geo_t b) {
    return q31_32_mul(a, b);
}

static inline geo_t geo_div(geo_t a, geo_t b) {
    return q31_32_div(a, b);
}

static inline geo_t geo_inv(geo_t x) {
    return q31_32_reciprocal(x);
}

static inline geo_t geo_sqrt(geo_t x) {
    return q31_32_sqrt(x);
}

static inline geo_t geo_rsqrt(geo_t x) {
    return q31_32_rsqrt(x);
}

#endif /* ifndef INCLUDE_VECTOR_GEOMETRY */
