# Multi-threaded File Processing System

A console file analyzer written in C. It use POSIX threads to analyze multiple text files at the same time, counting lines, words, and characters for each file. Results are written to a `filename_stats.txt` file per input so you can check them after the run.

## Features

1. `Pass file names on the command line`: each argument is one text file to process
2. `One thread per file`: `pthread_create` starts a worker for every input
3. `Count lines, words, and characters`: each thread reads its file and fills a `FileStats` struct
4. `Save results per file`: write `File`, `Lines`, `Words`, and `Characters` into `<filename>_stats.txt`
5. `Join all threads`: wait until every worker finishes, then print a short summary

## How to build and run

```bash
gcc -Wall -pedantic -Wextra -Werror main.c file_sys.c -o filesys -pthread

./filesys test_files/test.bin test_files/text.txt
```

Give one or more file paths after the program name. Each finished file gets its own `*_stats.txt` next to that input name (for example `test_stats.txt`, `test.s_stats.txt`).

## Test files (`test_files/`)

Sample inputs used to try the program on different kinds of content:

| File | What it is |
|------|------------|
| `test.bin` | Empty file (0 bytes) — to see how an empty input is counted |
| `test.c` | Small C source (`Hello, World!`) used as plain text input |
| `test.s` | Assembly generated from `test.c` with `gcc -S test.c -o test.s` |
| `test` | Short jokes about C programming |

## Files

- `file_sys.h` — FileStats struct and `process_file` prototype
- `file_sys.c` — thread worker that counts and writes stats
- `main.c` — create threads from argv, join them, then free memory
- `test_files/` — sample inputs described above

---
<div align="center">
<p> Build with ❤️</p>
</div>
