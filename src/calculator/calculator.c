#include "common.h"
#include "error.h"
#include "lexer.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

bool running = true;
char input[MAX_LINE_LENGTH];

int main(int argc, char* argv[]) {
    printf("Faraday Calculator %d.%d: %s", FARADAY_VERSION_MAJOR, FARADAY_VERSION_MINOR, get_date());

    while (running) {
        getline(">>> ", input, MAX_LINE_LENGTH);
        printf("%s\n", input);

        init_lexer_with_buffer(input);
        Token token;
        do {
            token = scan();
            print_token(token);
        } while (token.code != T_EOF);
    }

    
    printf("lexer finished with %d error%s.\n", get_error_count(), ((get_error_count() == 1) ? "" : "s"));
    destroy_lexer();

    return 0;
}