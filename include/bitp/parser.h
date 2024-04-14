/*
 * parser.h
 *
 *  Created on: Jan 28, 2024
 *      Author: pavel
 */

#ifndef INCLUDE_BIT_PARSER_PARSER_H_
#define INCLUDE_BIT_PARSER_PARSER_H_

#include "types.h"

typedef struct bitp_parser_tag {
    const char *buf;
    size_t capacity;
    size_t iter;
} bitp_parser_t;

void bitp_parser_init(bitp_parser_t *inst, const char *buf, size_t buf_len_bits);

bitp_status_t bitp_parser_skip(bitp_parser_t *inst, size_t n_bits);

bitp_status_t bitp_parser_extract_u8(bitp_parser_t *inst, uint8_t *res, unsigned n_bits);

bitp_status_t bitp_parser_extract_u16(bitp_parser_t *inst, uint16_t *res, unsigned n_bits);

bitp_status_t bitp_parser_extract_u32(bitp_parser_t *inst, uint32_t *res, unsigned n_bits);

bitp_status_t bitp_parser_extract_u64(bitp_parser_t *inst, uint64_t *res, unsigned n_bits);

bitp_status_t bitp_parser_extract_i8(bitp_parser_t *inst, int8_t *res, unsigned n_bits);

bitp_status_t bitp_parser_extract_i16(bitp_parser_t *inst, int16_t *res, unsigned n_bits);

bitp_status_t bitp_parser_extract_i32(bitp_parser_t *inst, int32_t *res, unsigned n_bits);

bitp_status_t bitp_parser_extract_i64(bitp_parser_t *inst, int64_t *res, unsigned n_bits);

bitp_status_t bitp_parser_extract_float(bitp_parser_t *inst, float *res);

bitp_status_t bitp_parser_extract_double(bitp_parser_t *inst, double *res);

/*
 **************************************************************************************************
  Realization
 **************************************************************************************************
 */

#define BITP_EXTRACT_INTEGER_(inst_, out_, out_type_, n_bits_, unsigned_prefix, bswap_)    \
    do {                                                                                   \
        int bits_in_type = sizeof(out_type_) * CHAR_BIT;                                   \
        unsigned hi_bits = bits_in_type - (inst_->iter % bits_in_type);                    \
        int idx_low = sizeof(out_type_) * (inst_->iter / bits_in_type);                    \
        if (hi_bits < n_bits_) {                                                           \
            unsigned_prefix##out_type_ high;                                               \
            memcpy((void *)&high, (void *)&inst_->buf[idx_low], sizeof(high));             \
            high = bswap_(high);                                                           \
            unsigned_prefix##out_type_ low;                                                \
            memcpy((void *)&low, (void *)&inst_->buf[idx_low + sizeof(low)], sizeof(low)); \
            low = bswap_(low);                                                             \
            low >>= hi_bits;                                                               \
            low += high << (inst_->iter % bits_in_type);                                   \
            memcpy(out_, &low, sizeof(low));                                               \
            *out_ >>= (bits_in_type - n_bits_);                                            \
        }                                                                                  \
        else {                                                                             \
            unsigned_prefix##out_type_ low;                                                \
            memcpy((void *)&low, (void *)&inst_->buf[idx_low], sizeof(low));               \
            low = bswap_(low);                                                             \
            low <<= (inst_->iter % bits_in_type);                                          \
            memcpy(out_, &low, sizeof(low));                                               \
            *out_ >>= bits_in_type - n_bits_;                                              \
        }                                                                                  \
    } while (0)

#define BITP_EXTRACT_UINT(inst_, out_, out_type_, n_bits_, bswap_) \
    BITP_EXTRACT_INTEGER_(inst_, out_, out_type_, n_bits_, , bswap_)

#define BITP_EXTRACT_INT(inst_, out_, out_type_, n_bits_, bswap_) \
    BITP_EXTRACT_INTEGER_(inst_, out_, out_type_, n_bits, u, bswap_)

