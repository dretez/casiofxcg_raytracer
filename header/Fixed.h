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

/* ********************************* Q31.32 ********************************* */

/** Signed Q31.32 fixed-point value. */
typedef i64 q31_32;

/** Number of fractional bits in the Q31.32 format. */
#define Q31_32_SHIFT 32
/** The approximate representation of 1 in Q31.32 format. */
#define Q31_32_ONE ((q31_32)0x100000000ll)
/** The approximate representation of 2 in Q31.32 format. */
#define Q31_32_TWO ((q31_32)0x200000000ll)
/** The approximate representation of 0.5 in Q31.32 format.√ */
#define Q31_32_HALF ((q31_32)0x80000000ll)
/** The approximate representation of √2 in Q31.32 format. */
#define Q31_32_SQRT_2 ((q31_32)0x6074001000)
/** The approximate representation of 1/√2 in Q31.32 format. */
#define Q31_32_RSQRT_2 ((q31_32)0x3037000500)

/** Minimum representable value in Q0.15 format (-1). */
#define Q31_32_MIN (INT64_MIN)
/** Maximum representable value in Q0.15 format (1). */
#define Q31_32_MAX (INT64_MAX)

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

/* ********************************* Q31.32 ********************************* */

/**
 * Convert a floating-point value to Q31.32.
 *
 * @param f Floating-point value.
 * @return Approximate Q31.32 value.
 */
#define Q31_32_FROM_FLOAT(f) (q31_32)((f) * (float)Q31_32_ONE + ((f) >= 0.0f ? 0.5f : -0.5f))

/**
 * Convert a floating-point value to Q31.32.
 *
 * @param x Floating-point value.
 * @return Approximate Q31.32 value.
 */
static inline q31_32 q31_32_from_float(float x) {
    return Q31_32_FROM_FLOAT(x);
}

/**
 * Convert a Q31.32 value to floating point.
 *
 * @param x Fixed-point value.
 * @return Floating-point representation.
 */
