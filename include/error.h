#ifndef error_h
#define error_h

typedef enum {
    NO_ERROR, SYNTAX_ERROR
} ErrorCode;

void fatal_error(const char* fmt, ...);

void report_warning(const char* fmt, ...);

void report_error(const char* fmt, ...);

void error(ErrorCode code);

#endif