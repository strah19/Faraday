#include "lexer.h"
#include "error.h"

#define NESTED_COMMENTS 32

/* Globals */
FILE* source_fp = NULL;
int current_line = 0;

char* program_buffer = NULL;
char* start = NULL;
char* current = NULL;

static void open_source_file(char* filename);
void load_program_buffer();
int get_file_size();

void remove_invalid_text();

bool is_digit(char ch);
bool is_letter(char ch);

Token new_token(TokenCode code);
Token number();
Token identifier();
TokenCode keywords();
Token single_character(char ch);
bool match(const char* keyword, int size);

Token new_string();
Token string();

static void close_source_file();

//Helper functions
char peek();
char peek_ahead();
char advance_by(int by);
char advance();
bool check(char expected);
bool is_eof();

void init_lexer(char* filename) {
    open_source_file(filename);
    load_program_buffer();

    start = current = program_buffer;
    current_line = 1;
}

void reset_lexer() {
    start = current = program_buffer;
    current_line = 1;   
}

FILE* get_lexer_fp() {
    return source_fp;
}

void open_source_file(char* filename) {
    source_fp = fopen(filename, "r");
    if (!source_fp) fatal_error("Unable to open source file '%s'.\n", filename);
}

void load_program_buffer() {
    int size = get_file_size();

    program_buffer = alloc_string(size + 1);
    int bytes_read = fread(program_buffer, sizeof(char), size, source_fp);
    program_buffer[bytes_read] = '\0';
}

int get_file_size() {
    fseek(source_fp, 0, SEEK_END);
    int size = ftell(source_fp);
    fseek(source_fp, 0, SEEK_SET); 
    return size;
}

void run_lexer() {
    Token token;
    do {
        token = scan();
        printf("%4d (%d): %.*s\n", token.line, token.code, token.size, token.start);
    } while(token.code != T_EOF);
}

Token scan() {
    remove_invalid_text();
    start = current;
    if (is_eof()) return new_token(T_EOF);
    char first = advance();

    if (is_digit(first)) return number();
    if (is_letter(first)) return identifier();
    return single_character(first);
}

void remove_invalid_text() {
    while (true) {
        char ch = peek();
        if (ch == '\t' || ch == ' ' || ch == '\r') {
            advance();
        }
        else if (ch == '\n') {
            current_line++;
            advance();
        }
        else if (ch == '/') {
            if (peek_ahead() != '/') return;
            while (peek() != '\n' && !is_eof()) advance();    
        }
        else if (ch == '<') {
            if (peek_ahead() != '/') return;
            int nested = 1;
            advance_by(2);
            while (!is_eof() && nested > 0) {
                if (peek() == '/' && peek_ahead() == '>')
                    nested--;
                else if (peek() == '<' && peek_ahead() == '/')
                    nested++;                              
                else if (peek() == '\n')
                    current_line++;      
                if (nested > NESTED_COMMENTS) {
                    error(MAX_NESTED_COMMENTS, current_line);
                    return;         
                }
                advance();
            }
            advance();
        }
        else return;
    }
}

bool is_digit(char ch) {
    return (ch >= '0' && ch <= '9');
}

bool is_letter(char ch) {
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_'));
}

Token new_token(TokenCode code) {
    Token token;
    token.code = code;
    token.line = current_line;
    token.start = start;
    token.size = (int) (current - start);
    
    token.str = alloc_string(token.size + 1);
    memset(token.str, '\0', token.size + 1);
    strncpy(token.str, token.start, token.size);

    return token;
}

Token number() {
    while (is_digit(peek()) && !is_eof()) 
        advance();
    
    if (peek() == '.' && is_digit(peek_ahead())) {
        advance();

        while (is_digit(peek()) && !is_eof()) 
            advance();
        return new_token(T_FLOAT_CONST);
    }
    return new_token(T_INT_CONST);
}

Token identifier() {
    while ((is_letter(peek()) || is_digit(peek())) && !is_eof()) 
        advance();
    return new_token(keywords());
}


