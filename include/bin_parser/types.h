/*
 * defs.h
 *
 *  Created on: Jan 28, 2024
 *      Author: pavel
 */

#ifndef INCLUDE_BIN_PARSER_TYPES_H_
#define INCLUDE_BIN_PARSER_TYPES_H_

#include <stdint.h>

#ifdef BIT_PARSER_CHECK_ALL
#define BIT_PARSER_CHECK_BUFFER_BOUNDARY 1
#define BIT_PARSER_CHECK_PARAM_SIZE 1
#endif

#ifndef BIT_PARSER_CHECK_BUFFER_BOUNDARY
#define BIT_PARSER_CHECK_BUFFER_BOUNDARY 0
#endif

#ifndef BIT_PARSER_CHECK_PARAM_SIZE
#define BIT_PARSER_CHECK_PARAM_SIZE 0
#endif

typedef uint32_t bit_parser_iter_type_t;

typedef enum bin_parser_status_tag {
    BIN_PARSER_OK = 0,
    BIN_PARSER_EFULL,
    BIN_PARSER_EINVALID_ARG
} bin_parser_status_t;

typedef enum bin_parser_endianess_tag {
    BIN_PARSER_END_LITTLE,
    BIN_PARSER_END_BIG,
    BIN_PARSER_END_HOST
} bin_parser_endianess_t;

#endif /* INCLUDE_BIN_PARSER_TYPES_H_ */
