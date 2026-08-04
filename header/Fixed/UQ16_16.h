#ifndef INCLUDE_FIXED_UQ16_16_H
#define INCLUDE_FIXED_UQ16_16_H

#include <gint/defs/types.h>

typedef u16 uq0_16;
/** Unsigned Q16.16 fixed-point value. */
typedef u32 uq16_16;

/** Number of fractional bits in the Q16.16 format. */
#define UQ16_16_SHIFT 16
/** The approximate representation of 1 in Q16.16 format. */
#define UQ16_16_ONE ((uq16_16)(1u << UQ16_16_SHIFT))
/** The approximate representation of 0.5 in Q16.16 format. */
#define UQ16_16_HALF ((uq16_16)(1u << (UQ16_16_SHIFT - 1)))

/** The approximate representation of √2 in Q16.16 format. */
#define UQ16_16_SQRT_2 0x16a0a
/** The approximate representation of 1/√2 in Q16.16 format. */
#define UQ16_16_RSQRT_2 0xb505

/* ************************************************************************** */
/* ****************************** CONVERSIONS ******************************* */
/* ************************************************************************** */

/**
 * Convert a floating-point value to Q16.16.
 *
 * @param f Floating-point value.
 * @return Approximate Q16.16 value.
 */
#define UQ16_16_FROM_FLOAT(f) ((uq16_16)((f) <= 0.0f ? 0 : (f) * (float)UQ16_16_ONE + 0.5f))

/**
 * Convert a floating-point value to Q16.16.
 *
 * @param x Floating-point value.
 * @return Approximate Q16.16 value.
 */
static inline uq16_16 uq16_16_from_float(float x) {
    return UQ16_16_FROM_FLOAT(x);
}

/**
 * Convert a Q16.16 value to floating point.
 *
 * @param x Fixed-point value.
 * @return Floating-point representation.
 */
static inline float uq16_16_to_float(uq16_16 x) {
    return (float)x / (float)UQ16_16_ONE;
}

/**
 * Convert a Q16.16 value to Q0.16.
 *
 * Values larger than the representable Q0.16 range are saturated.
 *
 * @param x Input value.
 * @return Equivalent Q0.16 value.
 */
static inline uq0_16 uq16_16_to_uq0_16(uq16_16 x) {
    return x > UQ16_16_ONE ? (uq0_16)UQ16_16_ONE : (uq0_16)x;
}

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

/**
 * Add two Q16.16 values.
 *
 * The result is saturated to the representable Q16.16 range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated sum.
 */
static inline uq16_16 uq16_16_add(uq16_16 a, uq16_16 b) {
    u64 c = ((u64)a + (u64)b);
    return c > UINT32_MAX ? UINT32_MAX : (uq16_16)c;
}

/**
 * Subtract two Q16.16 values.
 *
 * The result is saturated to the representable range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated difference.
 */
static inline uq16_16 uq16_16_sub(uq16_16 a, uq16_16 b) {
    return a < b ? 0 : a - b;
}

/**
 * Multiply two Q16.16 values.
 *
 * The product is rescaled back to Q16.16 by discarding the lower fractional bits.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Product in Q16.16 format.
 */
static inline uq16_16 uq16_16_mul(uq16_16 a, uq16_16 b) {
    return (uq16_16)(((u64)a * (u64)b + UQ16_16_HALF) >> UQ16_16_SHIFT);
}

/**
 * Divide one Q16.16 value by another.
 *
 * The numerator is shifted before division to preserve the fractional precision.
 *
 * @warning Division by zero is undefined.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return Quotient.
 */
static inline uq16_16 uq16_16_div(uq16_16 a, uq16_16 b) {
    return (uq16_16)(((u64)a << UQ16_16_SHIFT) / b);
}

/**
 * Compute the reciprocal (1 / x) in Q16.16 format.
 *
 * @warning Division by zero is undefined.
 *
 * @param x Input value.
 * @return Reciprocal of @p x.
 */
static inline uq16_16 uq16_16_reciprocal(uq16_16 x) {
    return (uq16_16)(((u64)UQ16_16_ONE << UQ16_16_SHIFT) / x);
}

// PERF: implement lookup table
/**
 * Compute the inverse square root in Q16.16 format.
 *
 * Uses a linear initial approximation followed by two Newton-Raphson refinement iterations.
 *
 * For non-positive inputs, the function returns the maximum representable value.
 *
 * @param x Positive fixed-point value.
 * @return Approximation of 1/sqrt(x).
 */
static inline uq16_16 uq16_16_rsqrt(uq16_16 x) {
    if (x <= 0)
        return UINT32_MAX;
    int msb = 31 - __builtin_clz(x);
    int shift = UQ16_16_SHIFT - 1 - msb;
    // normalize x to [0.5,1.0[
    uq16_16 m = shift >= 0 ? x << shift : x >> -shift;
    // initial approximation: y ≃ 1.912 - 0.912m
    uq16_16 y = uq16_16_from_float(1.912f) - uq16_16_mul(uq16_16_from_float(0.912f), m);
    // 2 iterations of Newton-Raphson
    for (int i = 0; i < 2; i++) {
        uq16_16 yy = uq16_16_mul(y, y);
        uq16_16 myy = uq16_16_mul(m, yy);

        y = uq16_16_mul(y, (3 * UQ16_16_ONE - myy));
        y >>= 1;
    }
    // undo normalization
    y = shift >= 0 ? y << (shift / 2) : y >> (-shift / 2);
    // Handle odd powers of two.
    if (shift & 1)
        y = uq16_16_mul(y, shift > 0 ? UQ16_16_SQRT_2 : UQ16_16_RSQRT_2);

    return y;
}

/**
 * @brief Compute the square root in Q16.16 format.
 *
 * @param x Input value.
 * @return Approximation of sqrt(x).
 */
static inline uq16_16 uq16_16_sqrt(uq16_16 x) {
    return uq16_16_mul(x, uq16_16_rsqrt(x));
}

#endif /* ifndef INCLUDE_FIXED_UQ16_16_H */
