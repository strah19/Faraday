#include "common.h"
#include <time.h>

#define MAX_LINES_PER_PAGE 50
#define FORMAT_LENGTH 9

/* Globals */
FILE* source = NULL;

//Buffers
char date[DATE_STR_LENGTH];
char source_name[MAX_FILE_NAME_LENGTH];
char source_buffer[MAX_LINE_LENGTH];
char print_buffer[MAX_LINE_LENGTH + FORMAT_LENGTH];

int line_number = 0;
int page_line_number = 50;
int page_number = 0;
int nesting_level = 0;

void open_source_file(char* name) {
    strcpy(source_name, name);
    source = fopen(source_name, "r");
    if (!source) {
        fprintf(stderr, "Unable to open source file '%s'.\n", source_name);
        exit(EXIT_FAILURE);
    }  
}

void set_time() {
    time_t timer;
    time(&timer);
    strcpy(date, asctime(localtime(&timer)));
}

void init_lister(char* name) {
    open_source_file(name);
    set_time();
}

bool new_header() {
    return (++page_line_number > MAX_LINES_PER_PAGE);
}

void print_header() {
    page_line_number = 1;
    page_number++;
    printf("Page %d   %s  %s", page_number, source_name, date);   
}

void header() {
    if (new_header())
        print_header();
}

void list() {
    while (fgets(source_buffer, MAX_LINE_LENGTH, source) != NULL) {
        line_number++;

        header();

        // Makes sure source buffer is not too big, otherwise breaks it up into different lines
        char* save_chp = source_buffer; //Max save_chp = MAX_PRINT_LINE - FORMAT_LENGTH
        char save_ch;
        save_ch = save_chp[MAX_PRINT_LINE - FORMAT_LENGTH];
        save_chp[MAX_PRINT_LINE - FORMAT_LENGTH] = '\0';
        int inner_lines = 0;

        do {
            if (inner_lines > 0)
                printf("\n");
            sprintf(print_buffer, "%4d %d: %s", line_number, nesting_level, save_chp);
            printf("%s", print_buffer);

            save_chp = save_chp + MAX_PRINT_LINE - FORMAT_LENGTH; 
            *save_chp = save_ch;

            save_ch = save_chp[MAX_PRINT_LINE - FORMAT_LENGTH];
            save_chp[MAX_PRINT_LINE - FORMAT_LENGTH] = '\0';
            inner_lines++;
        } while(save_chp < source_buffer + strlen(source_buffer));   
    } 
}

void close_source_file() {
    fclose(source);
}

int main(int argc, char* argv[]) {
    init_lister(argv[1]);

    list();

    printf("\n");
    close_source_file();

    return 0;
}