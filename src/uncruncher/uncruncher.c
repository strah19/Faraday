#include "common.h"
#include "lexer.h"
#include "error.h"
#include "symbol_table.h"

#define INIT_SIZE 256
#define EOF_VAL 26

FILE* crunched_file = NULL;
int output_size = INIT_SIZE;
int current_size = 0;
char* output_buffer = NULL;
int need_space = 0;

FILE* uncrunched_file = NULL;

void realloc_output_buffer(int size);

int main(int argc, char* argv[]) {
    crunched_file = fopen(argv[1], "r");
    if (crunched_file == NULL) {
        fprintf(stderr, "Unable to open file '%s'.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int symbol_count;
    fread(&symbol_count, sizeof(short), 1, crunched_file);
    printf("File has %d symbols.\n", symbol_count);

    char** symbols = malloc(symbol_count * sizeof(char*));

    for (int i = 0; i < symbol_count; i++) {
        int id_len;
        fread(&id_len, sizeof(char), 1, crunched_file);
        char* id = alloc_string(id_len);
        fread(id, id_len, 1, crunched_file);
        int code;
        fread(&code, sizeof(char), 1, crunched_file);
        symbols[code] = id;
    }

    output_buffer = alloc_string(output_size);
    memset(output_buffer, '\0', output_size);
    short code;
    bool found_id = false;
    while((code = fgetc(crunched_file)) != T_EOF) {
        if (found_id) {
            found_id = false;
            int len = strlen(symbols[code]);
            realloc_output_buffer(len + 1);
            if (need_space == 2) {
                need_space = 0;
                current_size += 1;
                strcat(output_buffer, " ");
            }
            current_size += len;
            strcat(output_buffer, symbols[code]);
            continue;
        }
        if (code == T_IDENTIFIER || code == T_INT_CONST || code == T_FLOAT_CONST) {
            found_id = true;
            need_space += 1;
            continue;
        }

        int len = strlen(token_str[code]);
        realloc_output_buffer(len);
        strcat(output_buffer, token_str[code]);
        if (code < 37)
            need_space = 0;
        else
            need_space += 1;
        current_size += len;
    }
    output_buffer[current_size++] = EOF_VAL;

    char uncrunched_file_name[MAX_FILE_NAME_LENGTH];
    strncpy(uncrunched_file_name, argv[1], strlen(argv[1]) - 4);
    strcat(uncrunched_file_name, ".unc");

    uncrunched_file = fopen(uncrunched_file_name, "w");
    if (uncrunched_file == NULL) {
        fprintf(stderr, "Unable to open file '%s'.\n", uncrunched_file_name);
        exit(EXIT_FAILURE);
    }

    fwrite(output_buffer, current_size, 1, uncrunched_file);
    printf("Uncrunched '%s' into '%s'.\n", argv[1], uncrunched_file_name);

    for (int i = 0; i < symbol_count; i++) {
        free(symbols[i]);
    }
    free(symbols);
    free(output_buffer);
    fclose(uncrunched_file);
    fclose(crunched_file);

    return 0;
}

void realloc_output_buffer(int size) {
    if (output_size < current_size + size) {
        output_size *= 2;
        output_buffer = realloc(output_buffer, output_size);
    }   
}