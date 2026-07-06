#ifndef INCLUDE_COLOR_H
#define INCLUDE_COLOR_H

#include "gint/defs/types.h"

#define COLOR_SHIFT 15
#define COLOR_ONE 0x7FFFu
#define COLOR_HALF (1u << (COLOR_SHIFT - 1))

typedef u16 color_t;
typedef u32 color_acc_t;

#define COLOR_FROM_FLOAT(f)                                                                        \
    ((color_t)(((f) <= 0.0f) ? 0 : ((f) >= 1.0f) ? COLOR_ONE : ((f) * COLOR_ONE + 0.5f)))

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
    return x <= 0.0f ? 0 : x >= 1.0f ? COLOR_ONE : (color_t)(x * COLOR_ONE + 0.5f);
}
static inline float color2float(color_t x) {
    return (float)x / (float)COLOR_ONE;
}

static inline color_t colorMul(color_t a, color_t b) {
    return (color_t)(((color_acc_t)a * (color_acc_t)b + COLOR_HALF) >> COLOR_SHIFT);
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
    color_acc_t r = (color_acc_t)c1.r + c2.r;
    color_acc_t g = (color_acc_t)c1.g + c2.g;
    color_acc_t b = (color_acc_t)c1.b + c2.b;

    if (r > COLOR_ONE) r = COLOR_ONE;
    if (g > COLOR_ONE) g = COLOR_ONE;
    if (b > COLOR_ONE) b = COLOR_ONE;

    return (Color){ (color_t)r, (color_t)g, (color_t)b };
}

static inline void ColorAccumulator_clear(ColorAccumulator* acc) {
    acc->r = 0;
    acc->g = 0;
    acc->b = 0;
}

static inline void ColorAccumulator_add(ColorAccumulator* acc, Color c) {
    acc->r += c.r;
    acc->g += c.g;
    acc->b += c.b;
}

static inline void ColorAccumulator_addScaled(ColorAccumulator* acc, Color c, color_t scale) {
    acc->r += colorMul(c.r, scale);
    acc->g += colorMul(c.g, scale);
    acc->b += colorMul(c.b, scale);
}

static inline void ColorAccumulator_scale(ColorAccumulator* acc, color_t scale) {
    acc->r = ((acc->r * scale) + COLOR_HALF) >> COLOR_SHIFT;
    acc->g = ((acc->g * scale) + COLOR_HALF) >> COLOR_SHIFT;
    acc->b = ((acc->b * scale) + COLOR_HALF) >> COLOR_SHIFT;
}

static inline Color ColorAccumulator_toColor(const ColorAccumulator* acc) {
    color_acc_t r = acc->r;
    color_acc_t g = acc->g;
    color_acc_t b = acc->b;

    if (r > COLOR_ONE) r = COLOR_ONE;
    if (g > COLOR_ONE) g = COLOR_ONE;
    if (b > COLOR_ONE) b = COLOR_ONE;

    return (Color){
        (color_t)r,
        (color_t)g,
        (color_t)b,
    };
}

static inline u16 color2rgb565(Color color) {
    return ((color.r >> 10) << 11) | ((color.g >> 9) << 5) | ((color.b >> 10));
}

#endif /* ifndef INCLUDE_COLOR_H */
