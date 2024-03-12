#pragma once

#include "header.h"

#include <stddef.h>
#include <stdio.h>

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

/**
 * Given a string containing the hex representation of a 32bit integer un stringify it into the given uint32_t
 * 
 * @param strloc the string to unstringify
 * @param dest where to put the result
 * 
 * @return 0 if successfull, 1 otherwise
 */
int unStringifyHex(const unsigned char *strloc, uint32_t *dest);

/**
 * given a 32bit int creates a string of its hex representation
 *
 * the string MUST be already allocated with a size of at least 8 bytes, one for each bit quartet
 *
 * @param val the value to convert
 * @param strloc the string where to put the hex representation
 * @param lowercase if true uses lowercase letters, if false uses uppercase letters
 */
void stringifyHex(const uint32_t val, unsigned char *strloc, const bool lowercase);

/**
 * find a string null terminated string needle in the string haystack while making sure that not more than count characters are compared for haystack
 *
 * @param haystack where to search the other given string
 * @param needle the null terminated string to search for
 * @param count the maximum amount of character of haystack to check
 *
 * @return a pointer to the first character of the matches sequence or nullptr of count or no matches are found
 */
const char *strnstr(const char *haystack, const char *needle, const size_t count);

/**
 * Find the given string in the given file line by line
 * if found set line and pos to the line number and column of the first occurrence
 *
 * @param toSearch string to search in the file, must be null terminated
 * @param file the file to seach into
 * @param line line number of the found string
 * @param col column in the line of the found string
 *
 * @return true if the string was found in the file, false if not
 * if return false the value of line and col shound be discarded
 */
bool findInFile(const char *toSearch, FILE *file, size_t *line, size_t *col);
