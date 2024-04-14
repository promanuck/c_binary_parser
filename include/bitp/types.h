/*
 * bit_parser_types.h
 *
 *  Created on: Feb 22, 2024
 *      Author: pavel
 */

#ifndef INCLUDE_BITP_TYPES_H_
#define INCLUDE_BITP_TYPES_H_

#include <string.h>
#include <limits.h>
#include <stdint.h>

#define bitp_ntoh_8(x) x

#if defined(__BYTE_ORDER__)
#warning __BYTE_ORDER__ SHOULD BE DEFINED TO __ORDER_BIG_ENDIAN__ or __ORDER_LITTLE_ENDIAN__. Using __ORDER_LITTLE_ENDIAN__
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#ifdef _MSC_VER

#include <stdlib.h>
#define bitp_ntoh_16(x) _byteswap_ushort(x)
#define bitp_ntoh_32(x) _byteswap_ulong(x)
#define bitp_ntoh_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define bitp_ntoh_16(x) OSSwapInt16(x)
#define bitp_ntoh_32(x) OSSwapInt32(x)
#define bitp_ntoh_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bitp_ntoh_16(x) BSWAP_16(x)
#define bitp_ntoh_32(x) BSWAP_32(x)
#define bitp_ntoh_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bitp_ntoh_16(x) bswap16(x)
#define bitp_ntoh_32(x) bswap32(x)
#define bitp_ntoh_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bitp_ntoh_16(x) swap16(x)
#define bitp_ntoh_32(x) swap32(x)
#define bitp_ntoh_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bin_parser_ntoh_32)
#define bitp_ntoh_16(x) bswap16(x)
#define bitp_ntoh_32(x) bswap32(x)
#define bitp_ntoh_64(x) bswap64(x)
#endif

#else

#include <byteswap.h>
#define bitp_ntoh_16(x) bswap_16(x)
#define bitp_ntoh_32(x) bswap_32(x)
#define bitp_ntoh_64(x) bswap_64(x)

#endif

#else

#define bitp_ntoh_16(x) x
#define bitp_ntoh_32(x) x
#define bitp_ntoh_64(x) x

#endif    // __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#ifdef BITP_CHECK_ALL
#define BITP_CHECK_BUFFER_BOUNDARY 1
#define BITP_CHECK_PARAM 1
#define BITP_CHECK_RANGE 1
#endif

#ifndef BITP_CHECK_BUFFER_BOUNDARY
#define BITP_CHECK_BUFFER_BOUNDARY 0
#endif

#ifndef BITP_CHECK_PARAM
#define BITP_CHECK_PARAM 0
#endif

#ifndef BITP_CHECK_RANGE
#define BITP_CHECK_RANGE 0
#endif

typedef enum bin_parser_status_tag { BITP_OK = 0, BITP_EFULL, BITP_EINVALID_ARG } bitp_status_t;

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

#if BITP_CHECK_BUFFER_BOUNDARY == 0
#define BITP_CHECK_OVERFLOW(inst_, n_bits_)
#else
#define BITP_CHECK_OVERFLOW(inst_, n_bits_)                \
    do {                                                   \
        if ((inst_)->capacity - (inst_)->iter < (n_bits_)) \
            return BITP_EFULL;                             \
    } while (0)
#endif

#if BITP_CHECK_PARAM == 0
#define BITP_CHECK_PARAM_SIZE(inst_, n_bits_, type_)
#else
#define BITP_CHECK_PARAM_SIZE(inst_, n_bits_, type_) \
    do {                                             \
        if (n_bits_ > 8 * sizeof(type_))             \
            return BITP_EINVALID_ARG;                \
    } while (0)
#endif

#if BITP_CHECK_RANGE == 0
#define BITP_CHECK_PARAM_RANGE(val_, n_bits_, is_signed_)
#else
#define BITP_CHECK_PARAM_RANGE(val_, n_bits_, is_signed_)                   \
    do {                                                                    \
        uint64_t max_val_u_ = 0xFFFFFFFFFFFFFFFFULL >> (64 - n_bits_);      \
        if (is_signed_) {                                                   \
            int64_t max_val_i_ = max_val_u_ / 2;                            \
            int64_t min_val_i_ = -max_val_i_ - 1;                           \
            if ((int64_t)val_ > max_val_i_ || (int64_t)val_ < min_val_i_) { \
                return BITP_EINVALID_ARG;                                   \
            }                                                               \
        }                                                                   \
        else {                                                              \
            if ((uint64_t)val_ > max_val_u_) {                              \
                return BITP_EINVALID_ARG;                                   \
            }                                                               \
        }                                                                   \
    } while (0)
#endif

#endif /* INCLUDE_BITP_TYPES_H_ */
