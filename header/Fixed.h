#ifndef INCLUDE_FIXED_H
#define INCLUDE_FIXED_H

#include <gint/defs/types.h>
#include <stdint.h>

/* ************************************************************************** */
/* ********************************* TYPES ********************************** */
/* ************************************************************************** */

typedef i16 q0_15;
typedef i32 q16_15;

typedef u16 uq0_16;
typedef u32 uq16_16;

/* ********************************* Q0.15 ********************************** */

#define Q0_15_SHIFT 15
#define Q0_15_ONE ((q0_15)((1 << Q0_15_SHIFT) - 1))
#define Q0_15_HALF ((q0_15)(1 << (Q0_15_SHIFT - 1)))

#define Q0_15_MIN ((q0_15)(1 << Q0_15_SHIFT))
#define Q0_15_MAX ((q0_15)Q0_15_ONE)

#define Q0_15_FROM_FLOAT(f)                                                                        \
    ((((f) <= -1.0f)  ? Q0_15_MIN                                                                  \
      : ((f) >= 1.0f) ? Q0_15_MAX                                                                  \
                      : (q0_15)((f) * (float)Q0_15_ONE + ((f) >= 0.0f ? 0.5f : -0.5f))))

static inline q0_15 q0_15_from_float(float x) {
    return Q0_15_FROM_FLOAT(x);
}

static inline float q0_15_to_float(q0_15 x) {
    return (float)x / (float)Q0_15_ONE;
}

static inline q0_15 q0_15_add(q0_15 a, q0_15 b) {
    i32 c = (i32)a + (i32)b;
    return c < Q0_15_MIN ? Q0_15_MIN : c > Q0_15_MAX ? Q0_15_MAX : (q0_15)c;
}

static inline q0_15 q0_15_sub(q0_15 a, q0_15 b) {
    i32 c = (i32)a - (i32)b;
    return c < Q0_15_MIN ? Q0_15_MIN : c > Q0_15_MAX ? Q0_15_MAX : (q0_15)c;
}

static inline q0_15 q0_15_mul(q0_15 a, q0_15 b) {
    return (q0_15)(((i32)a * (i32)b) >> Q0_15_SHIFT);
}

static inline q0_15 q0_15_div(q0_15 a, q0_15 b) {
    return (q0_15)(((i32)a << Q0_15_SHIFT) / b);
}

/* ********************************* Q16.15 ********************************* */

#define Q16_15_SHIFT 15
#define Q16_15_ONE ((q16_15)(1 << Q16_15_SHIFT))
#define Q16_15_HALF ((q16_15)(1 << (Q16_15_SHIFT - 1)))
#define Q16_15_SQRT_2 0xb505
#define Q16_15_RSQRT_2 0x5a82

#define Q16_15_FROM_FLOAT(f) (q16_15)((f) * (float)Q16_15_ONE + ((f) >= 0.0f ? 0.5f : -0.5f))

static inline q16_15 q16_15_from_float(float x) {
    return Q16_15_FROM_FLOAT(x);
}

static inline float q16_15_to_float(q16_15 x) {
    return (float)x / (float)Q16_15_ONE;
}

static inline q0_15 q16_15_to_q0_15(q16_15 x) {
    return x > Q0_15_ONE ? Q0_15_ONE : (uq0_16)x;
}

static inline q16_15 q16_15_add(q16_15 a, q16_15 b) {
    i64 c = (i64)a + (i64)b;
    return c > INT32_MAX ? INT32_MAX : c < INT32_MIN ? INT32_MIN : (q16_15)c;
}

static inline q16_15 q16_15_sub(q16_15 a, q16_15 b) {
    i64 c = (i64)a - (i64)b;
    return c > INT32_MAX ? INT32_MAX : c < INT32_MIN ? INT32_MIN : (q16_15)c;
}

static inline q16_15 q16_15_mul(q16_15 a, q16_15 b) {
    return (q16_15)(((i64)a * (i64)b) >> Q16_15_SHIFT);
}

static inline q16_15 q16_15_div(q16_15 a, q16_15 b) {
    return (q16_15)(((i64)a << Q16_15_SHIFT) / b);
}

