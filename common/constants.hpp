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

// Method values meaning
#define METHOD_GET   0
#define METHOD_POST  1
#define VARIANT_TEXT 0
#define VARIANT_BIN  1

#define MAX_MESSAGE_LEN 1048576
