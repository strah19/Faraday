#include "symbol_table.h"
#include "common.h"

Symbol* search_symbol_table(const char* name, Symbol* root) {
    while (root != NULL) {
        int cmp = strcmp(name, root->name);
        if (cmp == 0) return root;
        root = (cmp < 0) ? root->left : root->right;
    }
    return NULL;
}

Symbol* enter_symbol(char* name, Symbol** root) {
    Symbol* new_node = alloc_struct(Symbol);
    new_node->name = name;
    new_node->left = new_node->right = NULL;
    new_node->defn = UNDEFINED;
    new_node->info = NULL;

    Symbol* test_node;
    while ((test_node = *root) != NULL) {
        int cmp = strcmp(name, test_node->name);
        root = (cmp < 0) ? &test_node->left : &test_node->right;
    }

    *root = new_node;
    return new_node;
}

void free_symbol_table(Symbol* root) {
    if (root == NULL) return;

    free_symbol_table(root->left);
    free_symbol_table(root->right);

    dealloc(root->name);
    dealloc(root);
}