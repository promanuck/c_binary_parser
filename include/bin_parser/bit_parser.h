/*
 * parser.h
 *
 *  Created on: Jan 28, 2024
 *      Author: pavel
 */

#ifndef INCLUDE_BIT_PARSER_PARSER_H_
#define INCLUDE_BIT_PARSER_PARSER_H_

#include <string.h>
#include <limits.h>
#include <stdint.h>

#define bin_parser_ntoh_8(x) x

#if defined(__BYTE_ORDER__)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#ifdef _MSC_VER

#include <stdlib.h>
#define bin_parser_ntoh_16(x) _byteswap_ushort(x)
#define bin_parser_ntoh_32(x) _byteswap_ulong(x)
#define bin_parser_ntoh_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define bin_parser_ntoh_16(x) OSSwapInt16(x)
#define bin_parser_ntoh_32(x) OSSwapInt32(x)
#define bin_parser_ntoh_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bin_parser_ntoh_16(x) BSWAP_16(x)
#define bin_parser_ntoh_32(x) BSWAP_32(x)
#define bin_parser_ntoh_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bin_parser_ntoh_16(x) bswap16(x)
#define bin_parser_ntoh_32(x) bswap32(x)
#define bin_parser_ntoh_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bin_parser_ntoh_16(x) swap16(x)
#define bin_parser_ntoh_32(x) swap32(x)
#define bin_parser_ntoh_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bin_parser_ntoh_32)
#define bin_parser_ntoh_16(x) bswap16(x)
#define bin_parser_ntoh_32(x) bswap32(x)
#define bin_parser_ntoh_64(x) bswap64(x)
#endif

#else

#include <byteswap.h>
#define bin_parser_ntoh_16(x) bswap_16(x)
#define bin_parser_ntoh_32(x) bswap_32(x)
#define bin_parser_ntoh_64(x) bswap_64(x)

#endif

#else

#define bin_parser_ntoh_16(x) x
#define bin_parser_ntoh_32(x) x
#define bin_parser_ntoh_64(x) x

#endif    // __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#else
#error __BYTE_ORDER__ SHOULD BE DEFINED TO __ORDER_BIG_ENDIAN__ or __ORDER_LITTLE_ENDIAN__
#endif    // DEFINED(__BYTE_ORDER__)

#ifdef BIT_PARSER_CHECK_ALL
#define BIT_PARSER_CHECK_BUFFER_BOUNDARY 1
#define BIT_PARSER_CHECK_PARAM 1
#endif

#ifndef BIT_PARSER_CHECK_BUFFER_BOUNDARY
#define BIT_PARSER_CHECK_BUFFER_BOUNDARY 0
#endif

#ifndef BIT_PARSER_CHECK_PARAM
#define BIT_PARSER_CHECK_PARAM 0
#endif

typedef uint32_t bit_parser_iter_type_t;

typedef enum bin_parser_status_tag {
    BIT_PARSER_OK = 0,
    BIT_PARSER_EFULL,
    BIT_PARSER_EINVALID_ARG
} bit_parser_status_t;

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

typedef struct bit_parser_tag {
    const char *buf;
    bit_parser_iter_type_t capacity;
    bit_parser_iter_type_t iter;
} bit_parser_t;

void bit_parser_init(bit_parser_t *inst, const char *buf, bit_parser_iter_type_t buf_len_bits);

bit_parser_status_t bit_parser_skip(bit_parser_t *inst, bit_parser_iter_type_t n_bits);

bit_parser_status_t bit_parser_extract_u8(bit_parser_t *inst, uint8_t *res, int n_bits);

bit_parser_status_t bit_parser_extract_u16(bit_parser_t *inst, uint16_t *res, int n_bits);

bit_parser_status_t bit_parser_extract_u32(bit_parser_t *inst, uint32_t *res, int n_bits);

bit_parser_status_t bit_parser_extract_u64(bit_parser_t *inst, uint64_t *res, int n_bits);

