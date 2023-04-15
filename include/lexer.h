#ifndef lexer_h
#define lexer_h

#include "common.h"

typedef enum {
    // Single character tokens
    T_PLUS, T_MINUS, T_STAR, T_SLASH, T_LPAR, T_RPAR, T_LCURLY, T_RCURLY,
    T_SEMICOLON, T_COLON, T_EXCLAMATION, T_AMPERSAND, T_POUND, T_EQUAL,
    T_LARROW, T_RARROW, T_LINE, T_LBRACKET, T_RBRACKET, T_PERCENT, T_NOT,
    T_CARET, T_COMMA, T_PERIOD,

    // Multi-character tokens
    T_NOT_EQUAL, T_LTE, T_GTE, T_COMPARE_EQUAL, T_LSHIFT, T_RSHIFT, T_COLON_EQUAL,
    T_PLUS_EQUAL, T_MINUS_EQUAL, T_STAR_EQUAL, T_SLASH_EQUAL, T_POINTER_ARROW, 
    T_ARGS,

    // Keywords
    T_IF, T_ELIF, T_ELSE, T_FOR, T_WHILE, T_BREAK, T_RETURN, T_INT, T_BOOLEAN,
    T_CHAR, T_STRING, T_FLOAT, T_AND, T_OR, T_TRUE, T_FALSE, T_PRINT,
    T_CAST,

    T_IDENTIFIER, T_INT_CONST, T_FLOAT_CONST, T_BINARY_CONST, T_STRING_CONST, T_CHAR_CONST,
    T_EOF, T_NO_TOKEN
} TokenCode;

typedef struct {
    int size;
    int line;
    char* start;
    char* str;

    TokenCode code;
} Token;

void init_lexer(char* filename);

void reset_lexer();

Token scan();

void run_lexer();

FILE* get_lexer_fp();

void destroy_lexer();

#endif