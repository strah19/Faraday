#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static const char* error_msg[] = {
    "no error",
    "syntax error",
    "unterminated string",
    "reached limit of nested comments",
    "unknown character found",
    "double period is not a valid token"
};

void fatal_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    fprintf(stderr, "fatal error: ");
    vprintf(fmt, args);

    va_end(args);
    exit(EXIT_FAILURE);
}

void report_warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    fprintf(stdout, "warning: ");    
    vprintf(fmt, args);

    va_end(args);
}

void report_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    fprintf(stderr, "error: ");    
    vprintf(fmt, args);

    va_end(args);
}

int error_count = 0;
void error(ErrorCode code) {
    report_error("%s.\n", error_msg[code]);
    error_count++;
}

int get_error_count() {
    return error_count;
}