extern inline void bitp_parser_init(bitp_parser_t *inst, const char *buf, size_t buf_len_bits) {
    inst->buf = buf;
    inst->capacity = buf_len_bits;
    inst->iter = 0;
}

extern inline bitp_status_t bitp_parser_skip(bitp_parser_t *inst, size_t n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);

    inst->iter += n_bits;

    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_u8(bitp_parser_t *inst,
                                                   uint8_t *res,
                                                   unsigned n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint8_t);
    BITP_EXTRACT_UINT(inst, res, uint8_t, n_bits, bitp_ntoh_8);
    inst->iter += n_bits;
    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_i8(bitp_parser_t *inst,
                                                   int8_t *res,
                                                   unsigned n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, int8_t);
    BITP_EXTRACT_INT(inst, res, int8_t, n_bits, bitp_ntoh_8);
    inst->iter += n_bits;
    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_u16(bitp_parser_t *inst,
                                                    uint16_t *res,
                                                    unsigned n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint16_t);
    BITP_EXTRACT_UINT(inst, res, uint16_t, n_bits, bitp_ntoh_16);
    inst->iter += n_bits;

    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_i16(bitp_parser_t *inst,
                                                    int16_t *res,
                                                    unsigned n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, int16_t);
    BITP_EXTRACT_INT(inst, res, int16_t, n_bits, bitp_ntoh_16);
    inst->iter += n_bits;

    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_u32(bitp_parser_t *inst,
                                                    uint32_t *res,
                                                    unsigned n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint32_t);
    BITP_EXTRACT_UINT(inst, res, uint32_t, n_bits, bitp_ntoh_32);
    inst->iter += n_bits;

    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_i32(bitp_parser_t *inst,
                                                    int32_t *res,
                                                    unsigned n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, int32_t);
    BITP_EXTRACT_INT(inst, res, int32_t, n_bits, bitp_ntoh_32);
    inst->iter += n_bits;

    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_u64(bitp_parser_t *inst,
                                                    uint64_t *res,
                                                    unsigned n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, uint64_t);
    BITP_EXTRACT_UINT(inst, res, uint64_t, n_bits, bitp_ntoh_64);
    inst->iter += n_bits;

    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_i64(bitp_parser_t *inst,
                                                    int64_t *res,
                                                    unsigned n_bits) {
    BITP_CHECK_OVERFLOW(inst, n_bits);
    BITP_CHECK_PARAM_SIZE(inst, n_bits, int64_t);
    BITP_EXTRACT_INT(inst, res, int64_t, n_bits, bitp_ntoh_64);
    inst->iter += n_bits;

    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_float(bitp_parser_t *inst, float *res) {
    unsigned float_size_bits = CHAR_BIT * sizeof(float);
    BITP_CHECK_OVERFLOW(inst, float_size_bits);
    BITP_CHECK_PARAM_SIZE(inst, float_size_bits, float);
    uint32_t tmp;
    BITP_EXTRACT_UINT(inst, &tmp, uint32_t, float_size_bits, bitp_ntoh_32);
    memcpy(res, &tmp, sizeof(tmp));
    inst->iter += float_size_bits;
    return BITP_OK;
}

extern inline bitp_status_t bitp_parser_extract_double(bitp_parser_t *inst, double *res) {
    unsigned double_size_bits = CHAR_BIT * sizeof(double);
    BITP_CHECK_OVERFLOW(inst, double_size_bits);
    BITP_CHECK_PARAM_SIZE(inst, double_size_bits, double);
    uint64_t tmp;
    BITP_EXTRACT_UINT(inst, &tmp, uint64_t, double_size_bits, bitp_ntoh_64);
    memcpy(res, &tmp, sizeof(tmp));
    inst->iter += double_size_bits;
    return BITP_OK;
}

#endif /* INCLUDE_BIT_PARSER_PARSER_H_ */
