#include "common.h"
#include "lexer.h"

#define COMPRESSED_FILE_EXT_LEN 4

#define UNDEFINED_TOKEN 0
#define SPACE_TOKEN 1
#define CLOSE_TOKEN 2

//TODO: Compressor could also get rid of comments...
int main(int argc, char* argv[]) {
    init_lexer(argv[1]);

    FILE* fp = get_lexer_fp();
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* compressed_buffer = alloc_string(size);

    int previous_token_class = UNDEFINED_TOKEN;
    int current_token_class = UNDEFINED_TOKEN;
    int index = 0;

    Token token;
    do {
        token = scan();
        previous_token_class = current_token_class;

        if (token.code >= 37 && token.code != T_EOF) {
            current_token_class = SPACE_TOKEN;
        }
        else {
            current_token_class = CLOSE_TOKEN;
        }

        if (current_token_class == SPACE_TOKEN && previous_token_class == SPACE_TOKEN) {
            compressed_buffer[index++] = ' ';
        }
        for (int i = 0; i < token.size; i++) {
            compressed_buffer[index++] = token.start[i];
        }
    } while(token.code != T_EOF);

    destroy_lexer();

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
    dealloc(compressed_buffer);

    float fractional = (float) new_size / (float) size;

    printf("Successfully compressed '%s' into '%s'.\n", argv[1], compressed_file_name);
    printf("Compressed file from %d to %d bytes (%.2f%%).\n", size, new_size, 100 - (fractional * 100.0f));
    return 0;
}