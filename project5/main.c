#include "file_sys.h"

int main(int argc, char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    int num_files = argc - 1;
    pthread_t *threads = (pthread_t *)malloc(num_files * sizeof(pthread_t));
    FileStats *stats = (FileStats *)malloc(num_files * sizeof(FileStats));
    if (!threads || !stats) {
        fprintf(stderr, "memory allocation failed.\n");
        free(threads);
        free(stats);
        return 1;
    }

    // prepare stats and create threads
    for (int i = 0; i < num_files; i++) {
        stats[i].filename = argv[i + 1];
        if (pthread_create(&threads[i], NULL, process_file, &stats[i]) != 0) {
            fprintf(stderr, "error creating thread for '%s'\n", stats[i].filename);
            stats[i].status = -1;
            // continue creating other threads anyway
        }
    }

    // wait for all threads to finish
    for (int i = 0; i < num_files; i++) {
        pthread_join(threads[i], NULL);
    }

    puts("\nAll threads completed. Results saved to filename_stats.txt");

    free(threads);
    free(stats);
    return 0;
}
