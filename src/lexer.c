#include "lexer.h"

/* Globals */

char ch; //Current character
Literal literal; 
TokenCode token; //Current token

typedef enum {
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE
} CharacterCode;

typedef struct {
    char* string;
    TokenCode code;
} ReservedWords;