bit_parser_status_t bit_parser_extract_i8(bit_parser_t *inst, int8_t *res, int n_bits);

bit_parser_status_t bit_parser_extract_i16(bit_parser_t *inst, int16_t *res, int n_bits);

bit_parser_status_t bit_parser_extract_i32(bit_parser_t *inst, int32_t *res, int n_bits);

bit_parser_status_t bit_parser_extract_i64(bit_parser_t *inst, int64_t *res, int n_bits);

bit_parser_status_t bit_parser_extract_float(bit_parser_t *inst, float *res);

bit_parser_status_t bit_parser_extract_double(bit_parser_t *inst, double *res);

/*
 **************************************************************************************************
  Realization
 **************************************************************************************************
 */
#if BIT_PARSER_CHECK_BUFFER_BOUNDARY == 0
#define BIT_PARSER_CHECK_OVERFLOW(inst_, n_bits_)
#else
#define BIT_PARSER_CHECK_OVERFLOW(inst_, n_bits_)          \
    do {                                                   \
        if ((inst_)->capacity - (inst_)->iter < (n_bits_)) \
            return BIT_PARSER_EFULL;                       \
    } while (0)
#endif

#if BIT_PARSER_CHECK_PARAM == 0
#define BIT_PARSER_CHECK_PARAM_SIZE(inst_, n_bits_, type_)
#else
#define BIT_PARSER_CHECK_PARAM_SIZE(inst_, n_bits_, type_) \
    do {                                                   \
        if (n_bits_ > 8 * sizeof(type_))                   \
            return BIT_PARSER_EINVALID_ARG;                \
    } while (0)
#endif

#define BIT_PARSER_EXTRACT_INTEGER_(inst_, out_, out_type_, n_bits_, unsigned_prefix, bswap) \
    do {                                                                                     \
        int bits_in_type = sizeof(out_type_) * 8;                                            \
        out_type_ hi_bits = bits_in_type - (inst_->iter % bits_in_type);                     \
        int idx_low = inst_->iter / bits_in_type;                                            \
        if (hi_bits < n_bits_) {                                                             \
            unsigned_prefix##out_type_ high;                                                 \
            memcpy((void *)&high, (void *)&inst_->buf[idx_low], sizeof(high));               \
            high = bswap(high);                                                              \
            unsigned_prefix##out_type_ low;                                                  \
            memcpy((void *)&low, (void *)&inst_->buf[idx_low + sizeof(low)], sizeof(low));   \
            low = bswap(low);                                                                \
            low >>= hi_bits;                                                                 \
            low += high << (inst_->iter % bits_in_type);                                     \
            memcpy(out_, &low, sizeof(low));                                                 \
            *out_ >>= (bits_in_type - n_bits);                                               \
        }                                                                                    \
        else {                                                                               \
            unsigned_prefix##out_type_ low;                                                  \
            memcpy((void *)&low, (void *)&inst_->buf[idx_low], sizeof(low));                 \
            low = bswap(low);                                                                \
            low <<= (inst_->iter % bits_in_type);                                            \
            memcpy(out_, &low, sizeof(low));                                                 \
            *out_ >>= bits_in_type - n_bits;                                                 \
        }                                                                                    \
    } while (0)

#define BIT_PARSER_EXTRACT_UINT(inst_, out_, out_type_, n_bits_, bswap) \
    BIT_PARSER_EXTRACT_INTEGER_(inst_, out_, out_type_, n_bits, , bswap)

#define BIT_PARSER_EXTRACT_INT(inst_, out_, out_type_, n_bits_, bswap) \
    BIT_PARSER_EXTRACT_INTEGER_(inst_, out_, out_type_, n_bits, u, bswap)

extern inline void bit_parser_init(bit_parser_t *inst,
                                   const char *buf,
                                   bit_parser_iter_type_t buf_len_bits) {
    inst->buf = buf;
    inst->capacity = buf_len_bits;
    inst->iter = 0;
}

