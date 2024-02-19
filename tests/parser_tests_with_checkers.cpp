/*
 * example_test.cpp
 *
 *  Created on: Jan 29, 2024
 *      Author: pavel
 */

#include "gtest/gtest.h"

extern "C" {
#define BIT_PARSER_CHECK_ALL
#include "bin_parser/bit_parser.h"
}

TEST(parser_tests, u8) {
    uint8_t buf[] = {0xDE, 0xAD};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);

    uint8_t res = 0xAE;
    bit_parser_status_t status = bit_parser_extract_u8(&parser, &res, 9);
    ASSERT_EQ(res, 0xAE);
    ASSERT_EQ(status, BIT_PARSER_EINVALID_ARG);

    status = bit_parser_extract_u8(&parser, &res, 3);
    ASSERT_EQ(res, 6);
    ASSERT_EQ(status, BIT_PARSER_OK);

    status = bit_parser_extract_u8(&parser, &res, 8);
    ASSERT_EQ(res, 0xF5);
    ASSERT_EQ(status, BIT_PARSER_OK);

    status = bit_parser_extract_u8(&parser, &res, 5);
    ASSERT_EQ(res, 0xD);
    ASSERT_EQ(status, BIT_PARSER_OK);

    status = bit_parser_extract_u8(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);
}

TEST(parser_tests, i8) {
    uint8_t buf[] = {0xDE, 0xAD};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);

    int8_t res;
    bit_parser_status_t status = bit_parser_extract_i8(&parser, &res, 9);

    ASSERT_EQ(status, BIT_PARSER_EINVALID_ARG);

    status = bit_parser_extract_i8(&parser, &res, 3);
    ASSERT_EQ(res, -2);
    ASSERT_EQ(status, BIT_PARSER_OK);

    status = bit_parser_extract_i8(&parser, &res, 8);
    ASSERT_EQ(res, -11);
    ASSERT_EQ(status, BIT_PARSER_OK);

    status = bit_parser_extract_i8(&parser, &res, 5);
    ASSERT_EQ(res, 13);
    ASSERT_EQ(status, BIT_PARSER_OK);

    status = bit_parser_extract_i8(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);
}

TEST(parser_tests, skip) {
    uint8_t buf[] = {0xDE, 0xAD};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);

    auto status = bit_parser_skip(&parser, 3 + 8);

    ASSERT_EQ(status, BIT_PARSER_OK);

    uint8_t res = 0xAE;

    status = bit_parser_extract_u8(&parser, &res, 5);
    ASSERT_EQ(res, 13);
    ASSERT_EQ(status, BIT_PARSER_OK);
}

TEST(parser_tests, u16) {
    uint8_t buf[] = {0xDE, 0xAD};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    uint16_t res = 0xAE;

    bit_parser_status_t status = bit_parser_extract_u16(&parser, &res, 2 * CHAR_BIT + 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);

    status = bit_parser_extract_u16(&parser, &res, 3);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 6);

    status = bit_parser_extract_u16(&parser, &res, 12);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 3926);

    status = bit_parser_extract_u16(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 1);

    status = bit_parser_extract_u16(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);
}

TEST(parser_tests, i16) {
    uint8_t buf[] = {0xDE, 0xAD};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    int16_t res = 0xAE;

    bit_parser_status_t status = bit_parser_extract_i16(&parser, &res, 2 * CHAR_BIT + 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);

    status = bit_parser_extract_i16(&parser, &res, 3);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -2);

    status = bit_parser_extract_i16(&parser, &res, 12);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -170);

    status = bit_parser_extract_i16(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -1);

    status = bit_parser_extract_i16(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);
}

TEST(parser_tests, u32) {
    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    uint32_t res = 0xAE;

    bit_parser_status_t status = bit_parser_extract_u32(&parser, &res, 4 * CHAR_BIT + 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);

    status = bit_parser_extract_u32(&parser, &res, 3);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 6);

    status = bit_parser_extract_u32(&parser, &res, 28);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 257351543);

    status = bit_parser_extract_u32(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 1);

    status = bit_parser_extract_u32(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);
}

TEST(parser_tests, i32) {
    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    int32_t res = 0xAE;

    bit_parser_status_t status = bit_parser_extract_i32(&parser, &res, 4 * CHAR_BIT + 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);

    status = bit_parser_extract_i32(&parser, &res, 3);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -2);

    status = bit_parser_extract_i32(&parser, &res, 28);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -11083913);

    status = bit_parser_extract_i32(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -1);

    status = bit_parser_extract_i32(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);
}

TEST(parser_tests, u64) {
    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF, 0xAB, 0xBA};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    uint64_t res = 0xAE;

    bit_parser_status_t status = bit_parser_extract_u64(&parser, &res, 8 * CHAR_BIT + 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);

    status = bit_parser_extract_u64(&parser, &res, 4);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 0xD);

    status = bit_parser_extract_u64(&parser, &res, 56);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 0xEADBEEFFEEFABBULL);

    status = bit_parser_extract_u64(&parser, &res, 4);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, 0xA);

    status = bit_parser_extract_u64(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);
}

TEST(parser_tests, i64) {
    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF, 0xAB, 0xBA};

    bit_parser_t parser;

    bit_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    int64_t res = 0xAE;

    bit_parser_status_t status = bit_parser_extract_i64(&parser, &res, 8 * CHAR_BIT + 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);

    status = bit_parser_extract_i64(&parser, &res, 4);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -3);

    status = bit_parser_extract_i64(&parser, &res, 56);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -0x15241100110545LL);

    status = bit_parser_extract_i64(&parser, &res, 4);
    ASSERT_EQ(status, BIT_PARSER_OK);
    ASSERT_EQ(res, -6);

    status = bit_parser_extract_i64(&parser, &res, 1);
    ASSERT_EQ(status, BIT_PARSER_EFULL);
}
