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

typedef enum bin_parser_status_tag {
    BIT_PARSER_OK = 0,
    BIT_PARSER_EFULL,
    BIT_PARSER_EINVALID_ARG
} bitp_status_t;

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

typedef struct bitp_inst_tag {
    const char *buf;
    size_t capacity;
    size_t iter;
} bitp_inst_t;

#endif /* INCLUDE_BITP_TYPES_H_ */
