#ifndef FILE_SYS_H
#define FILE_SYS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

// structure to pass file name and receive results for each thread
typedef struct {
    char *filename;          // input file name
    int lines;               // result: number of lines
    int words;               // result: number of words
    int chars;               // result: number of characters
    int status;              // 0 = success, -1 = error
} FileStats;

// thread function: analyze one file
void *process_file(void *arg);

#endif
