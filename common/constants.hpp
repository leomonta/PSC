#pragma once

#define DEFAULT_PORT 61970

// position of data in the header as an array
#define POS_VERSION_MAJOR 0
#define POS_VERSION_MINOR 1
#define POS_METHOD        2
#define POS_VARIANT       4
#define POS_UUID          32
#define POS_TIME          64
#define POS_DATA_START    96

// Method values meaning
#define METHOD_GET   0
#define METHOD_POST  1
#define VARIANT_TEXT 0
#define VARIANT_BIN  1
