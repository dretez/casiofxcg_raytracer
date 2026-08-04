#ifndef INCLUDE_FIXED_Q0_15_H
#define INCLUDE_FIXED_Q0_15_H

#include <gint/defs/types.h>

typedef i16 q0_15;
/** Signed Q16.15 fixed-point value. */
typedef i32 q16_15;

/** Number of fractional bits in the Q16.15 format. */
#define Q16_15_SHIFT 15
/** The approximate representation of 1 in Q16.15 format. */
#define Q16_15_ONE ((q16_15)(1 << Q16_15_SHIFT))
/** The approximate representation of 0.5 in Q16.15 format.√ */
#define Q16_15_HALF ((q16_15)(1 << (Q16_15_SHIFT - 1)))
/** The approximate representation of √2 in Q16.15 format. */
#define Q16_15_SQRT_2 ((q16_15)0xb505)
/** The approximate representation of 1/√2 in Q16.15 format. */
#define Q16_15_RSQRT_2 ((q16_15)0x5a82)

/* ************************************************************************** */
/* ****************************** CONVERSIONS ******************************* */
/* ************************************************************************** */

/**
 * Convert a floating-point value to Q16.15.
 *
 * @param f Floating-point value.
 * @return Approximate Q16.15 value.
 */
#define Q16_15_FROM_FLOAT(f) (q16_15)((f) * (float)Q16_15_ONE + ((f) >= 0.0f ? 0.5f : -0.5f))

/**
 * Convert a floating-point value to Q16.15.
 *
 * @param x Floating-point value.
 * @return Approximate Q16.15 value.
 */
static inline q16_15 q16_15_from_float(float x) {
    return Q16_15_FROM_FLOAT(x);
}

/**
 * Convert a Q16.15 value to floating point.
 *
 * @param x Fixed-point value.
 * @return Floating-point representation.
 */
static inline float q16_15_to_float(q16_15 x) {
    return (float)x / (float)Q16_15_ONE;
}

/**
 * Convert a Q16.15 value to Q0.15.
 *
 * Values larger than the representable Q0.15 range are saturated.
 *
 * @param x Input value.
 * @return Equivalent Q0.15 value.
 */
static inline q0_15 q16_15_to_q0_15(q16_15 x) {
    return x > Q16_15_ONE ? (q0_15)Q16_15_ONE : (q0_15)x;
}

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

/**
 * Add two Q16.15 values.
 *
 * The result is saturated to the representable Q16.15 range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated sum.
 */
static inline q16_15 q16_15_add(q16_15 a, q16_15 b) {
    i64 c = (i64)a + (i64)b;
    return c > INT32_MAX ? INT32_MAX : c < INT32_MIN ? INT32_MIN : (q16_15)c;
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
static inline q16_15 q16_15_sub(q16_15 a, q16_15 b) {
    i64 c = (i64)a - (i64)b;
    return c > INT32_MAX ? INT32_MAX : c < INT32_MIN ? INT32_MIN : (q16_15)c;
}

/**
 * Multiply two Q16.15 values.
 *
 * The product is rescaled back to Q16.15 by discarding the lower fractional bits.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Product in Q16.15 format.
 */
static inline q16_15 q16_15_mul(q16_15 a, q16_15 b) {
    return (q16_15)(((i64)a * (i64)b) >> Q16_15_SHIFT);
}

/**
 * Divide one Q16.15 value by another.
 *
 * The numerator is shifted before division to preserve the fractional precision.
 *
 * @warning Division by zero is undefined.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return Quotient.
 */
static inline q16_15 q16_15_div(q16_15 a, q16_15 b) {
    return (q16_15)(((i64)a << Q16_15_SHIFT) / b);
}

/**
 * Compute the reciprocal (1 / x) in Q16.15 format.
 *
 * @warning Division by zero is undefined.
 *
 * @param x Input value.
 * @return Reciprocal of @p x.
 */
static inline q16_15 q16_15_reciprocal(q16_15 x) {
    return (q16_15)(((i64)Q16_15_ONE << Q16_15_SHIFT) / x);
}

// PERF: implement lookup table
/**
 * Compute the inverse square root in Q16.15 format.
 *
 * Uses a linear initial approximation followed by two Newton-Raphson refinement iterations.
 *
 * For non-positive inputs, the function returns the maximum representable value.
 *
 * @param x Positive fixed-point value.
 * @return Approximation of 1/sqrt(x).
 */
static inline q16_15 q16_15_rsqrt(q16_15 x) {
    if (x <= 0)
        return INT32_MAX;

    u32 ux = (u32)x;
    int msb = 31 - __builtin_clz(ux);
    int shift = Q16_15_SHIFT - 1 - msb;
    // normalize x to [0.5,1.0[
    q16_15 m = shift >= 0 ? x << shift : x >> -shift;
    // initial approximation: y ≃ 1.912 - 0.912m
    q16_15 y = q16_15_from_float(1.912f) - q16_15_mul(q16_15_from_float(0.912f), m);
    // 2 iterations of Newton-Raphson
    for (int i = 0; i < 2; i++) {
        q16_15 yy = q16_15_mul(y, y);
        q16_15 m_yy = q16_15_mul(m, yy);

        y = q16_15_mul(y, (3 * Q16_15_ONE - m_yy));
        y >>= 1;
    }
    // undo normalization
    y = shift >= 0 ? y << (shift / 2) : y >> (-shift / 2);
    // Handle odd powers of two.
    if (shift & 1)
        y = q16_15_mul(y, shift > 0 ? Q16_15_SQRT_2 : Q16_15_RSQRT_2);

    return y;
}

/**
 * @brief Compute the square root in Q16.15 format.
 *
 * @param x Input value.
 * @return Approximation of sqrt(x).
 */
static inline q16_15 q16_15_sqrt(q16_15 x) {
    return q16_15_mul(x, q16_15_rsqrt(x));
}

#endif /* ifndef INCLUDE_FIXED_Q0_15_H */
