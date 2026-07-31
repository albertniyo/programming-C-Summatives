#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define book_file "books.bin"

// our book structure
typedef struct{
    int id;
    char title[50];
    char author[30];
    char category[20];
    int copies;
} Book;


// dynamic book array with books count
extern Book *books;
extern int book_counts;


// functions prototypes
void load_books_from_file();
void save_books_to_file();
int find_book_by_id(int id);
int check_id_duplicates(int id);
void clearing_buffer();

void add_book();
void display_books();
void delete_book();
void update_book();

void search_by_id();
void search_by_title();

int compare_by_id(const void *a, const void *b);
int compare_by_title(const void *a, const void *b);
int compare_by_copies(const void *a, const void *b);
void sort_books(int (*com)(const void *a, const void *b), const char *descri);

void inventory_reporting();
void menu();

#endif
