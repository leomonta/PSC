#pragma once

#define LOG_ALL     0
#define LOG_DEBUG   1
#define LOG_INFO    2
#define LOG_WARNING 3
#define LOG_ERROR   4
#define LOG_FATAL   5

/**
 * print on stdout the mexage given interpreted as printf format with variadics
 * All messages are printed in this format
 * [Loglevel] [ Thread id ] message
 * 
 * the \n at the end is not automatically appended
 * the log level is color coded based on the severity
 * the Thread id is colored based on the number itself
 * 
 * @param logLevel the level of the message
 * @param the message / format of the message
 * @param variadic the data to be printed
 */
void llog(const char logLevel, const char *mex, ...);

/**
 * Set the minimun log level that a message need to be to be printed
 *
 * @param minLogLevel the minimun log level required for a message to be printed
 */
void setLogLevel(const char minLogLevel);
