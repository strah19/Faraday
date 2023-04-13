#include "common.h"
#include "lexer.h"
#include "error.h"
#include "symbol_table.h"

FILE* output_file = NULL;

void write_symbol_table(Symbol* root);

int main (int argc, char* argv[]) {
    init_lexer(argv[1]);
    
    Symbol* root = NULL;

    Token token;
    do {
        token = scan();

        if (token.code == T_IDENTIFIER || token.code == T_INT_CONST || token.code == T_FLOAT_CONST || token.code == T_STRING_CONST) {
            char* id_name = alloc_string(token.size + 1);
            memset(id_name, '\0', token.size + 1);
            strncpy(id_name, token.start, token.size);
            Symbol* node = search_symbol_table(id_name, root);       
            if (node == NULL) {
                node = enter_symbol(id_name, &root);
            }
        }
    } while(token.code != T_EOF);

    char output_file_name[MAX_FILE_NAME_LENGTH + 4];
    strcpy(output_file_name, argv[1]);
    strcat(output_file_name, ".cru");
    output_file = fopen(output_file_name, "w");
    if (output_file_name == NULL) {
        fprintf(stderr, "Failed to open file '%s'.\n", output_file_name);
        exit(EXIT_FAILURE);
    }
    write_symbol_table(root);

    printf("lexer finished with %d error%s.\n", get_error_count(), ((get_error_count() == 1) ? "" : "s"));

    fclose(output_file);
    destroy_lexer();
    free_symbol_table(root);
    return 0;
}

int symbol_index = 0;
void write_symbol_table(Symbol* root) {
    if (root == NULL) return;

    write_symbol_table(root->left);

    fwrite(root->name, strlen(root->name) + 1, 1, output_file);
    fwrite(&symbol_index, sizeof(int), 1, output_file);
    symbol_index++;

    write_symbol_table(root->right);
}