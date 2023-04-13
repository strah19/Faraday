#ifndef symbol_table_h
#define symbol_table_h

typedef enum {
    UNDEFINED, CONST_DEFN, VAR_DEFN
} DefinitionKey;

typedef union {
    int integer;
    float decimal;
    char* string;
} SymbolValue;

//Other definitions would go here too like procedures and classes.
typedef struct {
    DefinitionKey key;
    union {
        struct {
            SymbolValue val;
        } constant;
    } info; 
} SymbolDefinition;

typedef struct Symbol Symbol;

struct Symbol {
    Symbol* left;
    Symbol* right;
    SymbolDefinition* defn;
    char* name;
    char* info;
};

Symbol* search_symbol_table(const char* name, Symbol* root);

Symbol* enter_symbol(char* name, Symbol** root);

void free_symbol_table(Symbol* root);

#endif