#include "common.h"
#include "error.h"
#include "lexer.h"
#include "utility.h"
#include <time.h>

int main(int argc, char* argv[]) {
    printf("Faraday %d.%d\n", FARADAY_VERSION_MAJOR, FARADAY_VERSION_MINOR);

    begin_benchmark("Lexer");
    init_lexer(argv[1]);

    run_lexer();
    stop_benchmark();
    printf("lexer finished with %d error%s.\n", get_error_count(), ((get_error_count() == 1) ? "" : "s"));

    destroy_lexer();

    return 0;
}