static inline float q31_32_to_float(q31_32 x) {
    return (float)x / (float)Q31_32_ONE;
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

/* ********************************* Q31.32 ********************************* */

/**
 * Add two Q31.32 values.
 *
 * The result is saturated to the representable Q31.32 range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated sum.
 */
static inline q31_32 q31_32_add(q31_32 a, q31_32 b) {
    return b > 0 && a > INT64_MAX - b ? INT64_MAX : b < 0 && a < INT64_MIN - b ? INT64_MIN : a + b;
}

/**
 * Subtract two Q31.32 values.
 *
 * The result is saturated to the representable range.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Saturated difference.
 */
static inline q31_32 q31_32_sub(q31_32 a, q31_32 b) {
    return b < 0 && a > INT64_MAX + b ? INT64_MAX : b > 0 && a < INT64_MIN + b ? INT64_MIN : a - b;
}

/**
 * Multiply two Q31.32 values.
 *
 * @param a Left operand.
 * @param b Right operand.
 * @return Product in Q31.32 format.
 */
static inline q31_32 q31_32_mul(q31_32 a, q31_32 b) {
    bool neg = ((a ^ b) < 0);
    u64  ua  = (a < 0) ? (~(u64)a + 1) : (u64)a;
    u64  ub  = (b < 0) ? (~(u64)b + 1) : (u64)b;

    u64 al = (u32)ua;
    u64 ah = ua >> 32;
    u64 bl = (u32)ub;
    u64 bh = ub >> 32;

    u64 p0 = (al * bl) >> 32;
    u64 p1 = al * bh;
    u64 p2 = ah * bl;
    u64 p3 = (ah * bh) << 32;

    u64 result = p0 + p1 + p2 + p3;

    return neg ? -(q31_32)result : (q31_32)result;
}

static const q31_32 q31_32_reciprocal_lut[256] = {
    0x1ff007fc0, 0x1fd04794a, 0x1fb0c610d, 0x1f9182b68, 0x1f727cce5, 0x1f53b3a3f, 0x1f3526859,
    0x1f16d4c44, 0x1ef8bdb38, 0x1edae0a9b, 0x1ebd3cff8, 0x1e9fd2104, 0x1e829f39a, 0x1e65a3dbe,
    0x1e48df596, 0x1e2c51171, 0x1e0ff87c0, 0x1df3d4f17, 0x1dd7e5e31, 0x1dbc2abe7, 0x1da0a2f38,
    0x1d854df40, 0x1d6a2b33e, 0x1d4f3a293, 0x1d347a4bc, 0x1d19eb155, 0x1cff8c01c, 0x1ce55c8ea,
    0x1ccb5c3b6, 0x1cb18a893, 0x1c97e6fb1, 0x1c7e7115d, 0x1c65285fd, 0x1c4c0c614, 0x1c331ca3e,
    0x1c1a58b32, 0x1c01c01c0, 0x1be9526d0, 0x1bd10f365, 0x1bb8f6098, 0x1ba10679b, 0x1b89401b8,
    0x1b71a284e, 0x1b5a2d4d5, 0x1b42e00da, 0x1b2bba5ff, 0x1b14bbdfd, 0x1afde42a2, 0x1ae732dd1,
    0x1ad0a7981, 0x1aba41fbd, 0x1aa401aa4, 0x1a8de6468, 0x1a77ef750, 0x1a621cdb4, 0x1a4c6e200,
    0x1a36e2eb1, 0x1a217ae57, 0x1a0c35b92, 0x19f713117, 0x19e2129a7, 0x19cd34019, 0x19b876f52,
    0x19a3db247, 0x198f603fe, 0x197b05f8d, 0x1966cc019, 0x1952b20d7, 0x193eb7d0a, 0x192add006,
    0x19172152b, 0x1903847ea, 0x18f0063c0, 0x18dca6439, 0x18c9644f0, 0x18b64018b, 0x18a3395c0,
    0x18904fd50, 0x187d8340a, 0x186ad35cb, 0x18583fe7a, 0x1845c8a0c, 0x18336d483, 0x18212d9eb,
    0x180f0965d, 0x17fd005ff, 0x17eb124ff, 0x17d93ef9a, 0x17c786217, 0x17b5e78c6, 0x17a463005,
    0x1792f843c, 0x1781a71dc, 0x17706f561, 0x175f50b52, 0x174e4b040, 0x173d5e0c5, 0x172c89987,
    0x171bcd732, 0x170b29680, 0x16fa9d432, 0x16ea28d11, 0x16d9cbdf2, 0x16c9863b1, 0x16b957b34,
    0x16a94016a, 0x16993f349, 0x168954dd2, 0x167980e0b, 0x1669c3107, 0x165a1b3dd, 0x164a893ad,
    0x163b0cda2, 0x162ba5eea, 0x161c544c0, 0x160d17c61, 0x15fdf0317, 0x15eedd630, 0x15dfdf303,
    0x15d0f56ec, 0x15c21ff51, 0x15b35e99f, 0x15a4b1346, 0x1596179c2, 0x158791a93, 0x15791f340,
    0x156ac0156, 0x155c7426b, 0x154e3b419, 0x154015401, 0x153201fcb, 0x152401524, 0x1516131c0,
    0x150837359, 0x14fa6d7ae, 0x14ecb5c86, 0x14df0ffac, 0x14d17bef1, 0x14c3f982c, 0x14b688939,
    0x14a928ffa, 0x149bdaa58, 0x148e9d63e, 0x14817119f, 0x147455a72, 0x14674aeb4, 0x145a50c67,
    0x144d67190, 0x14408dc3e, 0x1433c4a7e, 0x14270ba69, 0x141a62a17, 0x140dc97a8, 0x140140140,
    0x13f4c6507, 0x13e85c12a, 0x13dc013dc, 0x13cfb5b51, 0x13c3795c5, 0x13b74c176, 0x13ab2dca8,
    0x139f1e5a2, 0x13931daaf, 0x13872ba20, 0x137b48248, 0x136f7317f, 0x1363ac622, 0x1357f3e90,
    0x134c4992d, 0x1340ad461, 0x13351ee97, 0x13299e640, 0x131e2b9cd, 0x1312c67b6, 0x13076ee75,
    0x12fc24c88, 0x12f0e8071, 0x12e5b88b5, 0x12da963dd, 0x12cf81075, 0x12c478d0c, 0x12b97d835,
    0x12ae8f087, 0x12a3ad49a, 0x1298d830d, 0x128e0fa7d, 0x128353990, 0x1278a3eea, 0x126e00937,
    0x126369721, 0x1258de758, 0x124e5f890, 0x1243ec97d, 0x1239858d8, 0x122f2a55c, 0x1224dadc9,
    0x121a970dd, 0x12105ed5f, 0x120632213, 0x11fc10dc4, 0x11f1faf3f, 0x11e7f0550, 0x11ddf0ecb,
    0x11d3fca84, 0x11ca13750, 0x11c035409, 0x11b661f8c, 0x11ac998b7, 0x11a2dbe6a, 0x119928f89,
    0x118f80af9, 0x1185e2fa4, 0x117c4fc72, 0x1172c7052, 0x116948a33, 0x115fd4906, 0x11566abc0,
    0x114d0b155, 0x1143b58c0, 0x113a6a0f9, 0x1131288ff, 0x1127f0fd0, 0x111ec346e, 0x11159f5db,
    0x110c8531d, 0x110374b3b, 0x10fa6dd3f, 0x10f170834, 0x10e87cb29, 0x10df9252c, 0x10d6b154f,
    0x10cdd9aa6, 0x10c50b446, 0x10bc46146, 0x10b38a0c0, 0x10aad71ce, 0x10a22d38e, 0x10998c51f,
    0x1090f45a1, 0x108865436, 0x107fdf004, 0x10776182f, 0x106eecbe0, 0x106680a40, 0x105e1d27a,
    0x1055c23bb, 0x104d6fd32, 0x104525e0f, 0x103ce4584, 0x1034ab2c5, 0x102c7a505, 0x102451b7d,
    0x101c31565, 0x1014191f6, 0x100c0906c, 0x100401004,
};

static inline q31_32 q31_32_reciprocal_seed(q31_32 x) {
    u64 ux   = (u64)x;
    u64 mask = -(ux >> 63);
    ux       = (ux ^ mask) - mask;

    int highest = 63 - __builtin_clzll(ux);
    int shift   = 31 - highest;

    u64    mant  = (shift >= 0 ? ux << shift : ux >> -shift);
    int    index = ((mant >> 23) & 0xff);
    q31_32 r     = q31_32_reciprocal_lut[index];

    r = shift >= 0 ? r << shift : r >> -shift;

    return (r ^ mask) - mask;
}

/**
 * Compute the reciprocal (1 / x) in Q31.32 format.
 *
 * @warning Division by zero is undefined.
 *
 * @param x Input value.
 * @return Reciprocal of @p x.
 */
static inline q31_32 q31_32_reciprocal(q31_32 x) {
    q31_32 r = q31_32_reciprocal_seed(x);

    r = q31_32_mul(r, Q31_32_TWO - q31_32_mul(x, r));
    r = q31_32_mul(r, Q31_32_TWO - q31_32_mul(x, r));

    return r;
}

/**
 * Divide one Q31.32 value by another.
 *
 * @warning Division by zero is undefined.
 *
 * @param a Dividend.
 * @param b Divisor.
 * @return Quotient.
 */
static inline q31_32 q31_32_div(q31_32 a, q31_32 b) {
    return q31_32_mul(a, q31_32_reciprocal(b));
}

static const q31_32 q31_32_rsqrt_lut[256] = {
    0x100000000, 0xff017d84, 0xfe05ec45, 0xfd0d3ddb, 0xfc176441, 0xfb2451d2, 0xfa33f941, 0xf9464d9c,
    0xf85b4247,  0xf772caf6, 0xf68cdbaf, 0xf5a968c6, 0xf4c866d7, 0xf3e9cac9, 0xf30d89c8, 0xf2339944,
    0xf15beef0,  0xf08680bd, 0xefb344dc, 0xeee231b7, 0xee133df5, 0xed466074, 0xec7b9047, 0xebb2c4ba,
    0xeaebf549,  0xea2719a3, 0xe96429a7, 0xe8a31d66, 0xe7e3ed19, 0xe726912b, 0xe66b0230, 0xe5b138e4,
    0xe4f92e2e,  0xe442db1c, 0xe38e38e4, 0xe2db40dd, 0xe229ec88, 0xe17a3584, 0xe0cc1597, 0xe01f86a7,
    0xdf7482b8,  0xdecb03f1, 0xde230497, 0xdd7c7f0d, 0xdcd76dd3, 0xdc33cb85, 0xdb9192dc, 0xdaf0beab,
    0xda5149e1,  0xd9b32f85, 0xd9166ab7, 0xd87af6b1, 0xd7e0cec3, 0xd747ee56, 0xd6b050e8, 0xd619f20f,
    0xd584cd74,  0xd4f0ded8, 0xd45e220d, 0xd3cc92fc, 0xd33c2da1, 0xd2acee09, 0xd21ed057, 0xd191d0bd,
    0xd105eb80,  0xd07b1cf8, 0xcff1618b, 0xcf68b5b1, 0xcee115f2, 0xce5a7ee7, 0xcdd4ed36, 0xcd505d96,
    0xcccccccd,  0xcc4a37ad, 0xcbc89b18, 0xcb47f3fe, 0xcac83f5c, 0xca497a3c, 0xc9cba1b4, 0xc94eb2e9,
    0xc8d2ab0b,  0xc8578754, 0xc7dd450e, 0xc763e18b, 0xc6eb5a2b, 0xc673ac57, 0xc5fcd583, 0xc586d330,
    0xc511a2e6,  0xc49d423a, 0xc429aec8, 0xc3b6e63a, 0xc344e63f, 0xc2d3ac93, 0xc26336f8, 0xc1f3833d,
    0xc1848f35,  0xc11658c0, 0xc0a8ddc3, 0xc03c1c2f, 0xbfd011f9, 0xbf64bd20, 0xbefa1bac, 0xbe902baa,
    0xbe26eb32,  0xbdbe5860, 0xbd567158, 0xbcef3446, 0xbc889f5e, 0xbc22b0d8, 0xbbbd66f4, 0xbb58bff9,
    0xbaf4ba35,  0xba9153fa, 0xba2e8ba3, 0xb9cc5f8f, 0xb96ace23, 0xb909d5cc, 0xb8a974fa, 0xb849aa25,
    0xb7ea73ca,  0xb78bd069, 0xb72dbe8b, 0xb6d03cbc, 0xb673498f, 0xb616e399, 0xb5bb0976, 0xb55fb9c8,
    0xb504f334,  0xb4aab464, 0xb450fc07, 0xb3f7c8d0, 0xb39f1978, 0xb346ecba, 0xb2ef4158, 0xb2981616,
    0xb24169bd,  0xb1eb3b1b, 0xb1958901, 0xb1405244, 0xb0eb95bd, 0xb0975249, 0xb04386c9, 0xaff03221,
    0xaf9d533a,  0xaf4ae8ff, 0xaef8f25f, 0xaea76e4e, 0xae565bc0, 0xae05b9b0, 0xadb5871b, 0xad65c300,
    0xad166c63,  0xacc7824b, 0xac7903bf, 0xac2aefcf, 0xabdd4587, 0xab9003fd, 0xab432a44, 0xaaf6b775,
    0xaaaaaaab,  0xaa5f0304, 0xaa13bfa0, 0xa9c8dfa4, 0xa97e6234, 0xa934467a, 0xa8ea8ba1, 0xa8a130d5,
    0xa8583548,  0xa80f982c, 0xa7c758b5, 0xa77f761c, 0xa737ef9a, 0xa6f0c46b, 0xa6a9f3cd, 0xa6637d01,
    0xa61d5f4a,  0xa5d799ec, 0xa5922c2f, 0xa54d155c, 0xa50854bd, 0xa4c3e9a2, 0xa47fd357, 0xa43c1130,
    0xa3f8a27f,  0xa3b58699, 0xa372bcd7, 0xa330448f, 0xa2ee1d1e, 0xa2ac45e0, 0xa26abe34, 0xa2298579,
    0xa1e89b12,  0xa1a7fe63, 0xa167aed0, 0xa127abc2, 0xa0e7f4a0, 0xa0a888d5, 0xa06967ce, 0xa02a90f7,
    0x9fec03bf,  0x9fadbf98, 0x9f6fc3f4, 0x9f321046, 0x9ef4a404, 0x9eb77ea3, 0x9e7a9f9d, 0x9e3e066b,
    0x9e01b287,  0x9dc5a36e, 0x9d89d89e, 0x9d4e5195, 0x9d130dd3, 0x9cd80cdc, 0x9c9d4e30, 0x9c62d156,
    0x9c2895d1,  0x9bee9b29, 0x9bb4e0e6, 0x9b7b6691, 0x9b422bb3, 0x9b092fda, 0x9ad07291, 0x9a97f366,
    0x9a5fb1e9,  0x9a27ada8, 0x99efe637, 0x99b85b26, 0x99810c09, 0x9949f875, 0x99131fff, 0x98dc823e,
    0x98a61ec9,  0x986ff539, 0x983a0528, 0x98044e2f, 0x97cecfeb, 0x979989f7, 0x97647bf3, 0x972fa57b,
    0x96fb062f,  0x96c69db0, 0x96926b9e, 0x965e6f9c, 0x962aa94c, 0x95f71853, 0x95c3bc55, 0x959094f7,
    0x955da1e0,  0x952ae2b8, 0x94f85725, 0x94c5fed2, 0x9493d967, 0x9461e68f, 0x943025f5, 0x93fe9745
};

static inline q31_32 q31_32_rsqrt_seed(q31_32 x) {
    int highest = 63 - __builtin_clzll((u64)x);
    int shift   = highest & ~1;

    q31_32 normalized = x >> shift;

    u64 index = ((u64)normalized >> 24) & 0xff;

    q31_32 r = q31_32_rsqrt_lut[index];

    int scale = shift / 2;

    return scale >= 0 ? r >> scale : r << -scale;
}

/**
 * Compute the inverse square root in Q31.32 format.
 *
 * Uses a linear initial approximation followed by two Newton-Raphson refinement iterations.
 *
 * For non-positive inputs, the function returns the maximum representable value.
 *
 * @param x Positive fixed-point value.
 * @return Approximation of 1/sqrt(x).
 */
static inline q31_32 q31_32_rsqrt(q31_32 x) {
    if (x <= 0) return INT32_MAX;

    q31_32 r = q31_32_rsqrt_seed(x);

    q31_32 rr  = q31_32_mul(r, r);
    q31_32 xrr = q31_32_mul(x, rr);
    r = q31_32_mul(r, (3LL << 31) - (xrr >> 1));

    rr  = q31_32_mul(r, r);
    xrr = q31_32_mul(x, rr);
    r = q31_32_mul(r, (3LL << 31) - (xrr >> 1));

    return r;
}

/**
 * @brief Compute the square root in Q31.32 format.
 *
 * @param x Input value.
 * @return Approximation of sqrt(x).
 */
static inline q31_32 q31_32_sqrt(q31_32 x) {
    return q31_32_mul(x, q31_32_rsqrt(x));
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
