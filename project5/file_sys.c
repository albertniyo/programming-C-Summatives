#include "file_sys.h"

// thread function: analyze one file
void *process_file(void *arg){
    FileStats *stats = (FileStats *)arg;
    stats->lines = 0;
    stats->words = 0;
    stats->chars = 0;
    stats->status = 0;

    FILE *fp = fopen(stats->filename, "r");
    if (!fp) {
        fprintf(stderr, "Thread [%lu]: Error opening '%s'\n",
                (unsigned long)pthread_self(), stats->filename);
        stats->status = -1;
        pthread_exit(NULL);
    }

    // count lines, words and characters
    // lines: number of '\n', plus 1 if file is not empty and does not end with '\n'
    int in_word = 0;
    int c;
    int prev = 0;
    while ((c = fgetc(fp)) != EOF) {
        stats->chars++;
        if (c == '\n') stats->lines++;
        if (isspace(c)) in_word = 0;
        else if (!in_word) { in_word = 1; stats->words++; }
        prev = c;
    }
    if (stats->chars > 0 && prev != '\n') {
        stats->lines++;
    }
    fclose(fp);

    // write results to output file
    char out_filename[256];
    snprintf(out_filename, sizeof(out_filename), "%s_stats.txt", stats->filename);
    FILE *out = fopen(out_filename, "w");
    if (out) {
        fprintf(out, "File: %s\n", stats->filename);
        fprintf(out, "Lines: %d\n", stats->lines);
        fprintf(out, "Words: %d\n", stats->words);
        fprintf(out, "Characters: %d\n", stats->chars);
        fclose(out);
    } else {
        fprintf(stderr, "Thread [%lu]: Could not write output for '%s'\n",
                (unsigned long)pthread_self(), stats->filename);
    }

    printf("Thread [%lu] finished '%s': lines=%d, words=%d, chars=%d\n",
           (unsigned long)pthread_self(), stats->filename,
           stats->lines, stats->words, stats->chars);
    pthread_exit(NULL);
}
