#ifndef INCLUDE_FIXED_H
#define INCLUDE_FIXED_H

#include <gint/defs/types.h>

/* ************************************************************************** */
/* ********************************* TYPES ********************************** */
/* ************************************************************************** */

/* ********************************* Q0.15 ********************************** */

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

/* ********************************* Q16.15 ********************************* */

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

/* ********************************* UQ0.16 ********************************* */

/** Unsigned Q0.16 fixed-point value. */
typedef u16 uq0_16;

/** Number of fractional bits in the Q0.16 format. */
#define UQ0_16_SHIFT 16
/** The approximate representation of 1 in Q0.16 format. */
#define UQ0_16_ONE ((uq0_16)((1u << UQ0_16_SHIFT) - 1u))
/** The approximate representation of 0.5 in Q0.16 format. */
#define UQ0_16_HALF ((uq0_16)(1u << (UQ0_16_SHIFT - 1)))

/* *******************************  UQ16.16  ******************************** */

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

/* ********************************* Q0.15 ********************************** */

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

/* ********************************* Q16.15 ********************************* */

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
    return x > Q0_15_ONE ? Q0_15_ONE : (uq0_16)x;
}

/* ********************************* UQ0.16 ********************************* */

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

/* *******************************  UQ16.16  ******************************** */

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
    return x > UQ0_16_ONE ? UQ0_16_ONE : (uq0_16)x;
}

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

/* ********************************* Q0.15 ********************************** */

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

/* ********************************* Q16.15 ********************************* */

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
    if (x <= 0) return INT32_MAX;

    u32 ux    = (u32)x;
    int msb   = 31 - __builtin_clz(ux);
    int shift = Q16_15_SHIFT - 1 - msb;
    // normalize x to [0.5,1.0[
    q16_15 m = shift >= 0 ? x << shift : x >> -shift;
    // initial approximation: y ≃ 1.912 - 0.912m
    q16_15 y = q16_15_from_float(1.912f) - q16_15_mul(q16_15_from_float(0.912f), m);
    // 2 iterations of Newton-Raphson
    for (int i = 0; i < 2; i++) {
        q16_15 yy   = q16_15_mul(y, y);
        q16_15 m_yy = q16_15_mul(m, yy);

        y = q16_15_mul(y, (3 * Q16_15_ONE - m_yy));
        y >>= 1;
    }
    // undo normalization
    y = shift >= 0 ? y << (shift / 2) : y >> (-shift / 2);
    // Handle odd powers of two.
    if (shift & 1) y = q16_15_mul(y, shift > 0 ? Q16_15_SQRT_2 : Q16_15_RSQRT_2);

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

/* ********************************* UQ0.16 ********************************* */

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

/* *******************************  UQ16.16  ******************************** */

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
    if (x <= 0) return UINT32_MAX;
    int msb   = 31 - __builtin_clz(x);
    int shift = UQ16_16_SHIFT - 1 - msb;
    // normalize x to [0.5,1.0[
    uq16_16 m = shift >= 0 ? x << shift : x >> -shift;
    // initial approximation: y ≃ 1.912 - 0.912m
    uq16_16 y = uq16_16_from_float(1.912f) - uq16_16_mul(uq16_16_from_float(0.912f), m);
    // 2 iterations of Newton-Raphson
    for (int i = 0; i < 2; i++) {
        uq16_16 yy  = uq16_16_mul(y, y);
        uq16_16 myy = uq16_16_mul(m, yy);

        y = uq16_16_mul(y, (3 * UQ16_16_ONE - myy));
        y >>= 1;
    }
    // undo normalization
    y = shift >= 0 ? y << (shift / 2) : y >> (-shift / 2);
    // Handle odd powers of two.
    if (shift & 1) y = uq16_16_mul(y, shift > 0 ? UQ16_16_SQRT_2 : UQ16_16_RSQRT_2);

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

#endif /* ifndef INCLUDE_FIXED_H */
