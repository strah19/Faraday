#ifndef utility_h
#define utility_h

#include "common.h"

#define MAX_BENCHMARKS 32

void begin_benchmark(const char* name);

void stop_benchmark();

char* create_string(char* start, int size);

char* get_date();

int getline(char *prmpt, char *buff, size_t sz);

#endif 