TokenCode keywords() {
    switch (*start) {
    case 'i': return (match("f",      1) ? T_IF      : 
                     (match("nt",     2) ? T_INT     : T_IDENTIFIER));
    case 'e': return (match("lse",    3) ? T_ELSE    : 
                     (match("lif",    3) ? T_ELIF    : T_IDENTIFIER));
    case 's': return (match("tring",  5) ? T_STRING  : T_IDENTIFIER);
    case 'b': return (match("oolean", 6) ? T_BOOLEAN : 
                     (match("reak",   4) ? T_BREAK   : T_IDENTIFIER));
    case 'f': return (match("loat",   4) ? T_FLOAT   : 
                     (match("or",     2) ? T_FOR     : 
                     (match("alse",   4) ? T_FALSE   : T_IDENTIFIER)));
    case 'w': return (match("hile",   4) ? T_WHILE   : T_IDENTIFIER);
    case 'r': return (match("eturn",  5) ? T_RETURN  : T_IDENTIFIER);
    case 'c' :return (match("har",    3) ? T_CHAR    : 
                     (match("ast",    3) ? T_CAST    : T_IDENTIFIER));
    case 'a': return (match("nd",     2) ? T_AND     : T_IDENTIFIER);
    case 'o': return (match("r",      1) ? T_OR      : T_IDENTIFIER);
    case 't': return (match("rue",    3) ? T_TRUE    : T_IDENTIFIER);
    case 'p': return (match("rint",   4) ? T_PRINT   : T_IDENTIFIER);
    }

    return T_IDENTIFIER;
}

bool match(const char* keyword, int size) {
    if (size + 1 != current - start) return false;
    int i = 1;  // The index starts at one because we already verified the first character in the switch statement in 'keywords'.
    while ((start + i) < current) {
        if (start[i] != keyword[i - 1]) return false;
        i++;
    }
    return true;
}

Token single_character(char ch) {
    switch (ch) {
    case '(': return new_token(T_LPAR    );
    case ')': return new_token(T_RPAR    );
    case '{': return new_token(T_LCURLY  );
    case '}': return new_token(T_RCURLY  );
    case '=': return new_token((check('=')) ? T_COMPARE_EQUAL : T_EQUAL);
    case '!': return new_token((check('=')) ? T_NOT_EQUAL     : T_EXCLAMATION);
    case '+': return new_token((check('=')) ? T_PLUS_EQUAL    : T_PLUS);
    case '-': return new_token((check('=')) ? T_MINUS_EQUAL   : 
                                (check('>')) ? T_POINTER_ARROW : T_MINUS);
    case '*': return new_token((check('=')) ? T_STAR_EQUAL    : T_STAR);
    case '/': return new_token((check('=')) ? T_SLASH_EQUAL   : T_SLASH);
    case '|': return new_token(T_LINE    );
    case '&': return new_token(T_AMPERSAND);
    case '#': return new_token(T_POUND   );
    case ';': return new_token(T_SEMICOLON);
    case '<': return new_token((check('=')) ? T_LTE : check('<') ? T_LSHIFT : T_LARROW);
    case '>': return new_token((check('=')) ? T_GTE : check('>') ? T_RSHIFT : T_RARROW);
    case '[': return new_token(T_LBRACKET);
    case ']': return new_token(T_RBRACKET);
    case ':': return new_token((check('=')) ? T_COLON_EQUAL : T_COLON);
    case '%': return new_token(T_PERCENT );
    case '^': return new_token(T_CARET   );
    case '~': return new_token(T_NOT     );
    case ',': return new_token(T_COMMA   );
    case '"': return string();
    case '.': {
        if (check('.') && !check('.')) {
            error(DOUBLE_PERIOD, current_line);
            return new_token(T_NO_TOKEN);
        }
        if (check('.')) return new_token(T_ARGS); //This means that there are three dots, not just two
        else return new_token(T_PERIOD);
    }
    default:
        error(UNKNOWN_CHAR, current_line);
        return new_token(T_NO_TOKEN);
    }
}

Token string() {
    while (peek() != '"' && !is_eof()) {
        if (peek() == '\n') 
            current_line++;
        advance();
    }
    if (is_eof())
        error(UNTERMINATING_STR, current_line);
    advance();
    return new_string();
}

Token new_string() {
    Token token;

    token.code = T_STRING_CONST;
    token.line = current_line;
    token.start = start + 1;
    token.size = (int) (current - start - 2);

    token.str = alloc_string(token.size + 1);
    memset(token.str, '\0', token.size + 1);
    strncpy(token.str, token.start, token.size);

    return token;
}

char peek() {
    return (*current);
}

char peek_ahead() {
    if (is_eof()) return '\0';
    return current[1];
}

char advance() {
    current++;
    return current[-1];
}

char advance_by(int by) {
    char c;
    while (by > 0) {
        c = advance();
        by--;
    }
    return c;
}

bool check(char expected) {
    if (is_eof()) return false;
    if (peek() != expected) return false;
    advance();
    return true;
}

bool is_eof() {
    return (*current == '\0');
}

void destroy_lexer() {
    close_source_file();
    dealloc(program_buffer);
}

void close_source_file() {
    fclose(source_fp);
}