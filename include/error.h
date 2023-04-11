#ifndef error_h
#define error_h

typedef enum {
    NO_ERROR, SYNTAX_ERROR, UNTERMINATING_STR, MAX_NESTED_COMMENTS,
    UNKNOWN_CHAR, DOUBLE_PERIOD,
} ErrorCode;

void fatal_error(const char* fmt, ...);

void report_warning(const char* fmt, ...);

void report_error(const char* fmt, ...);

void error(ErrorCode code, int line);

int get_error_count();

#endif