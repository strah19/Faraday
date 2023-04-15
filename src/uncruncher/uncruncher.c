#include "common.h"
#include "lexer.h"
#include "error.h"
#include "symbol_table.h"

FILE* crunched_file = NULL;

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
        fread(&id_len, sizeof(short), 1, crunched_file);
        char* id = alloc_string(id_len);
        fread(id, id_len, 1, crunched_file);
        int code;
        fread(&code, sizeof(short), 1, crunched_file);
        printf("Found symbol '%s' of id: %d.\n", id, code);
        symbols[code] = id;
    }

    short code;
    while((code = fgetc(crunched_file)) != EOF) {
        printf("%d\n", code);
    }

    fclose(crunched_file);

    return 0;
}