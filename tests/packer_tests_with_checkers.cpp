/*
 * packer_tests.cpp
 *
 *  Created on: Feb 23, 2024
 *      Author: pavel
 */

#include "gtest/gtest.h"

extern "C" {
#define BITP_CHECK_ALL
#include "bitp/packer.h"
}

TEST(packer_tests, u8) {
    uint8_t buf[2] = {0xFE, 0xEF};
    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, sizeof(buf) * 8, true);

    bitp_status_t status = bitp_packer_add_u8(&packer, 0xAE, 9);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_u8(&packer, 5, 2);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_u8(&packer, 6, 3);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u8(&packer, 0xF5, 8);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u8(&packer, 0xD, 5);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u8(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {0xDE, 0xAD};

    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, i8) {
    uint8_t buf[2] = {0xFE, 0xEF};
    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, sizeof(buf) * 8, true);

    bitp_status_t status = bitp_packer_add_i8(&packer, 0x1, 9);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_i8(&packer, -3, 2);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_i8(&packer, 2, 2);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_i8(&packer, -2, 3);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_i8(&packer, -11, 8);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_i8(&packer, 13, 5);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_i8(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {0xDE, 0xAD};
    for (int i = 0; i < 2; ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, u16) {
    uint8_t buf[] = {0xFE, 0xEF};
    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, sizeof(buf) * 8, true);

    bitp_status_t status = bitp_packer_add_u16(&packer, 0x1, 17);
    ASSERT_NE(status, BITP_OK);

    status = bitp_packer_add_u16(&packer, 16, 4);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_u16(&packer, 6, 3);
    ASSERT_EQ(status, BITP_OK);
    status = bitp_packer_add_u16(&packer, 3926, 12);
    ASSERT_EQ(status, BITP_OK);
    status = bitp_packer_add_u16(&packer, 1, 1);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u16(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {0xDE, 0xAD};

    for (size_t i = 0; i < sizeof(buf); ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, i16) {
    uint8_t buf[] = {0xFE, 0xEF};
    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, sizeof(buf) * 8, true);

    bitp_status_t status = bitp_packer_add_i16(&packer, 0x1, 17);
    ASSERT_NE(status, BITP_OK);

    status = bitp_packer_add_i16(&packer, 16, 4);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_i16(&packer, -2, 3);
    ASSERT_EQ(status, BITP_OK);
    status = bitp_packer_add_i16(&packer, -170, 12);
    ASSERT_EQ(status, BITP_OK);
    status = bitp_packer_add_i16(&packer, -1, 1);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_i16(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {0xDE, 0xAD};

    for (size_t i = 0; i < sizeof(buf); ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, u32) {
    uint8_t buf[] = {0xFF, 0xFE, 0xFF, 0xAB};

    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, CHAR_BIT * sizeof(buf), 1);

    bitp_status_t status = bitp_packer_add_u32(&packer, 0x1, 33);
    ASSERT_NE(status, BITP_OK);

    status = bitp_packer_add_u32(&packer, 16, 4);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_u32(&packer, 6, 3);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u32(&packer, 257351543, 28);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u32(&packer, 1, 1);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u32(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {0xDE, 0xAD, 0xBE, 0xEF};

    for (size_t i = 0; i < sizeof(buf); ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, i32) {
    uint8_t buf[] = {0xFF, 0xFE, 0xFF, 0xAB};

    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, CHAR_BIT * sizeof(buf), 1);

    bitp_status_t status = bitp_packer_add_i32(&packer, 0x1, 33);
    ASSERT_NE(status, BITP_OK);

    status = bitp_packer_add_i32(&packer, 16, 4);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_i32(&packer, -2, 3);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_i32(&packer, -11083913, 28);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_i32(&packer, -1, 1);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_i32(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {0xDE, 0xAD, 0xBE, 0xEF};

    for (size_t i = 0; i < sizeof(buf); ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, u64) {
    uint8_t buf[] = {0xFF, 0xFE, 0xFF, 0xAB, 0xFF, 0xFE, 0xFF, 0xAB};

    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, CHAR_BIT * sizeof(buf), 1);

    bitp_status_t status = bitp_packer_add_u64(&packer, 0x1, 65);
    ASSERT_NE(status, BITP_OK);

    status = bitp_packer_add_u64(&packer, 16, 4);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_u64(&packer, 0xD, 4);
    ASSERT_EQ(status, BITP_OK);
    status = bitp_packer_add_u64(&packer, 0xEADBEEFFEEFABBULL, 56);
    ASSERT_EQ(status, BITP_OK);
    status = bitp_packer_add_u64(&packer, 0xA, 4);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u64(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF, 0xAB, 0xBA};

    for (size_t i = 0; i < sizeof(buf); ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, i64) {
    uint8_t buf[] = {0xFF, 0xFE, 0xFF, 0xAB, 0xFF, 0xFE, 0xFF, 0xAB};

    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, CHAR_BIT * sizeof(buf), 1);

    bitp_status_t status = bitp_packer_add_i64(&packer, 0x1, 65);
    ASSERT_NE(status, BITP_OK);

    status = bitp_packer_add_i64(&packer, 16, 4);
    ASSERT_EQ(status, BITP_EINVALID_ARG);

    status = bitp_packer_add_i64(&packer, -3, 4);
    ASSERT_EQ(status, BITP_OK);
    status = bitp_packer_add_i64(&packer, -0x15241100110545LL, 56);
    ASSERT_EQ(status, BITP_OK);
    status = bitp_packer_add_i64(&packer, -6, 4);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_i64(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF, 0xAB, 0xBA};

    for (size_t i = 0; i < sizeof(buf); ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, f32) {
    uint8_t buf[] = {0xFF, 0xFE, 0xFF, 0xAB};

    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, CHAR_BIT * sizeof(buf), 1);

    bitp_status_t status = bitp_packer_add_float(&packer, 2.5);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u32(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {64, 32, 0, 0};

    for (size_t i = 0; i < sizeof(buf); ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}

TEST(packer_tests, f64) {
    uint8_t buf[] = {0xFF, 0xFE, 0xFF, 0xAB, 0xFF, 0xFE, 0xFF, 0xAB};

    bitp_packer_t packer;

    bitp_packer_init(&packer, (char *)buf, CHAR_BIT * sizeof(buf), 1);

    bitp_status_t status = bitp_packer_add_double(&packer, 3.5);
    ASSERT_EQ(status, BITP_OK);

    status = bitp_packer_add_u32(&packer, 1, 1);
    ASSERT_EQ(status, BITP_EFULL);

    uint8_t expected[] = {64, 12, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < sizeof(buf); ++i) {
        ASSERT_EQ(buf[i], expected[i]);
    }
}
