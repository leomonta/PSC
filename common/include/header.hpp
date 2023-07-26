#pragma once

#include <stdint.h>

// #define GET_PSC_VER_ALL(header) (char)(header.version >> 24)
// #define GET_PSC_VER_MAJ(header) (char)(header.version >> 28)
// #define GET_PSC_VER_MIN(header) ((char)(header.version >> 24) & 0b0000'1111)
// #define GET_PSC_MET_COD(header) ((char)(header.version >> 22) & 0b0000'0011)
// #define GET_PSC_MET_VAR(header) ((char)(header.version >> 21) & 0b0000'0001)
// #define GET_PSC_LEN_GTH(header) (uint32_t)(header.version & 0b0000'0000'0001'1111'1111'1111'1111'1111)

typedef struct PSCheader {
	uint8_t  versionMajor;
	uint8_t  versionMinor;
	uint8_t  method;
	uint32_t bodyLength;
	uint32_t UUID;
	uint32_t timestamp;
	char    *data;
} PSCheader;

/**
 * Parse msg as a PSC Header and saves the values in head
 *
 * @param msg the message to parse it must have a size of at least HEADER_LEN
 * @param head the struct where to put the parsed data
 */
void disassembleHeader(const uint8_t *msg, PSCheader &head);

/**
 * fills the given string with the data from header
 *
 * @param msg the location where to put the data, it should already be at least TOT_HEADER_LEN long
 * @param head the header where to take the data from
 */
void assembleHeader(uint8_t *msg, const PSCheader &head);