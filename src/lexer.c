#include "lexer.h"
#include "error.h"

/* Globals */
FILE* source_fp = NULL;
int current_line = 0;

char* program_buffer = NULL;
char* start = NULL;
char* current = NULL;

void open_source_file(char* filename);
void load_program_buffer();
int get_file_size();

Token scan();

void close_source_file();

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

    } while(token.code != T_EOF);
}

Token scan() {

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
    free(program_buffer);
}

void close_source_file() {
    fclose(source_fp);
}