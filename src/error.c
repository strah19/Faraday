#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static const char* error_msg[] = {
    "no error",
    "syntax error"
};

void fatal_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    fprintf(stderr, "\033[0;31mfatal error: \033[0m");
    vprintf(fmt, args);

    va_end(args);
    exit(EXIT_FAILURE);
}

void report_warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    fprintf(stdout, "\033[1;33mwarning: \033[0m");    
    vprintf(fmt, args);

    va_end(args);
}

void report_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    fprintf(stderr, "\033[0;31merror: \033[0m");    
    vprintf(fmt, args);

    va_end(args);
}

int error_count = 0;
void error(ErrorCode code) {
    report_error("%s.\n", error_msg[code]);
    error_count++;
}