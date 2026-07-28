#ifndef INCLUDE_VECTOR_GEOMETRY
#define INCLUDE_VECTOR_GEOMETRY

#include <math.h>
#include <float.h>

#include "Color.h"
#include "Fixed.h"

typedef float geo_t;

#define GEO_ZERO 0.0f
#define GEO_HALF 0.5f
#define GEO_ONE 1.0f
#define GEO_TWO 2.0f

#define GEO_MIN FLT_MIN
#define GEO_MAX FLT_MAX

#define GEO_FROM_FLOAT(_x) (_x)

static inline geo_t geo_from_float(float x) {
    return GEO_FROM_FLOAT(x);
}

static inline geo_t geo_abs(geo_t x) {
    return fabsf(x);
}

static inline color_t geo_to_color(geo_t x) {
    return float2color(x);
}

static inline geo_t geo_add(geo_t a, geo_t b) {
    return a + b;
}

static inline geo_t geo_sub(geo_t a, geo_t b) {
    return a - b;
}

static inline geo_t geo_mul(geo_t a, geo_t b) {
    return a * b;
}

static inline geo_t geo_div(geo_t a, geo_t b) {
    return a / b;
}

static inline geo_t geo_inv(geo_t x) {
    return 1.0f / x;
}

static inline geo_t geo_sqrt(geo_t x) {
    return sqrtf(x);
}

static inline geo_t geo_rsqrt(geo_t x) {
    return 1.0f / sqrtf(x);
}

#endif /* ifndef INCLUDE_VECTOR_GEOMETRY */
