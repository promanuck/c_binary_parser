/*
 * coder.h
 *
 *  Created on: Jan 28, 2024
 *      Author: pavel
 */

#ifndef INCLUDE_BITP_PACKER_H_
#define INCLUDE_BITP_PACKER_H_

#include "types.h"

void bitp_packer_init(bitp_inst_t *inst, const char *buf, size_t buf_len_bits);

bitp_status_t bitp_pack_u8(bitp_inst_t *inst, uint8_t val, int n_bits);

bitp_status_t bitp_pack_u16(bitp_inst_t *inst, uint16_t val, int n_bits);

bitp_status_t bitp_pack_u32(bitp_inst_t *inst, uint32_t val, int n_bits);

bitp_status_t bitp_pack_u64(bitp_inst_t *inst, uint64_t val, int n_bits);

bitp_status_t bitp_pack_i8(bitp_inst_t *inst, int8_t val, int n_bits);

bitp_status_t bitp_pack_i16(bitp_inst_t *inst, int16_t val, int n_bits);

bitp_status_t bitp_pack_i32(bitp_inst_t *inst, int32_t val, int n_bits);

bitp_status_t bitp_pack_i64(bitp_inst_t *inst, int64_t val, int n_bits);

bitp_status_t bitp_pack_float(bitp_inst_t *inst, float val);

bitp_status_t bitp_pack_double(bitp_inst_t *inst, double val);

#endif /* INCLUDE_BITP_PACKER_H_ */
