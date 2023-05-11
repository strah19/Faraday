#include "common.h"
#include "lexer.h"
#include "error.h"
#include "symbol_table.h"
#include "utility.h"

FILE* output_file = NULL;

void write_symbol_table(Symbol* root);

int main(int argc, char* argv[]) {
    init_lexer(argv[1]);
    
    Symbol* root = NULL;

    int token_count = 0;
    int symbol_index = 0;
    Token token;
    do {
        token = scan();

        if (token.code == T_IDENTIFIER || token.code == T_INT_CONST || token.code == T_FLOAT_CONST || token.code == T_STRING_CONST) {
            char* token_str = create_string(token.start, token.size);
            Symbol* node = search_symbol_table(token_str, root);       
            if (node == NULL) {
                node = enter_symbol(token_str, &root);
                node->defn.info.constant.val.integer = symbol_index++;
            }
            token_count++;
        }
        token_count++;
    } while(token.code != T_EOF);

    reset_lexer();

    int byte_index = 0;
    char* byte_code = alloc_array(char, token_count);
    do {
        token = scan(); 
        byte_code[byte_index++] = token.code;
        if (token.code == T_IDENTIFIER || token.code == T_INT_CONST || token.code == T_FLOAT_CONST || token.code == T_STRING_CONST) {
            char* token_str = create_string(token.start, token.size);
            Symbol* node = search_symbol_table(token_str, root);       
            if (node == NULL) {
                printf("Unidentified symbol '%s' found in second pass.\n", token_str);
            }
            dealloc(token_str);
            byte_code[byte_index++] = node->defn.info.constant.val.integer;
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
    fwrite(&symbol_index, sizeof(short), 1, output_file);
    write_symbol_table(root);
    fwrite(byte_code, sizeof(char) * token_count, 1, output_file);

    printf("lexer finished with %d error%s.\n", get_error_count(), ((get_error_count() == 1) ? "" : "s"));

    fclose(output_file);
    destroy_lexer();
    free_symbol_table(root);
    return 0;
}

void write_symbol_table(Symbol* root) {
    if (root == NULL) return;

    write_symbol_table(root->left);

    int len = strlen(root->name) + 1;
    fwrite(&len, sizeof(char), 1, output_file);
    fwrite(root->name, len, 1, output_file);
    fwrite(&root->defn.info.constant.val.integer, sizeof(char), 1, output_file);

    write_symbol_table(root->right);
}