#ifndef INCLUDE_FIXED_Q0_15_H
#define INCLUDE_FIXED_Q0_15_H

#include <gint/defs/types.h>

/** Signed Q0.15 fixed-point value. */
typedef i16 q0_15;

/** Number of fractional bits in the Q0.15 format. */
#define Q0_15_SHIFT 15
/** The approximate representation of 1 in Q0.15 format. */
#define Q0_15_ONE ((q0_15)((1 << Q0_15_SHIFT) - 1))
/** The approximate representation of 0.5 in Q0.15 format. */
#define Q0_15_HALF ((q0_15)(1 << (Q0_15_SHIFT - 1)))

/** Minimum representable value in Q0.15 format (-1). */
#define Q0_15_MIN ((q0_15)(1 << Q0_15_SHIFT))
/** Maximum representable value in Q0.15 format (1). */
#define Q0_15_MAX ((q0_15)Q0_15_ONE)

/* ************************************************************************** */
/* ****************************** CONVERSIONS ******************************* */
/* ************************************************************************** */

/**
 * Convert a floating-point value to Q0.15.
 *
 * @param f Floating-point value.
 * @return Approximate Q0.15 value.
 */
#define Q0_15_FROM_FLOAT(f)                                                                        \
    ((((f) <= -1.0f)  ? Q0_15_MIN                                                                  \
      : ((f) >= 1.0f) ? Q0_15_MAX                                                                  \
                      : (q0_15)((f) * (float)Q0_15_ONE + ((f) >= 0.0f ? 0.5f : -0.5f))))

/**
 * Convert a floating-point value to Q0.15.
 *
 * @param x Floating-point value.
 * @return Approximate Q0.15 value.
 */
static inline q0_15 q0_15_from_float(float x) {
    return Q0_15_FROM_FLOAT(x);
}

/**
 * Convert a floating-point value to Q0.15.
 *
 * @warning This function assumes the provided float sits in the range [-1, 1] and will wrap with
 *          values outside this range. When unsure, use `q0_15_from_float()` instead.
 *
 * @param x Floating-point value.
 * @return Approximate Q0.15 value.
 */
static inline q0_15 q0_15_from_unitfloat(float x) {
    return (q0_15)(x * (float)Q0_15_ONE + 0.5);
}

/**
 * Convert a Q0.15 value to floating point.
 *
 * @param x Fixed-point value.
 * @return Floating-point representation.
 */
static inline float q0_15_to_float(q0_15 x) {
    return (float)x / (float)Q0_15_ONE;
}

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

/**
 * Add two Q0.15 values.
 *
 * The result is saturated to the representable Q0.15 range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated sum.
 */
static inline q0_15 q0_15_add(q0_15 a, q0_15 b) {
    i32 c = (i32)a + (i32)b;
    return c < Q0_15_MIN ? Q0_15_MIN : c > Q0_15_MAX ? Q0_15_MAX : (q0_15)c;
}

/**
 * Subtract two Q0.15 values.
 *
 * The result is saturated to the representable range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated difference.
 */
static inline q0_15 q0_15_sub(q0_15 a, q0_15 b) {
    i32 c = (i32)a - (i32)b;
    return c < Q0_15_MIN ? Q0_15_MIN : c > Q0_15_MAX ? Q0_15_MAX : (q0_15)c;
}

/**
 * Multiply two Q0.15 values.
 *
 * The product is rescaled back to Q0.15 by discarding the lower fractional bits.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Product in Q0.15 format.
 */
static inline q0_15 q0_15_mul(q0_15 a, q0_15 b) {
    return (q0_15)(((i32)a * (i32)b) >> Q0_15_SHIFT);
}

/**
 * Divide one Q0.15 value by another.
 *
 * The numerator is shifted before division to preserve the fractional precision.
 *
 * @warning Division by zero is undefined.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return Quotient.
 */
static inline q0_15 q0_15_div(q0_15 a, q0_15 b) {
    return (q0_15)(((i32)a << Q0_15_SHIFT) / b);
}

#endif /* ifndef INCLUDE_FIXED_Q0_15_H */
