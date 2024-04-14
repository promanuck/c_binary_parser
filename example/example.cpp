/*
 * example.cpp
 *
 *  Created on: Mar 10, 2024
 *      Author: pavel
 */

#include <iostream>

#define BITP_CHECK_ALL 1
#include "bitp/packer.h"
#include "bitp/parser.h"

/*
BCCH-BCH-Message-NB ::= SEQUENCE {
    message BCCH-BCH-MessageType-NB
}

BCCH-BCH-MessageType-NB::=  MasterInformationBlock-NB

MasterInformationBlock-NB ::= SEQUENCE {
    systemFrameNumber-MSB-r13 BIT STRING (SIZE (4)),
    hyperSFN-LSB-r13 BIT STRING (SIZE (2)),
    schedulingInfoSIB1-r13 INTEGER (0..15),
    systemInfoValueTag-r13 INTEGER (0..31),
    ab-Enabled-r13 BOOLEAN,
    operationModeInfo-r13 CHOICE {
    inband-SamePCI-r13 Inband-SamePCI-NB-r13,
    inband-DifferentPCI-r13 Inband-DifferentPCI-NB-r13,
    guardband-r13 Guardband-NB-r13,
    standalone-r13 Standalone-NB-r13
    },
    additionalTransmissionSIB1-r15 BOOLEAN,
    spare BIT STRING (SIZE (10))
    }
    Guardband-NB-r13 ::= SEQUENCE {
    rasterOffset-r13 ChannelRasterOffset-NB-r13,
    spare BIT STRING (SIZE (3))
    }
    Inband-SamePCI-NB-r13 ::= SEQUENCE {
    eutra-CRS-SequenceInfo-r13 INTEGER (0..31)
    }
    Inband-DifferentPCI-NB-r13 ::= SEQUENCE {
    eutra-NumCRS-Ports-r13 ENUMERATED {same, four},
    rasterOffset-r13 ChannelRasterOffset-NB-r13,
    spare BIT STRING (SIZE (2))
}

Standalone-NB-r13 ::= SEQUENCE {
    spare BIT STRING (SIZE (5))
}
*/

int main() {
    // LTE BCCH-BCH-Message-NB
    uint8_t data[] = {0x00, 0x92, 0xC0, 0x00, 0x00};

    bitp_parser_t parser;
    bitp_parser_init(&parser, (char *)data, CHAR_BIT * sizeof(data));

    uint8_t frame_num_msb;
    uint8_t hyperfame_lsb;
    uint8_t scheduling_info_sib1;
    uint8_t system_info_value_tag;

    bitp_status_t status = bitp_parser_extract_u8(&parser, &frame_num_msb, 4);
    if (status != BITP_OK) {
        return -1;
    }
    status = bitp_parser_extract_u8(&parser, &hyperfame_lsb, 2);
    status = bitp_parser_extract_u8(&parser, &scheduling_info_sib1, 4);
    status = bitp_parser_extract_u8(&parser, &system_info_value_tag, 5);

    std::cout << "frame_num_msb = " << (int)frame_num_msb << std::endl
              << "hyperfame_lsb = " << (int)hyperfame_lsb << std::endl
              << "scheduling info sib1 = " << (int)scheduling_info_sib1 << std::endl
              << "system info value tag = " << (int)system_info_value_tag << std::endl;
    return 0;
}