static inline q16_15 q16_15_reciprocal(q16_15 x) {
    return (q16_15)(((i64)Q16_15_ONE << Q16_15_SHIFT) / x);
}

// PERF: implement lookup table
static inline q16_15 q16_15_rsqrt(q16_15 x) {
    if (x <= 0) return INT32_MAX;

    u32 ux = (u32)x;
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

static inline q16_15 q16_15_sqrt(q16_15 x) {
    return q16_15_mul(x, q16_15_rsqrt(x));
}

/* ********************************* UQ0.16 ********************************* */

#define UQ0_16_SHIFT 16
#define UQ0_16_ONE ((uq0_16)((1u << UQ0_16_SHIFT) - 1u))
#define UQ0_16_HALF ((uq0_16)(1u << (UQ0_16_SHIFT - 1)))

#define UQ0_16_FROM_FLOAT(f)                                                                       \
    ((uq0_16)((f) <= 0.0f ? 0 : (f) >= 1.0f ? UQ0_16_ONE : ((f) * (float)UQ0_16_ONE + 0.5f)))

static inline uq0_16 uq0_16_from_float(float x) {
    return UQ0_16_FROM_FLOAT(x);
}

static inline float uq0_16_to_float(uq0_16 x) {
    return (float)x / (float)UQ0_16_ONE;
}

static inline uq0_16 uq0_16_add(uq0_16 a, uq0_16 b) {
    u32 c = (u32)a + (u32)b;
    return c > UQ0_16_ONE ? UQ0_16_ONE : (uq0_16)c;
}

static inline uq0_16 uq0_16_sub(uq0_16 a, uq0_16 b) {
    return a < b ? 0 : a - b;
}

static inline uq0_16 uq0_16_mul(uq0_16 a, uq0_16 b) {
    return (uq0_16)(((u32)a * (u32)b + UQ0_16_HALF) >> UQ0_16_SHIFT);
}

static inline uq0_16 uq0_16_div(uq0_16 a, uq0_16 b) {
    return (uq0_16)(((u32)a << UQ0_16_SHIFT) / b);
}

/* *******************************  UQ16.16  ******************************** */

#define UQ16_16_SHIFT 16
#define UQ16_16_ONE ((uq16_16)(1u << UQ16_16_SHIFT))
#define UQ16_16_HALF ((uq16_16)(1u << (UQ16_16_SHIFT - 1)))

#define UQ16_16_SQRT_2 0x16a0a
#define UQ16_16_RSQRT_2 0xb505

#define UQ16_16_FROM_FLOAT(f) ((uq16_16)((f) <= 0.0f ? 0 : (f) * (float)UQ16_16_ONE + 0.5f))

static inline uq16_16 uq16_16_from_float(float x) {
    return UQ16_16_FROM_FLOAT(x);
}

static inline float uq16_16_to_float(uq16_16 x) {
    return (float)x / (float)UQ16_16_ONE;
}

static inline uq0_16 uq16_16_to_uq0_16(uq16_16 x) {
    return x > UQ0_16_ONE ? UQ0_16_ONE : (uq0_16)x;
}

static inline uq16_16 uq16_16_add(uq16_16 a, uq16_16 b) {
    u64 c = ((u64)a + (u64)b);
    return c > UINT32_MAX ? UINT32_MAX : (uq16_16)c;
}

static inline uq16_16 uq16_16_sub(uq16_16 a, uq16_16 b) {
    return a < b ? 0 : a - b;
}

static inline uq16_16 uq16_16_mul(uq16_16 a, uq16_16 b) {
    return (uq16_16)(((u64)a * (u64)b + UQ16_16_HALF) >> UQ16_16_SHIFT);
}

static inline uq16_16 uq16_16_div(uq16_16 a, uq16_16 b) {
    return (uq16_16)(((u64)a << UQ16_16_SHIFT) / b);
}

static inline uq16_16 uq16_16_reciprocal(uq16_16 x) {
    return (uq16_16)(((u64)UQ16_16_ONE << UQ16_16_SHIFT) / x);
}

// PERF: implement lookup table
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

static inline uq16_16 uq16_16_sqrt(uq16_16 x) {
    return uq16_16_mul(x, uq16_16_rsqrt(x));
}

#endif /* ifndef INCLUDE_FIXED_H */
