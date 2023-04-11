#ifndef symbol_table_h
#define symbol_table_h

typedef union {
    int integer;
    float decimal;
    char* string;
} SymbolValue;

typedef struct {
    union {
        struct Constant {
            SymbolValue val;
        };

        
    } info; 
} SymbolDefinition;

typedef struct {
    struct Symbol* left;
    struct Symbol* right;
} Symbol;

#endif