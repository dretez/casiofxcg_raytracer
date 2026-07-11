#ifndef INCLUDE_COLOR_H
#define INCLUDE_COLOR_H

#include <gint/defs/types.h>

#include "Fixed.h"

#define COLOR_SHIFT UQ0_16_SHIFT
#define COLOR_ONE UQ0_16_ONE
#define COLOR_HALF UQ0_16_HALF

typedef uq0_16  color_t;
typedef uq16_16 color_acc_t;

#define COLOR_FROM_FLOAT(f) UQ0_16_FROM_FLOAT(f)

#define COLOR_RGB(r, g, b)                                                                         \
    (Color) {                                                                                      \
        COLOR_FROM_FLOAT(r), COLOR_FROM_FLOAT(g), COLOR_FROM_FLOAT(b)                              \
    }

typedef struct {
    color_t r;
    color_t g;
    color_t b;
} Color;

typedef struct {
    color_acc_t r;
    color_acc_t g;
    color_acc_t b;
} ColorAccumulator;

static inline Color color(color_t r, color_t g, color_t b) {
    return (Color){ r, g, b };
}
static inline color_t float2color(float x) {
    return COLOR_FROM_FLOAT(x);
}
static inline float color2float(color_t x) {
    return uq0_16_to_float(x);
}

static inline color_t colorAdd(color_t a, color_t b) {
    return uq0_16_add(a, b);
}

static inline color_t colorSub(color_t a, color_t b) {
    return uq0_16_sub(a, b);
}

static inline color_t colorMul(color_t a, color_t b) {
    return uq0_16_mul(a, b);
}

static inline Color Color_mul(Color a, Color b) {
    return (Color){
        colorMul(a.r, b.r),
        colorMul(a.g, b.g),
        colorMul(a.b, b.b),
    };
}

static inline Color Color_scale(Color c, color_t s) {
    return (Color){
        colorMul(c.r, s),
        colorMul(c.g, s),
        colorMul(c.b, s),
    };
}

static inline Color Color_add(Color c1, Color c2) {
    return (Color){
        uq0_16_add(c1.r, c2.r),
        uq0_16_add(c1.g, c2.g),
        uq0_16_add(c1.b, c2.b),
    };
}

static inline void ColorAccumulator_clear(ColorAccumulator* acc) {
    acc->r = 0;
    acc->g = 0;
    acc->b = 0;
}

static inline void ColorAccumulator_add(ColorAccumulator* acc, Color c) {
    acc->r = uq16_16_add(acc->r, c.r);
    acc->g = uq16_16_add(acc->g, c.g);
    acc->b = uq16_16_add(acc->b, c.b);
}

static inline void ColorAccumulator_addScaled(ColorAccumulator* acc, Color c, color_t scale) {
    acc->r = uq16_16_add(acc->r, uq16_16_mul(c.r, scale));
    acc->g = uq16_16_add(acc->g, uq16_16_mul(c.g, scale));
    acc->b = uq16_16_add(acc->b, uq16_16_mul(c.b, scale));
}

static inline void ColorAccumulator_scale(ColorAccumulator* acc, color_t scale) {
    acc->r = uq16_16_mul(acc->r, scale);
    acc->g = uq16_16_mul(acc->g, scale);
    acc->b = uq16_16_mul(acc->b, scale);
}

static inline Color ColorAccumulator_toColor(const ColorAccumulator* acc) {
    return (Color){
        uq16_16_to_uq0_16(acc->r),
        uq16_16_to_uq0_16(acc->g),
        uq16_16_to_uq0_16(acc->b),
    };
}

static inline u16 color2rgb565(Color color) {
    return ((color.r >> (COLOR_SHIFT - 5)) << 11) |
           ((color.g >> (COLOR_SHIFT - 6)) << 5) |
           ((color.b >> (COLOR_SHIFT - 5)));
}

#endif /* ifndef INCLUDE_COLOR_H */
