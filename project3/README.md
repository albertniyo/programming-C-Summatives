# Library Management System

A console library inventory manager written in C. It store books in a dynamic array and keep them in a binary file (`books.bin`) so data stay between runs. You can add, delete, update and search books, sort the list with callbacks, and print a short inventory report from a menu.

## Features

1. `Add Book`: enter ID, title, author, category and copies (IDs must be unique)
2. `Delete Book`: remove a book by ID and shrink the array
3. `Display Books`; displays books in the array after being loaded from our file(`books.bin`)
4. `Update Book`: change title, author, category or copies for an existing ID
5. `Search by ID` / `Search by Title`: look up one book and print its details
6. `Sort Books`: sort by ID, title, or number of copies using `qsort` callbacks
7. `Inventory Reporting`: total books, total copies, and the book with the most copies
8. `File I/O functions`: load from `books.bin` on start; save writes count and records as binary

## How to build and run

```bash
gcc -Wall -pedantic -Wextra -Werror main.c library.c -o library
./library
```

Then pick options `1`–`11` from the menu. Option `11` exits the program after saving data into `books.bin`.

## Files

- `library.h` — book struct, extern globals, and function prototypes
- `library.c` — file I/O, CRUD, search, sort, report, and menu
- `main.c` — load books then start the menu

---
<div align="center">
<p> Build with ❤️</p>
</div>
