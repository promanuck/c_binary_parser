# BITP

C-library for packing and parsing binary data.

* intended to pack and parse unaligned bit-oriended data, MSB-first;
* fast and lightweight, only headers;
* no dynamic memory allocations, perfect for embedded applications;
* configurable checks (e.g. buffer overflow, out of range).

## Quick start

```c
int parse_nb_bcch_bch_msg(void) {
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

```
## Usage

### Packer

1.  Init packer instance.
    ```c
    void bitp_packer_init(bitp_packer_t *inst, char *buf, size_t buf_len_bits, int reset_buffer)
    ```
    where:
    * inst - bitp_packer_t inst;
    * buf - preallocated buffer with size at least buf_len_bits/CHAR_BIT. This buffer will contain resulting message.
    Lifetime of this buffer should be greater or equal to inst;
    * buf_len_bits - maximum allowed message size, bits;
    * reset_buffer - if the buf isn't zeroed, you have to set this flag to 1.

1. Pack unsigned integer
   ```c
   bitp_status_t bitp_packer_add_u8(bitp_packer_t *inst, uint8_t val, size_t n_bits)
   bitp_status_t bitp_packer_add_u16(bitp_packer_t *inst, uint16_t val, size_t n_bits)
   bitp_status_t bitp_packer_add_u32(bitp_packer_t *inst, uint32_t val, size_t n_bits)
   bitp_status_t bitp_packer_add_u64(bitp_packer_t *inst, uint64_t val, size_t n_bits)
   ```
   where
   * inst - bitp_packer_t inst;
   * val - value to pack;
   * n_bits - number of bits to pack.
  
   returns `BITP_OK` in case of success. It can return error if runtime checkings are enabled (see [Configuration](#configuration))

1. Pack signed integer (two's complement)
   ```c
   bitp_status_t bitp_packer_add_i8(bitp_packer_t *inst, int8_t val, size_t n_bits)
   bitp_status_t bitp_packer_add_i16(bitp_packer_t *inst, int16_t val, size_t n_bits)
   bitp_status_t bitp_packer_add_i32(bitp_packer_t *inst, int32_t val, size_t n_bits)
   bitp_status_t bitp_packer_add_i64(bitp_packer_t *inst, int64_t val, size_t n_bits)
   ```
   where
   * inst - bitp_packer_t inst;
   * val - value to pack;
   * n_bits - number of bits to pack.
  
   returns `BITP_OK` in case of success. It can return error if runtime checkings are enabled (see [Configuration](#configuration))

1. Pack float (IEEE 754, single and double precision)
   ```c
    bitp_status_t bitp_packer_add_float(bitp_packer_t *inst, float val)
    bitp_status_t bitp_packer_add_double(bitp_packer_t *inst, double val)
   ```
   where
   * inst - bitp_packer_t inst;
   * val - value to pack;
  
   returns `BITP_OK` in case of success. It can return error if runtime checkings are enabled (see [Configuration](#configuration))

### Parser

1.  Init parser instance.
    ```c
    void bitp_parser_init(bitp_parser_t *inst, const char *buf, size_t buf_len_bits);
    ```
    where:
    * inst - bitp_parser_t inst;
    * buf - buffer with size at least buf_len_bits/CHAR_BIT. This buffer should contain parsing message.
    Lifetime of this buffer should be greater or equal to inst;
    * buf_len_bits - message size, bits.

1. Skip bits:
    ```c
    bitp_status_t bitp_parser_skip(bitp_parser_t *inst, size_t n_bits);
    ```
    where:
    * inst - bitp_parser_t inst;
    * n_bits - number of bits to skip.
  
   returns `BITP_OK` in case of success. It can return error if runtime checkings are enabled (see [Configuration](#configuration))
   
1. Parse unsigned integer
   ```c
   bitp_status_t bitp_parser_extract_u8(bitp_parser_t *inst, uint8_t *res, unsigned n_bits)
   bitp_status_t bitp_parser_extract_u16(bitp_parser_t *inst, uint16_t *res, unsigned n_bits)
   bitp_status_t bitp_parser_extract_u32(bitp_parser_t *inst, uint32_t *res, unsigned n_bits)
   bitp_status_t bitp_parser_extract_u64(bitp_parser_t *inst, uint64_t *res, unsigned n_bits)
   ```
   where
   * inst - bitp_parser_t inst;
   * res - variable to store extracted value;
   * n_bits - number of bits to extract.
  
   returns `BITP_OK` in case of success. It can return error if runtime checkings are enabled (see [Configuration](#configuration))

1. Parse signed integer (two's complement)
   ```c
   bitp_status_t bitp_parser_extract_i8(bitp_parser_t *inst, int8_t *res, unsigned n_bits)
   bitp_status_t bitp_parser_extract_i16(bitp_parser_t *inst, int16_t *res, unsigned n_bits)
   bitp_status_t bitp_parser_extract_i32(bitp_parser_t *inst, int32_t *res, unsigned n_bits)
   bitp_status_t bitp_parser_extract_i64(bitp_parser_t *inst, int64_t *res, unsigned n_bits);
   ```
   where
   * inst - bitp_parser_t inst;
   * res - variable to store extracted value;
   * n_bits - number of bits to extract.
  
   returns `BITP_OK` in case of success. It can return error if runtime checkings are enabled (see [Configuration](#configuration))

1. Pack float (IEEE 754, single and double precision)
   ```c
    bitp_status_t bitp_packer_add_float(bitp_packer_t *inst, float val)
    bitp_status_t bitp_packer_add_double(bitp_packer_t *inst, double val)
   ```
   where
   * inst - bitp_packer_t inst;
   * val - value to pack;
  
   returns `BITP_OK` in case of success. It can return error if runtime checkings are enabled (see [Configuration](#configuration))


## Build

This project is a header-only library. 

If you use CMake, you can add it using `add_subdirectory` and link with `target_link_libraries` command. 

If you use another build system, just copy the include directory of the project.

## Configuration

There are several configuration options in the project. You can enable/disable them by 
adding following compile definitons:

* BITP_CHECK_BUFFER_BOUNDARY - runtime checking access violations in the preallocated
 buffer (e.g. buffer overflow)
* BITP_CHECK_PARAM - runtime checking the size of the output parameter (e.g. if you try decode 
16 bits in uint8_t)
* BITP_CHECK_RANGE - runtime checking the ability to pack value into a given number of bits 
(e.g. if you try to encode value 255 into 4 bit-integer).
* BITP_CHECK_ALL - enable all checkers.

It's assumed that checkers will be enabled in the debug build and disabled in the release build. 

## Restrictions

* ANSI C99 or later.
* Negative integers - two's complement only.


