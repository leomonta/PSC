#pragma once

#include "header.hpp"

/**
 * Given a string at least TOT_HEADER_LEN long prints its bytes, 4 per line
 * 
 * @param head the header to print
 */
void printHeaderStr(const char *head);

/**
 * Given a Psc header struct pretty prints it
 * 
 * @param head the header to print
 */
void printHeaderStruct(const PSCheader *head);