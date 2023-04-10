#ifndef lexer_h
#define lexer_h

#include "common.h"

char* symbol_table[] = {
    "undefined", "identifier", "number", "string", 
    "*"
};

typedef enum {
    UNDEFINED, IDENTIFIER, NUMBER, STRING, 
    STAR
} TokenCode;

#endif