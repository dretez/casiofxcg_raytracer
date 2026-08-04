#ifndef INCLUDE_FIXED_UQ0_16_H
#define INCLUDE_FIXED_UQ0_16_H

#include <gint/defs/types.h>

/** Unsigned Q0.16 fixed-point value. */
typedef u16 uq0_16;

/** Number of fractional bits in the Q0.16 format. */
#define UQ0_16_SHIFT 16
/** The approximate representation of 1 in Q0.16 format. */
#define UQ0_16_ONE ((uq0_16)((1u << UQ0_16_SHIFT) - 1u))
/** The approximate representation of 0.5 in Q0.16 format. */
#define UQ0_16_HALF ((uq0_16)(1u << (UQ0_16_SHIFT - 1)))

/* ************************************************************************** */
/* ****************************** CONVERSIONS ******************************* */
/* ************************************************************************** */

/**
 * Convert a floating-point value to Q0.16.
 *
 * @param f Floating-point value.
 * @return Approximate Q0.16 value.
 */
#define UQ0_16_FROM_FLOAT(f)                                                                       \
    ((uq0_16)((f) <= 0.0f ? 0 : (f) >= 1.0f ? UQ0_16_ONE : ((f) * (float)UQ0_16_ONE + 0.5f)))

/**
 * Convert a floating-point value to Q0.16.
 *
 * @param x Floating-point value.
 * @return Approximate Q0.16 value.
 */
static inline uq0_16 uq0_16_from_float(float x) {
    return UQ0_16_FROM_FLOAT(x);
}

/**
 * Convert a floating-point value to Q0.16.
 *
 * @warning This function assumes the provided float sits in the range [0, 1] and will wrap with
 *          values outside this range. When unsure, use `uq0_16_from_float()` instead.
 *
 * @param x Floating-point value.
 * @return Approximate Q0.16 value.
 */
static inline uq0_16 uq0_16_from_unitfloat(float x) {
    return (uq0_16)(x * (float)UQ0_16_ONE + 0.5);
}

/**
 * Convert a Q0.16 value to floating point.
 *
 * @param x Fixed-point value.
 * @return Floating-point representation.
 */
static inline float uq0_16_to_float(uq0_16 x) {
    return (float)x / (float)UQ0_16_ONE;
}

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

/**
 * Add two Q0.16 values.
 *
 * The result is saturated to the representable Q0.16 range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated sum.
 */
static inline uq0_16 uq0_16_add(uq0_16 a, uq0_16 b) {
    u32 c = (u32)a + (u32)b;
    return c > UQ0_16_ONE ? UQ0_16_ONE : (uq0_16)c;
}

/**
 * Subtract two Q16.15 values.
 *
 * The result is saturated to the representable range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated difference.
 */
static inline uq0_16 uq0_16_sub(uq0_16 a, uq0_16 b) {
    return a < b ? 0 : a - b;
}

/**
 * Multiply two Q0.16 values.
 *
 * The product is rescaled back to Q0.16 by discarding the lower fractional bits.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Product in Q0.16 format.
 */
static inline uq0_16 uq0_16_mul(uq0_16 a, uq0_16 b) {
    return (uq0_16)(((u32)a * (u32)b + UQ0_16_HALF) >> UQ0_16_SHIFT);
}

/**
 * Divide one Q0.16 value by another.
 *
 * The numerator is shifted before division to preserve the fractional precision.
 *
 * @warning Division by zero is undefined.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return Quotient.
 */
static inline uq0_16 uq0_16_div(uq0_16 a, uq0_16 b) {
    return (uq0_16)(((u32)a << UQ0_16_SHIFT) / b);
}

#endif /* ifndef INCLUDE_FIXED_UQ0_16_H */
