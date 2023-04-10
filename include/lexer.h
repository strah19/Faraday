#ifndef lexer_h
#define lexer_h

#include "common.h"

const char* token_table[] = {
    "undefined", "identifier", "number", "string", 
    "*", "-", "+", "/", "=", "{", "}", "^", ";", 
    ":", ":=", "[", "]", "(", ")", ",", "<", ">", 
    "!", "!=", "&", "#", "<=", ">=", "==", " ", 
    "and", "or", "if", "elif", "else", "while", 
    "for", "func"
};

typedef enum {
    UNDEFINED, IDENTIFIER, NUMBER, STRING, 
    STAR, MINUS, PLUS, SLASH, EQUAL, LCURLY,
    RCURLY, 
} TokenCode;

typedef enum {
    INTEGER_LIT, FLOAT_LIT, STRING_LIT
} LiteralType;

typedef struct {
    union {
        int integer;
        float decimal;
        char string[MAX_LINE_LENGTH];
    } value;

    LiteralType type;
} Literal;

#endif