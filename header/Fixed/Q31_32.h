#ifndef INCLUDE_FIXED_Q31_32_H
#define INCLUDE_FIXED_Q31_32_H

#include <gint/defs/types.h>

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

/* ************************************************************************** */
/* ****************************** CONVERSIONS ******************************* */
/* ************************************************************************** */

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

/* ************************************************************************** */
/* ******************************* ARITHMETIC ******************************* */
/* ************************************************************************** */

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
    i32 ah = (i32)(a >> 32);
    u32 al = (u32)a;
    i32 bh = (i32)(b >> 32);
    u32 bl = (u32)b;

    i64 p0 = (i64)(( (u64)al * bl ) >> 32);
    i64 p1 = (i64)al * bh;
    i64 p2 = (i64)ah * bl;
    i64 p3 = ((i64)ah * bh) << 32;

    return p0 + p1 + p2 + p3;
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
    u64 ux = (u64)x;
    u64 mask = -(ux >> 63);
    ux = (ux ^ mask) - mask;

    int highest = 63 - __builtin_clzll(ux);
    int shift = 31 - highest;

    u64 mant = (shift >= 0 ? ux << shift : ux >> -shift);
    int index = ((mant >> 23) & 0xff);
    q31_32 r = q31_32_reciprocal_lut[index];

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
    // r = q31_32_mul(r, Q31_32_TWO - q31_32_mul(x, r));

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
    0x100000000, 0xfe8357a6, 0xfd0d3ddb, 0xfb9d82fb, 0xfa33f941, 0xf8d074ae, 0xf772caf6, 0xf61ad367,
    0xf4c866d7,  0xf37b5f91, 0xf2339944, 0xf0f0f0f1, 0xefb344dc, 0xee7a747d, 0xed466074, 0xec16ea76,
    0xeaebf549,  0xe9c564b0, 0xe8a31d66, 0xe785050e, 0xe66b0230, 0xe554fc26, 0xe442db1c, 0xe3348803,
    0xe229ec88,  0xe122f30d, 0xe01f86a7, 0xdf1f930c, 0xde230497, 0xdd29c83d, 0xdc33cb85, 0xdb40fc86,
    0xda5149e1,  0xd964a2b9, 0xd87af6b1, 0xd79435e5, 0xd6b050e8, 0xd5cf38bf, 0xd4f0ded8, 0xd415350e,
    0xd33c2da1,  0xd265bb31, 0xd191d0bd, 0xd0c061a0, 0xcff1618b, 0xcf24c485, 0xce5a7ee7, 0xcd928558,
    0xcccccccd,  0xcc094a82, 0xcb47f3fe, 0xca88bf0b, 0xc9cba1b4, 0xc9109249, 0xc8578754, 0xc7a0779f,
    0xc6eb5a2b,  0xc6382635, 0xc586d330, 0xc4d758c2, 0xc429aec8, 0xc37dcd4e, 0xc2d3ac93, 0xc22b4502,
    0xc1848f35,  0xc0df83f4, 0xc03c1c2f, 0xbf9a5100, 0xbefa1bac, 0xbe5b759b, 0xbdbe5860, 0xbd22bdad,
    0xbc889f5e,  0xbbeff76d, 0xbb58bff9, 0xbac2f341, 0xba2e8ba3, 0xb99b839d, 0xb909d5cc, 0xb8797ce8,
    0xb7ea73ca,  0xb75cb561, 0xb6d03cbc, 0xb6450503, 0xb5bb0976, 0xb5324570, 0xb4aab464, 0xb42451db,
    0xb39f1978,  0xb31b06f1, 0xb2981616, 0xb21642c8, 0xb1958901, 0xb115e4cc, 0xb0975249, 0xb019cdac,
    0xaf9d533a,  0xaf21df4d, 0xaea76e4e, 0xae2dfcb9, 0xadb5871b, 0xad3e0a12, 0xacc7824b, 0xac51ec83,
    0xabdd4587,  0xab698a34, 0xaaf6b775, 0xaa84ca41, 0xaa13bfa0, 0xa9a394a8, 0xa934467a, 0xa8c5d246,
    0xa8583548,  0xa7eb6cc8, 0xa77f761c, 0xa7144ea4, 0xa6a9f3cd, 0xa640630f, 0xa5d799ec, 0xa56f95f4,
    0xa50854bd,  0xa4a1d3ed, 0xa43c1130, 0xa3d70a3d, 0xa372bcd7, 0xa30f26c6, 0xa2ac45e0, 0xa24a1802,
    0xa1e89b12,  0xa187cd00, 0xa127abc2, 0xa0c83559, 0xa06967ce, 0xa00b4130, 0x9fadbf98, 0x9f50e127,
    0x9ef4a404,  0x9e99065e, 0x9e3e066b, 0x9de3a269, 0x9d89d89e, 0x9d30a753, 0x9cd80cdc, 0x9c800791,
    0x9c2895d1,  0x9bd1b602, 0x9b7b6691, 0x9b25a5ee, 0x9ad07291, 0x9a7bcaf9, 0x9a27ada8, 0x99d41929,
    0x99810c09,  0x992e84dd, 0x98dc823e, 0x988b02cb, 0x983a0528, 0x97e987fc, 0x979989f7, 0x974a09cb,
    0x96fb062f,  0x96ac7ddf, 0x965e6f9c, 0x9610da2b, 0x95c3bc55, 0x957714e9, 0x952ae2b8, 0x94df2499,
    0x9493d967,  0x9448ffff, 0x93fe9745, 0x93b49e1f, 0x936b1377, 0x9321f63b, 0x92d9455d, 0x9290ffd1,
    0x92492492,  0x9201b29c, 0x91baa8ed, 0x9174068a, 0x912dca7a, 0x90e7f3c5, 0x90a2817a, 0x905d72a8,
    0x9018c663,  0x8fd47bc2, 0x8f9091dd, 0x8f4d07d2, 0x8f09dcbf, 0x8ec70fc7, 0x8e84a010, 0x8e428cc0,
    0x8e00d502,  0x8dbf7804, 0x8d7e74f5, 0x8d3dcb09, 0x8cfd7973, 0x8cbd7f6d, 0x8c7ddc2e, 0x8c3e8ef5,
    0x8bff9700,  0x8bc0f391, 0x8b82a3ea, 0x8b44a752, 0x8b06fd11, 0x8ac9a471, 0x8a8c9cbf, 0x8a4fe549,
    0x8a137d60,  0x89d76458, 0x899b9984, 0x89601c3c, 0x8924ebda, 0x88ea07b7, 0x88af6f30, 0x887521a5,
    0x883b1e76,  0x88016505, 0x87c7f4b8, 0x878eccf3, 0x8755ed1e, 0x871d54a4, 0x86e502ef, 0x86acf76d,
    0x8675318c,  0x863db0bc, 0x8606746f, 0x85cf7c1a, 0x8598c730, 0x85625529, 0x852c257c, 0x84f637a4,
    0x84c08b1b,  0x848b1f5e, 0x8455f3eb, 0x84210842, 0x83ec5be3, 0x83b7ee51, 0x8383bf0f, 0x834fcda2,
    0x831c1990,  0x82e8a261, 0x82b5679f, 0x828268d2, 0x824fa586, 0x821d1d49, 0x81eacfa7, 0x81b8bc31,
    0x8186e275,  0x81554206, 0x8123da76, 0x80f2ab59, 0x80c1b443, 0x8090f4cb, 0x80606c88, 0x80301b11
};

static inline q31_32 q31_32_rsqrt_seed(q31_32 x) {
    int highest = 63 - __builtin_clzll((u64)x);
    int shift = (highest - 32) & ~1;

    q31_32 normalized = shift >= 0 ? x >> shift : x << -shift;

    u64 index = (((u64)normalized - Q31_32_ONE)) / (3ull << 24);
    index = index > 255 ? 255 : index;

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
    if (x <= 0)
        return INT64_MAX;

    q31_32 r = q31_32_rsqrt_seed(x);

    q31_32 rr = q31_32_mul(r, r);
    q31_32 xrr = q31_32_mul(x, rr);
    r = q31_32_mul(r, (3LL << 31) - (xrr >> 1));

    rr = q31_32_mul(r, r);
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

#endif /* ifndef INCLUDE_FIXED_Q31_32_H */
