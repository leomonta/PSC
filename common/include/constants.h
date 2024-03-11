#pragma once

#define DEFAULT_PORT 61970

// starting position of data in the header
#define POS_VERSION_MAJOR 0x00
#define POS_VERSION_MINOR 0x04
#define POS_METHOD        0x08
#define POS_VARIANT       0x0A
#define POS_LENGTH        0x0B
#define POS_UUID          0x20
#define POS_TIME          0x30
#define POS_DATA_START    0x40

// Method values taken singularly
#define METHOD_GET   0b00
#define METHOD_POST  0b01
#define METHOD_PATCH 0b10
#define VARIANT_TEXT 0b0
#define VARIANT_BIN  0b1

// i guess constexpr char const works
static char const *const METHODS_STR[3] = {
    "METHOD_GET",
    "METHOD_POST",
    "METHOD_PATCH"};

static char const *const VARIANTS_STR[2] = {
    "VARIANT_TEXT",
    "VARIANT_BIN"};

#define MMV_GET_TEXT   0b000
#define MMV_GET_BIN    0b001
#define MMV_POST_TEXT  0b010
#define MMV_POST_BIN   0b011
#define MMV_PATCH_TEXT 0b110
#define MMV_PATCH_BIN  0b111

#define MAX_MESSAGE_LEN 2097151 // bytes
#define TOT_HEADER_LEN  12      // bytes

// misc
#define NO_ERR 0

#define MAX_UNAME_LEN 256
