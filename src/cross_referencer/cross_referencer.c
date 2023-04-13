#include "common.h"
#include "error.h"
#include "lexer.h"
#include "utility.h"
#include <time.h>
#include "symbol_table.h"

typedef struct LineInfo LineInfo;
struct LineInfo {
    LineInfo* next;
    int line;
};

typedef struct {
    LineInfo* head;
    LineInfo* tail;
} LineHead;

void print_table(Symbol* root);

int main(int argc, char* argv[]) {
    init_lexer(argv[1]);

    Symbol* root = NULL;
    LineHead* line;

    Token token;
    do { 
        token = scan();
        if (token.code == T_IDENTIFIER) {
            char* id_name = alloc_string(token.size + 1);
            memset(id_name, '\0', token.size + 1);
            strncpy(id_name, token.start, token.size);
            Symbol* node = search_symbol_table(id_name, root);

            if (node == NULL) {
                node = enter_symbol(id_name, &root);
                line = alloc_struct(LineHead);
                line->head = line->tail = NULL;
                node->info = (char*) line;
            }

            LineInfo* line_info = alloc_struct(LineInfo);
            line_info->next = NULL;
            line_info->line = token.line;

            LineHead* head;
            head = (LineHead*) node->info;
            if (head->head == NULL)
                head->head = head->tail = line_info;
            else {
                head->tail->next = line_info;
                head->tail = line_info;
            }
        }
    } while(token.code != T_EOF);
    printf("lexer finished with %d error%s.\n", get_error_count(), ((get_error_count() == 1) ? "" : "s"));

    printf("\nCross Reference:\n");
    printf("---------------------");
    print_table(root);

    destroy_lexer();
    free_symbol_table(root);

    return 0;
}

void print_table(Symbol* root) {
    if (root == NULL) return;

    print_table(root->left);

    printf("\n%-16s   ", root->name);
    LineHead* head = (LineHead*) root->info;
    LineInfo* line_info;
    for (line_info = head->head; line_info != NULL; line_info = line_info->next) {
        printf("%4d", line_info->line);
    }
    printf("\n");

    print_table(root->right);
}