extern inline bit_parser_status_t bit_parser_skip(bit_parser_t *inst,
                                                  bit_parser_iter_type_t n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);

    inst->iter += n_bits;

    return BIT_PARSER_OK;
}

extern inline bit_parser_status_t bit_parser_extract_u8(bit_parser_t *inst,
                                                        uint8_t *res,
                                                        int n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIT_PARSER_CHECK_PARAM_SIZE(inst, n_bits, uint8_t);
    BIT_PARSER_EXTRACT_UINT(inst, res, uint8_t, n_bits, bin_parser_ntoh_8);
    inst->iter += n_bits;
    return BIT_PARSER_OK;
}

extern inline bit_parser_status_t bit_parser_extract_i8(bit_parser_t *inst,
                                                        int8_t *res,
                                                        int n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIT_PARSER_CHECK_PARAM_SIZE(inst, n_bits, int8_t);
    BIT_PARSER_EXTRACT_INT(inst, res, int8_t, n_bits, bin_parser_ntoh_8);
    inst->iter += n_bits;
    return BIT_PARSER_OK;
}

extern inline bit_parser_status_t bit_parser_extract_u16(bit_parser_t *inst,
                                                         uint16_t *res,
                                                         int n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIT_PARSER_CHECK_PARAM_SIZE(inst, n_bits, uint16_t);
    BIT_PARSER_EXTRACT_UINT(inst, res, uint16_t, n_bits, bin_parser_ntoh_16);
    inst->iter += n_bits;

    return BIT_PARSER_OK;
}

extern inline bit_parser_status_t bit_parser_extract_i16(bit_parser_t *inst,
                                                         int16_t *res,
                                                         int n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIT_PARSER_CHECK_PARAM_SIZE(inst, n_bits, int16_t);
    BIT_PARSER_EXTRACT_INT(inst, res, int16_t, n_bits, bin_parser_ntoh_16);
    inst->iter += n_bits;

    return BIT_PARSER_OK;
}

extern inline bit_parser_status_t bit_parser_extract_u32(bit_parser_t *inst,
                                                         uint32_t *res,
                                                         int n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIT_PARSER_CHECK_PARAM_SIZE(inst, n_bits, uint32_t);
    BIT_PARSER_EXTRACT_UINT(inst, res, uint32_t, n_bits, bin_parser_ntoh_32);
    inst->iter += n_bits;

    return BIT_PARSER_OK;
}

extern inline bit_parser_status_t bit_parser_extract_i32(bit_parser_t *inst,
                                                         int32_t *res,
                                                         int n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIT_PARSER_CHECK_PARAM_SIZE(inst, n_bits, int32_t);
    BIT_PARSER_EXTRACT_INT(inst, res, int32_t, n_bits, bin_parser_ntoh_32);
    inst->iter += n_bits;

    return BIT_PARSER_OK;
}

extern inline bit_parser_status_t bit_parser_extract_u64(bit_parser_t *inst,
                                                         uint64_t *res,
                                                         int n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIT_PARSER_CHECK_PARAM_SIZE(inst, n_bits, uint64_t);
    BIT_PARSER_EXTRACT_UINT(inst, res, uint64_t, n_bits, bin_parser_ntoh_64);
    inst->iter += n_bits;

    return BIT_PARSER_OK;
}

extern inline bit_parser_status_t bit_parser_extract_i64(bit_parser_t *inst,
                                                         int64_t *res,
                                                         int n_bits) {
    BIT_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIT_PARSER_CHECK_PARAM_SIZE(inst, n_bits, int64_t);
    BIT_PARSER_EXTRACT_INT(inst, res, int64_t, n_bits, bin_parser_ntoh_64);
    inst->iter += n_bits;

    return BIT_PARSER_OK;
}

#endif /* INCLUDE_BIT_PARSER_PARSER_H_ */
