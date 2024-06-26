/*
 * coder.h
 *
 *  Created on: Jan 28, 2024
 *      Author: pavel
 */

#ifndef INCLUDE_BITP_PACKER_H_
#define INCLUDE_BITP_PACKER_H_

#include "types.h"

typedef struct bitp_packer_tag {
    char *buf;
    size_t capacity;
    size_t iter;
} bitp_packer_t;

void bitp_packer_init(bitp_packer_t *inst, char *buf, size_t buf_len_bits, int reset_buffer);

bitp_status_t bitp_packer_add_u8(bitp_packer_t *inst, uint8_t val, size_t n_bits);

bitp_status_t bitp_packer_add_u16(bitp_packer_t *inst, uint16_t val, size_t n_bits);

bitp_status_t bitp_packer_add_u32(bitp_packer_t *inst, uint32_t val, size_t n_bits);

bitp_status_t bitp_packer_add_u64(bitp_packer_t *inst, uint64_t val, size_t n_bits);

bitp_status_t bitp_packer_add_i8(bitp_packer_t *inst, int8_t val, size_t n_bits);

bitp_status_t bitp_packer_add_i16(bitp_packer_t *inst, int16_t val, size_t n_bits);

bitp_status_t bitp_packer_add_i32(bitp_packer_t *inst, int32_t val, size_t n_bits);

bitp_status_t bitp_packer_add_i64(bitp_packer_t *inst, int64_t val, size_t n_bits);

bitp_status_t bitp_packer_add_float(bitp_packer_t *inst, float val);

bitp_status_t bitp_packer_add_double(bitp_packer_t *inst, double val);

/*
 **************************************************************************************************
  Realization
 **************************************************************************************************
 */

inline void bitp_packer_init(bitp_packer_t *inst,
                             char *buf,
                             size_t buf_len_bits,
                             int reset_buffer) {
    inst->buf = buf;
    inst->capacity = buf_len_bits;
    inst->iter = 0;

    if (reset_buffer) {
        for (size_t i = 0; i < (buf_len_bits + CHAR_BIT - 1) / CHAR_BIT; i++) {
            inst->buf[i] = 0;
        }
    }
}

inline void bitp_packer_add_u8_no_check(bitp_packer_t *inst, uint8_t val, size_t n_bits) {
    unsigned hi_bits = CHAR_BIT - (inst->iter % CHAR_BIT);
    if (hi_bits < n_bits) {
        unsigned low_bits = n_bits - hi_bits;
        inst->buf[inst->iter / CHAR_BIT] |= val >> low_bits;
        inst->buf[inst->iter / CHAR_BIT + 1] |= val << (CHAR_BIT - low_bits);
    }
    else {
        inst->buf[inst->iter / CHAR_BIT] |= val << (hi_bits - n_bits);
    }

    inst->iter += n_bits;
}

inline bitp_status_t bitp_packer_add_u8(bitp_packer_t *inst, uint8_t val, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint8_t);
    BITP_CHECK_PARAM_RANGE(val, n_bits, 0);
    bitp_packer_add_u8_no_check(inst, val, n_bits);
    return BITP_OK;
}

inline bitp_status_t bitp_packer_add_i8(bitp_packer_t *inst, int8_t val, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint8_t);
    BITP_CHECK_PARAM_RANGE(val, n_bits, 1);
    bitp_packer_add_u8_no_check(inst, (uint8_t)val, n_bits);

    return BITP_OK;
}

#define BITP_PACK_WORD(inst_, word_, n_bits_)                                               \
    do {                                                                                    \
        int num_bytes_ = (n_bits_) / CHAR_BIT;                                              \
        int rem_ = (n_bits_)-num_bytes_ * CHAR_BIT;                                         \
        if (rem_) {                                                                         \
            bitp_packer_add_u8_no_check((inst_), (word_) >> (num_bytes_ * CHAR_BIT), rem_); \
        }                                                                                   \
        for (int i_ = 0; i_ < num_bytes_; ++i_) {                                           \
            bitp_packer_add_u8_no_check((inst_),                                            \
                                        (word_) >> ((num_bytes_ - i_ - 1) * CHAR_BIT),      \
                                        CHAR_BIT);                                          \
        }                                                                                   \
    } while (0)

inline bitp_status_t bitp_packer_add_u16(bitp_packer_t *inst, uint16_t val, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint16_t);
    BITP_CHECK_PARAM_RANGE(val, n_bits, 0);

    BITP_PACK_WORD(inst, val, n_bits);

    return BITP_OK;
}

inline bitp_status_t bitp_packer_add_i16(bitp_packer_t *inst, int16_t val, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, int16_t);
    BITP_CHECK_PARAM_RANGE(val, n_bits, 1);

    uint16_t valu = (uint16_t)val;
    valu &= (1 << n_bits) - 1;

    BITP_PACK_WORD(inst, valu, n_bits);

    return BITP_OK;
}

inline bitp_status_t bitp_packer_add_u32(bitp_packer_t *inst, uint32_t val, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint32_t);
    BITP_CHECK_PARAM_RANGE(val, n_bits, 0);

    BITP_PACK_WORD(inst, val, n_bits);

    return BITP_OK;
}

inline bitp_status_t bitp_packer_add_i32(bitp_packer_t *inst, int32_t val, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, int32_t);
    BITP_CHECK_PARAM_RANGE(val, n_bits, 1);

    uint32_t valu = (uint32_t)val;
    valu &= (1 << n_bits) - 1;

    BITP_PACK_WORD(inst, valu, n_bits);

    return BITP_OK;
}

inline bitp_status_t bitp_packer_add_u64(bitp_packer_t *inst, uint64_t val, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint64_t);
    BITP_CHECK_PARAM_RANGE(val, n_bits, 0);

    BITP_PACK_WORD(inst, val, n_bits);

    return BITP_OK;
}

inline bitp_status_t bitp_packer_add_i64(bitp_packer_t *inst, int64_t val, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, int64_t);
    BITP_CHECK_PARAM_RANGE(val, n_bits, 1);

    uint64_t valu = (uint64_t)val;
    valu &= (1ULL << n_bits) - 1;

    BITP_PACK_WORD(inst, valu, n_bits);

    return BITP_OK;
}

inline bitp_status_t bitp_packer_add_float(bitp_packer_t *inst, float val) {
    BITP_CHECK_OVERFLOW(inst, CHAR_BIT * 4);

    uint32_t valu;
    memcpy(&valu, &val, 4);

    BITP_PACK_WORD(inst, valu, CHAR_BIT * 4);

    return BITP_OK;
}

inline bitp_status_t bitp_packer_add_double(bitp_packer_t *inst, double val) {
    BITP_CHECK_OVERFLOW(inst, CHAR_BIT * 8);

    uint64_t valu;
    memcpy(&valu, &val, 8);

    BITP_PACK_WORD(inst, valu, CHAR_BIT * 8);

    return BITP_OK;
}

#endif /* INCLUDE_BITP_PACKER_H_ */
