#ifndef __COMMON_HEADER_H
#define __COMMON_HEADER_H

#include <stdarg.h>

#if __GNUC__
#define NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif

NORETURN void die(const char * err, ...) __attribute__((format (printf, 1, 2)));

NORETURN void die_errno(const char * err, ...) __attribute__((format (printf, 1, 2)));

NORETURN void usage(const char * err, ...) __attribute__((format (printf, 1, 2)));

void err(const char *err, ...) __attribute__((format (printf, 1, 2)));

void err_errno(const char * err, ...) __attribute__((format (printf, 1, 2)));

void warning(const char *err, ...) __attribute__((format (printf, 1, 2)));

void warning_errno(const char * err, ...) __attribute__((format (printf, 1, 2)));

#endif
