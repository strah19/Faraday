#include "common.h"
#include "error.h"
#include "lexer.h"
#include <time.h>

int main(int argc, char* argv[]) {
    printf("Faraday %d.%d\n", FARADAY_VERSION_MAJOR, FARADAY_VERSION_MINOR);

    clock_t begin = clock();
    init_lexer(argv[1]);

    run_lexer();

    destroy_lexer();
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Lexer took %fs.\n", time_spent);

    return 0;
}