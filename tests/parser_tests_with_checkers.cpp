/*
 * example_test.cpp
 *
 *  Created on: Jan 29, 2024
 *      Author: pavel
 */

#include "gtest/gtest.h"

extern "C" {
#define BITP_CHECK_ALL
#include "bitp/parser.h"
}

TEST(parser_tests, u8) {
    uint8_t buf[] = {0xDE, 0xAD};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);

    uint8_t res = 0xAE;
    bitp_status_t status = bitp_parser_extract_u8(&parser, &res, 9);
    ASSERT_EQ(res, 0xAE);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_parser_extract_u8(&parser, &res, 3);
    ASSERT_EQ(res, 6);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_parser_extract_u8(&parser, &res, 8);
    ASSERT_EQ(res, 0xF5);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_parser_extract_u8(&parser, &res, 5);
    ASSERT_EQ(res, 0xD);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_parser_extract_u8(&parser, &res, 1);
    ASSERT_EQ(status, BITP_EFULL);
}

TEST(parser_tests, i8) {
    uint8_t buf[] = {0xDE, 0xAD};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);

    int8_t res;
    bitp_status_t status = bitp_parser_extract_i8(&parser, &res, 9);

    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_parser_extract_i8(&parser, &res, 3);
    ASSERT_EQ(res, -2);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_parser_extract_i8(&parser, &res, 8);
    ASSERT_EQ(res, -11);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_parser_extract_i8(&parser, &res, 5);
    ASSERT_EQ(res, 13);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_parser_extract_i8(&parser, &res, 1);
    ASSERT_EQ(status, BITP_EFULL);
}

TEST(parser_tests, skip) {
    uint8_t buf[] = {0xDE, 0xAD};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);

    auto status = bitp_parser_skip(&parser, 3 + 8);

    ASSERT_EQ(status, BITP_OK);

    uint8_t res = 0xAE;

    status = bitp_parser_extract_u8(&parser, &res, 5);
    ASSERT_EQ(res, 13);
    ASSERT_EQ(status, BITP_OK);
}

TEST(parser_tests, u16) {
    uint8_t buf[] = {0xDE, 0xAD};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    uint16_t res = 0xAE;

    bitp_status_t status = bitp_parser_extract_u16(&parser, &res, 2 * CHAR_BIT + 1);
    ASSERT_EQ(status, BITP_EFULL);

    status = bitp_parser_extract_u16(&parser, &res, 3);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 6);

    status = bitp_parser_extract_u16(&parser, &res, 12);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 3926);

    status = bitp_parser_extract_u16(&parser, &res, 1);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 1);

    status = bitp_parser_extract_u16(&parser, &res, 1);
    ASSERT_EQ(status, BITP_EFULL);
}

TEST(parser_tests, i16) {
    uint8_t buf[] = {0xDE, 0xAD};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    int16_t res = 0xAE;

    bitp_status_t status = bitp_parser_extract_i16(&parser, &res, 2 * CHAR_BIT + 1);
    ASSERT_EQ(status, BITP_EFULL);

    status = bitp_parser_extract_i16(&parser, &res, 3);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -2);

    status = bitp_parser_extract_i16(&parser, &res, 12);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -170);

    status = bitp_parser_extract_i16(&parser, &res, 1);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -1);

    status = bitp_parser_extract_i16(&parser, &res, 1);
    ASSERT_EQ(status, BITP_EFULL);
}

TEST(parser_tests, u32) {
    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    uint32_t res = 0xAE;

    bitp_status_t status = bitp_parser_extract_u32(&parser, &res, 4 * CHAR_BIT + 1);
    ASSERT_EQ(status, BITP_EFULL);

    status = bitp_parser_extract_u32(&parser, &res, 3);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 6);

    status = bitp_parser_extract_u32(&parser, &res, 28);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 257351543);

    status = bitp_parser_extract_u32(&parser, &res, 1);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 1);

    status = bitp_parser_extract_u32(&parser, &res, 1);
    ASSERT_EQ(status, BITP_EFULL);
}

TEST(parser_tests, i32) {
    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    int32_t res = 0xAE;

    bitp_status_t status = bitp_parser_extract_i32(&parser, &res, 4 * CHAR_BIT + 1);
    ASSERT_EQ(status, BITP_EFULL);

    status = bitp_parser_extract_i32(&parser, &res, 3);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -2);

    status = bitp_parser_extract_i32(&parser, &res, 28);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -11083913);

    status = bitp_parser_extract_i32(&parser, &res, 1);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -1);

    status = bitp_parser_extract_i32(&parser, &res, 1);
    ASSERT_EQ(status, BITP_EFULL);
}

TEST(parser_tests, u64) {
    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF, 0xAB, 0xBA};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    uint64_t res = 0xAE;

    bitp_status_t status = bitp_parser_extract_u64(&parser, &res, 8 * CHAR_BIT + 1);
    ASSERT_EQ(status, BITP_EFULL);

    status = bitp_parser_extract_u64(&parser, &res, 4);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 0xD);

    status = bitp_parser_extract_u64(&parser, &res, 56);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 0xEADBEEFFEEFABBULL);

    status = bitp_parser_extract_u64(&parser, &res, 4);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 0xA);

    status = bitp_parser_extract_u64(&parser, &res, 1);
    ASSERT_EQ(status, BITP_EFULL);
}

TEST(parser_tests, i64) {
    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF, 0xAB, 0xBA};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * 8);
    int64_t res = 0xAE;

    bitp_status_t status = bitp_parser_extract_i64(&parser, &res, 8 * CHAR_BIT + 1);
    ASSERT_EQ(status, BITP_EFULL);

    status = bitp_parser_extract_i64(&parser, &res, 4);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -3);

    status = bitp_parser_extract_i64(&parser, &res, 56);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -0x15241100110545LL);

    status = bitp_parser_extract_i64(&parser, &res, 4);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, -6);

    status = bitp_parser_extract_i64(&parser, &res, 1);
    ASSERT_EQ(status, BITP_EFULL);
}

TEST(parser_tests, f64) {
    uint8_t buf[] = {64, 12, 0, 0, 0, 0, 0, 0};

    bitp_parser_t parser;

    bitp_parser_init(&parser, (char *)buf, sizeof(buf) * CHAR_BIT);
    double res = 0.0;

    bitp_status_t status = bitp_parser_extract_double(&parser, &res);
    ASSERT_EQ(status, BITP_OK);
    ASSERT_EQ(res, 3.5);

    status = bitp_parser_extract_double(&parser, &res);
    ASSERT_EQ(status, BITP_EFULL);
}
