#include "utility.h"
#include <time.h>

static const char* benchmark_name;
static clock_t start_time;

static char date[DATE_STR_LENGTH];
static time_t timer;

void begin_benchmark(const char* name) {
    benchmark_name = name;
    printf("Beginning '%s' benchmark.\n", name);
    start_time = clock();
}

void stop_benchmark() {
  double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
  printf("Benchmark '%s' done in %f seconds\n", benchmark_name, elapsed_time);
} 

char* create_string(char* start, int size) {
    char* str = alloc_string(size + 1);
    memset(str, '\0', size + 1);
    strncpy(str, start, size);
}

char* get_date() {
    time(&timer);
    strcpy(date, asctime(localtime(&timer)));
    return date;
}

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

int getline(char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}