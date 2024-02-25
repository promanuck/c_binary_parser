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
