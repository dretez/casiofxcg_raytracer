#ifndef INCLUDE_COLOR_H
#define INCLUDE_COLOR_H

#include <gint/defs/types.h>

#include "Fixed/UQ0_16.h"
#include "Fixed/UQ16_16.h"
#include "Fixed/Q31_32.h"

/* ************************************************************************** */
/* ********************** DEFINITION & INITIALIZATION *********************** */
/* ************************************************************************** */

#define COLOR_SHIFT UQ0_16_SHIFT
#define COLOR_ONE UQ0_16_ONE
#define COLOR_HALF UQ0_16_HALF

/** A color channel represented by an unsigned Q0.16 fixed-point number */
typedef uq0_16 color_t;
/** Accumulator for color_t arithmetic */
typedef uq16_16 color_acc_t;

/**
 * Convert a floating-point value to a color_t representation.
 *
 * @param f Floating-point value.
 * @return Approximate color_t value.
 */
#define COLOR_FROM_FLOAT(f) UQ0_16_FROM_FLOAT(f)

/**
 * Convert a floating-point value to a color_t representation.
 *
 * @param r Floating-point red channel.
 * @param g Floating-point green channel.
 * @param b Floating-point blue channel.
 * @return Color with approximate fixed-point channels.
 */
#define COLOR_RGB(r, g, b)                                                                         \
    (Color) {                                                                                      \
        COLOR_FROM_FLOAT(r), COLOR_FROM_FLOAT(g), COLOR_FROM_FLOAT(b)                              \
    }

/**
 * A Color represented by 3 fixed point color channels (RGB) each ranging from 0 to 1.
 */
typedef struct {
    color_t r;
    color_t g;
    color_t b;
} Color;

/**
 * Accumulator for color values.
 *
 * A ColorAccumulator stores intermediate color sums using a wider fixed-point representation than
 * Color. It is intended for accumulating multiple lighting contributions before converting back to
 * a Color.
 */
typedef struct {
    color_acc_t r;
    color_acc_t g;
    color_acc_t b;
} ColorAccumulator;

/**
 * Initializes a Color struct
 *
 * @param r Fixed-point red channel.
 * @param g Fixed-point green channel.
 * @param b Fixed-point blue channel.
 * @return Color with all color channels set.
 */
static inline Color color(color_t r, color_t g, color_t b) {
    return (Color){ r, g, b };
}

/* ************************************************************************** */
/* ****************************** CONVERSIONS ******************************* */
/* ************************************************************************** */

/**
 * Convert a floating-point value to a fixed-point representation of a color channel.
 *
 * @param x Floating-point value.
 * @return Approximate Q0.15 value.
 */
static inline color_t float2color(float x) {
    return COLOR_FROM_FLOAT(x);
}

/**
 * Convert a Q31.32 value to a fixed-point representation of a color channel.
 *
 * @param x Q31.32 value.
 * @return Q0.15 value.
 */
static inline color_t q31_32_to_color(q31_32 x) {
    return x >> 17;
}

/**
 * Convert a fixed-point color channel to floating point.
 *
 * @param x Fixed-point color channel value.
 * @return Floating-point representation.
 */
static inline float color2float(color_t x) {
    return uq0_16_to_float(x);
}

/**
 * Converts a Color struct into RGB565 format.
 *
 * @param color Input Color.
 * @return RGB565 representation.
 */
static inline u16 color2rgb565(Color color) {
    return ((color.r >> (COLOR_SHIFT - 5)) << 11) | ((color.g >> (COLOR_SHIFT - 6)) << 5) |
           ((color.b >> (COLOR_SHIFT - 5)));
}

/**
 * Collapses a ColorAccumulator into a Color struct.
 *
 * @param acc Accumulator to be converted.
 * @return Color struct with channels capped at 1.
 */
static inline Color ColorAccumulator_toColor(const ColorAccumulator* acc) {
    return (Color){
        uq16_16_to_uq0_16(acc->r),
        uq16_16_to_uq0_16(acc->g),
        uq16_16_to_uq0_16(acc->b),
    };
}

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

/* ******************************** color_t ********************************* */

/**
 * Add two color channel values.
 *
 * The result is saturated to the representable range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated sum.
 */
static inline color_t colorAdd(color_t a, color_t b) {
    return uq0_16_add(a, b);
}

/**
 * Subtract two color channel values.
 *
 * The result is saturated to the representable range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated difference.
 */
static inline color_t colorSub(color_t a, color_t b) {
    return uq0_16_sub(a, b);
}

/**
 * Multiply two color channel values.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Product.
 */
static inline color_t colorMul(color_t a, color_t b) {
    return uq0_16_mul(a, b);
}

/* ********************************* Color ********************************** */

/**
 * Add two colors component-wise.
 *
 * @param c1 Left operand.
 * @param c2 Right operand.
 * @return The component-wise sum.
 */
static inline Color Color_add(Color c1, Color c2) {
    return (Color){
        colorAdd(c1.r, c2.r),
        colorAdd(c1.g, c2.g),
        colorAdd(c1.b, c2.b),
    };
}

/**
 * Multiply two colors component-wise.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return The component-wise product.
 */
static inline Color Color_mul(Color a, Color b) {
    return (Color){
        colorMul(a.r, b.r),
        colorMul(a.g, b.g),
        colorMul(a.b, b.b),
    };
}

/**
 * Scale a color by a scalar.
 *
 * @param c Color to scale.
 * @param s Scale factor.
 * @return The scaled color.
 */
static inline Color Color_scale(Color c, color_t s) {
    return (Color){
        colorMul(c.r, s),
        colorMul(c.g, s),
        colorMul(c.b, s),
    };
}

/* **************************** ColorAccumulator **************************** */

/**
 * Sets all accumulated channels to zero.
 *
 * @param acc Accumulator to clear.
 */
static inline void ColorAccumulator_clear(ColorAccumulator* acc) {
    acc->r = 0;
    acc->g = 0;
    acc->b = 0;
}

/**
 * Add a color to an accumulator.
 *
 * @param acc Destination accumulator.
 * @param c Color contribution to add.
 */
static inline void ColorAccumulator_add(ColorAccumulator* acc, Color c) {
    acc->r = uq16_16_add(acc->r, c.r);
    acc->g = uq16_16_add(acc->g, c.g);
    acc->b = uq16_16_add(acc->b, c.b);
}

/**
 * Add a scaled color to an accumulator.
 *
 * The color is first multiplied by the supplied scale factor, then accumulated.
 *
 * @param acc Destination accumulator.
 * @param c Color contribution.
 * @param scale Scaling factor.
 */
static inline void ColorAccumulator_addScaled(ColorAccumulator* acc, Color c, color_t scale) {
    acc->r = uq16_16_add(acc->r, uq16_16_mul(c.r, scale));
    acc->g = uq16_16_add(acc->g, uq16_16_mul(c.g, scale));
    acc->b = uq16_16_add(acc->b, uq16_16_mul(c.b, scale));
}

/**
 * Scale all accumulated color channels.
 *
 * @param acc Accumulator to scale.
 * @param scale Scaling factor.
 */
static inline void ColorAccumulator_scale(ColorAccumulator* acc, color_t scale) {
    acc->r = uq16_16_mul(acc->r, scale);
    acc->g = uq16_16_mul(acc->g, scale);
    acc->b = uq16_16_mul(acc->b, scale);
}

#endif /* ifndef INCLUDE_COLOR_H */
