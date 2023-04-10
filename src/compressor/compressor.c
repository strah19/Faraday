#include "common.h"

#define COMPRESSED_FILE_EXT_LEN 4

FILE* source_file = NULL;

void open_source(char* name);
void close_source();

//TODO: Compressor could also get rid of comments...
int main(int argc, char* argv[]) {
    open_source(argv[1]);

    fseek(source_file, 0, SEEK_END);
    int size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);
    char* compressed_buffer = alloc_string(size);

    bool skip_blank = false;
    int ch;
    int index = 0;
    while ((ch = fgetc(source_file)) != EOF) {
        switch (ch) {
        case '\t': {
            continue;
            break;
        }
        case '\n': {
            continue;
            break;
        }
        case ' ': {
            if (skip_blank) continue;
            else {
                skip_blank = true;
                compressed_buffer[index] = ' ';
            }
            break;
        }
        default: {
            skip_blank = false;
            compressed_buffer[index] = ch;
            break;
        }
        }
        index++;
    }

    char compressed_file_name[MAX_FILE_NAME_LENGTH + COMPRESSED_FILE_EXT_LEN];
    strcpy(compressed_file_name, argv[1]);
    strcat(compressed_file_name, ".com");

    FILE* compressed_file = fopen(compressed_file_name, "w");
    if (!compressed_file) {
        fprintf(stderr, "Failed to open file '%s'.\n", compressed_file_name);
        exit(EXIT_FAILURE); 
    }
    fputs(compressed_buffer, compressed_file);

    fseek(compressed_file, 0, SEEK_END);
    int new_size = ftell(compressed_file);

    fclose(compressed_file);
    free(compressed_buffer);

    close_source();
    printf("Successfully compressed '%s' into '%s'.\n", argv[1], compressed_file_name);
    printf("Compressed file from %d to %d bytes.\n", size, new_size);
    return 0;
}

void open_source(char* name) {
    source_file = fopen(name, "r");
    if (!source_file) {
        fprintf(stderr, "Failed to open file '%s'.\n", name);
        exit(EXIT_FAILURE);
    }
}

void close_source() {
    fclose(source_file);
}