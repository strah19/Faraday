#ifndef common_h
#define common_h

#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define alloc_struct(type) (type*) malloc(sizeof(type))
#define alloc_array(type, size) (type*) malloc(sizeof(type) * size)
#define alloc_string(length) (char*) malloc(length)
#define dealloc(ptr) free(ptr)

#define MAX_LINE_LENGTH 512
#define MAX_PRINT_LINE 80
#define MAX_FILE_NAME_LENGTH 256
#define DATE_STR_LENGTH 32

#endif 