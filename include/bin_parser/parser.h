/*
 * parser.h
 *
 *  Created on: Jan 28, 2024
 *      Author: pavel
 */

#ifndef INCLUDE_BIN_PARSER_PARSER_H_
#define INCLUDE_BIN_PARSER_PARSER_H_

#include "bin_parser/types.h"

typedef struct c_binary_parser_tag {
    char *buf;
    bit_parser_iter_type_t capacity;
    bit_parser_iter_type_t iter;
} c_binary_parser_t;

void bin_parser_init(c_binary_parser_t *inst, char *buf, bit_parser_iter_type_t buf_len_bits);

bin_parser_status_t bin_parser_skip(c_binary_parser_t *inst, bit_parser_iter_type_t n_bits);

bin_parser_status_t bin_parser_extract_aligned_u8(c_binary_parser_t *inst, uint8_t *res);

bin_parser_status_t bin_parser_extract_aligned_u16(c_binary_parser_t *inst,
                                                   uint16_t *res,
                                                   bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_aligned_u32(c_binary_parser_t *inst,
                                                   uint32_t *res,
                                                   bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_aligned_u64(c_binary_parser_t *inst,
                                                   uint32_t *res,
                                                   bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_u8(c_binary_parser_t *inst, uint8_t *res, int n_bits);

bin_parser_status_t bin_parser_extract_u16(c_binary_parser_t *inst,
                                           uint16_t *res,
                                           int n_bits,
                                           bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_u32(c_binary_parser_t *inst,
                                           uint32_t *res,
                                           int n_bits,
                                           bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_u64(c_binary_parser_t *inst,
                                           uint64_t *res,
                                           int n_bits,
                                           bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_i8(c_binary_parser_t *inst, uint8_t *res, int n_bits);

bin_parser_status_t bin_parser_extract_i16(c_binary_parser_t *inst,
                                           uint8_t *res,
                                           int n_bits,
                                           bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_i32(c_binary_parser_t *inst,
                                           uint8_t *res,
                                           int n_bits,
                                           bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_i64(c_binary_parser_t *inst,
                                           uint8_t *res,
                                           int n_bits,
                                           bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_float(c_binary_parser_t *inst,
                                             uint8_t *res,
                                             bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_double(c_binary_parser_t *inst,
                                              uint8_t *res,
                                              bin_parser_endianess_t byte_order);

bin_parser_status_t bin_parser_extract_c_str(c_binary_parser_t *inst, uint8_t *res, int n_bytes);

/*
 **************************************************************************************************
  Realization
 **************************************************************************************************
 */
#if BIT_PARSER_CHECK_BUFFER_BOUNDARY == 0
#define BIN_PARSER_CHECK_OVERFLOW(inst_, n_bits_)
#else
#define BIN_PARSER_CHECK_OVERFLOW(inst_, n_bits_)          \
    do {                                                   \
        if ((inst_)->capacity - (inst_)->iter < (n_bits_)) \
            return BIN_PARSER_EFULL;                       \
    } while (0)
#endif

#define BIN_PARSER_EXTRACT_UINT(inst_, out_, out_type_, n_bits_)                             \
    do {                                                                                     \
        out_type_ low_bits = inst_->iter & (sizeof(out_type_) * 8 - 1);                      \
        int idx_low = inst_->iter / (8 * sizeof(out_type_));                                 \
        if (low_bits > n_bits_) {                                                            \
            out_type_ low = ((out_type_ *)inst->buf)[idx_low];                               \
            low &= ((2 << low_bits) - 1);                                                    \
            out_type_ hi = ((out_type_ *)inst->buf)[1 + idx_low];                            \
            int hi_bits = n_bits_ - low;                                                     \
            hi >>= 8 * sizeof(out_) - hi_bits;                                               \
            hi <<= low_bits;                                                                 \
            *out_ = hi + low;                                                                \
        }                                                                                    \
        else {                                                                               \
            out_type_ low = ((out_type_ *)inst->buf)[inst_->iter / (8 * sizeof(out_type_))]; \
            int remain = low_bits - n_bits_;                                                 \
            low >> remain;                                                                   \
            low &= ((2 << low_bits) - 1);                                                    \
            *out_ = low;                                                                     \
        }                                                                                    \
    } while (0)

extern inline bin_parser_status_t bin_parser_skip(c_binary_parser_t *inst,
                                                  bit_parser_iter_type_t n_bits) {
    BIN_PARSER_CHECK_OVERFLOW(inst, n_bits);

    inst->iter += n_bits;

    return BIN_PARSER_OK;
}

extern inline bin_parser_status_t bin_parser_extract_u8(c_binary_parser_t *inst,
                                                        uint8_t *res,
                                                        int n_bits) {
    BIN_PARSER_CHECK_OVERFLOW(inst, n_bits);
    BIN_PARSER_EXTRACT_UINT(inst, res, uint8_t, n_bits);
    inst->iter += n_bits;
    return BIN_PARSER_OK;
}

#endif /* INCLUDE_BIN_PARSER_PARSER_H_ */
