#include "library.h"

// dynamic book array with books count
Book *books = NULL;
int book_counts = 0;

// file i/o functions
void load_books_from_file(){
    // loading books stored in binary file

    FILE *fp = fopen(book_file, "rb");
    if (!fp) {
        printf("Error opening file %s\n", book_file);
        return;
    }
    // check book counts from file to allocate their memory
    if (fread(&book_counts, sizeof(int), 1, fp) != 1) {
        printf("Error reading book count from file %s\n", book_file);
        fclose(fp);
        return;
    }
    // allocate memory for books
    books = (Book *)malloc(book_counts * sizeof(Book));
    if (!books) {
        puts("memory allocation failed");
        fclose(fp);
        return;
    }

    // reading records (fread returns item count, not bytes)
    if (fread(books, sizeof(Book), book_counts, fp) != (size_t)book_counts) {
        puts("Error reading books from file");
        free(books);
        books = NULL;
        book_counts = 0;
        fclose(fp);
        return;
    }

    fclose(fp);
    printf("loaded %d book(s) from %s \n", book_counts, book_file);
}

void save_books_to_file(){
    // saving books to binary file

    FILE *fp = fopen(book_file, "wb");
    if (!fp) {
        printf("error opening file %s\n", book_file);
        return;
    }
    // writing books
    if (fwrite(&book_counts, sizeof(int), 1, fp) != 1 ||
        fwrite(books, sizeof(Book), book_counts, fp) != (size_t)book_counts) {
        printf("error writing books to file %s\n", book_file);
    } else {
        printf("saved %d book(s) to %s \n", book_counts, book_file);
    }

    fclose(fp);
}

// helper functions
int find_book_by_id(int id){
    // finding books by ID in inventory

    for(int i = 0; i < book_counts; i++){
        if(books[i].id == id){
            return i;
        }
    }
    return -1;
}

int check_id_duplicates(int id){
    // checking for duplicates by ID in the inventory
    // using the find_bool_by_id failure

    return find_book_by_id(id) != -1;
}

void clearing_buffer(){
    // clearing the input buffer

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// core functions
void add_book(){
    // adds data for a new book

    Book new;
    printf("enter book ID: ");

    scanf("%d", &new.id);
    clearing_buffer();

    if(check_id_duplicates(new.id)){
        printf("Book with ID %d already exists\n", new.id);
        return;
    }

    // get book title, author, category and copies
    printf("enter book title: ");
    fgets(new.title, sizeof(new.title), stdin);

    new.title[strcspn(new.title, "\n")] = '\0';
    printf("enter book author: ");
    fgets(new.author, sizeof(new.author), stdin);

    new.author[strcspn(new.author, "\n")] = '\0';
    printf("enter book category: ");
    fgets(new.category, sizeof(new.category), stdin);

    new.category[strcspn(new.category, "\n")] = '\0';
    printf("enter number of copies: ");
    if (scanf("%d", &new.copies) != 1) {
        clearing_buffer();
        puts("Invalid number of copies");
        return;
    }
    clearing_buffer();
    if (new.copies < 0) {
        puts("Number of copies cannot be negative");
        return;
    }

    // allocate memory for the new book
    books = (Book *)realloc(books, (book_counts + 1) * sizeof(Book));
    if (!books) {
        puts("memory allocation failed");
        return;
    }

    // add the new book to the array
    books[book_counts] = new;
    book_counts++;

    puts("Book added successfully");
}

void display_books(){
    // display books inventory

    //check if inventory is not empty
    if (book_counts == 0) {
        puts("inventory is empty");
        return;
    }

    printf("Books in inventory:\n");
    for (int i = 0; i < book_counts; i++) {
        printf("ID - %d, Title: %s, Author: %s, Category: %s, Copies: %d\n",
               books[i].id, books[i].title, books[i].author, books[i].category, books[i].copies);
    }
}

void delete_book(){
    // deleting book by id

    display_books();

    int id, n;
    printf("enter book id to delete: ");
    scanf("%d", &id);
    clearing_buffer();

    n = find_book_by_id(id);
    if (n == -1) {
        printf("Book with ID %d not found\n", id);
        return;
    }

    // shift books to the left to remove the deleted book
    for (int i = n; i < book_counts - 1; i++) {
        books[i] = books[i + 1];
    }
    book_counts--;

    // reallocate memory for the reduced array as well
    books = (Book *)realloc(books, book_counts * sizeof(Book));
    if (!books) {
        puts("memory allocation failed");
        return;
    }

    puts("Book deleted successfully");

}

void update_book(){
    // updating books information by using id 

    int n, id;

    display_books();

    printf("enter book ID to update: ");
    if (scanf("%d", &id) != 1) {
        clearing_buffer();
        return;
    }
    clearing_buffer();

    n = find_book_by_id(id);
    if (n == -1) {
        printf("Book with ID %d not found\n", id);
        return;
    }

    puts("Current book:");
    printf("ID - %d, Title: %s, Author: %s, Category: %s, Copies: %d\n",
           books[n].id, books[n].title, books[n].author,
           books[n].category, books[n].copies);

    // get updated book information
    printf("enter updated book title: ");
    fgets(books[n].title, sizeof(books[n].title), stdin);
    books[n].title[strcspn(books[n].title, "\n")] = '\0';

    printf("enter updated book author: ");
    fgets(books[n].author, sizeof(books[n].author), stdin);
    books[n].author[strcspn(books[n].author, "\n")] = '\0';

    printf("enter updated book category: ");
    fgets(books[n].category, sizeof(books[n].category), stdin);
    books[n].category[strcspn(books[n].category, "\n")] = '\0';

    printf("enter updated number of copies: ");
    if (scanf("%d", &books[n].copies) != 1) {
        clearing_buffer();
        return;
    }
    clearing_buffer();

    puts("Book updated successfully.\n");
}

void search_by_id(){
    // searching a book by its ID

    int id, n;
    printf("enter book ID to search: ");
    if (scanf("%d", &id) != 1) { 
        clearing_buffer(); 
        return;
    }
    clearing_buffer();

    n = find_book_by_id(id);
    if (n != -1) {
        puts("searched book found: ");
        printf("ID - %d, Title: %s, Author: %s, Category: %s, Copies: %d\n",
               books[n].id, books[n].title, books[n].author, books[n].category, books[n].copies);
    } else {
        printf("searched book with ID %d not found\n", id);
    }

}

void search_by_title(){
    // searching a book by its title

    char t[50];

    printf("enter book title to search: ");
    fgets(t, sizeof(t), stdin);
    t[strcspn(t, "\n")] = '\0';
    
    for (int n = 0; n < book_counts; n++) {
        if (strcmp(books[n].title, t) == 0) {
            puts("searched book found: ");
            printf("ID - %d, Title: %s, Author: %s, Category: %s, Copies: %d\n",
                   books[n].id, books[n].title, books[n].author, books[n].category, books[n].copies);
            return;
        }
    }
    printf("searched book with title %s not found\n", t);
}

// sorting
int compare_by_id(const void *a, const void *b){
    // comparing two books by ID

    return ((Book *)a)->id - ((Book *)b)->id;
}
int compare_by_title(const void *a, const void *b){
    // comparing two books by title

    return strcmp(((Book *)a)->title, ((Book *)b)->title);
}
int compare_by_copies(const void *a, const void *b){
    // comparing two books by number of copies

    return ((Book *)a)->copies - ((Book *)b)->copies;
}
void sort_books(int (com)(const void *a, const void *b), const char *descri){
    // sort the books using callbacks

    // check inventory
    if (book_counts == 0) {
        puts("no books in inventory");
        return;
    }
    qsort(books, book_counts, sizeof(Book), com);
    printf("Books sorted by %s\n", descri);
}

void inventory_reporting(){
    // generating inventory report with total nbr of books,
    // total copies available, book with the highest nber or copies,
    // and nber of books in each category

    // check inventory first
    if (book_counts == 0) {
        puts("no books in inventory");
        return;
    }
    puts("------ Inventory Report ------");

    // calculate totals
    int total_books = book_counts, total_copies = 0;
    Book *highest_copies_book = &books[0];

    // calculate total copies and find book with highest copies
    for (int i = 0; i < book_counts; i++) {
        total_copies += books[i].copies;
        if (books[i].copies > highest_copies_book->copies) {
            highest_copies_book = &books[i];
        }
    }

    printf("Total Books: %d\n", total_books);
    printf("Total Copies Available: %d\n", total_copies);
    printf("Book with Highest Copies:\n");
    printf("ID - %d, Title: %s, Author: %s, Category: %s, Copies: %d\n",
           highest_copies_book->id, highest_copies_book->title, highest_copies_book->author, highest_copies_book->category, highest_copies_book->copies);

}

void menu(){
    // menu-based interface for navigation

    int choice;
    do {
        puts("\n----- Library Management System -----");
        puts("1. Add Book");
        puts("2. Delete Book");
        puts("3. Update Book");
        puts("4. Display Books");
        puts("5. Search Book by ID");
        puts("6. Search Book by Title");
        puts("7. Sort Books by ID");
        puts("8. Sort Books by Title");
        puts("9. Sort Books by Copies");
        puts("10. Inventory Reporting");
        puts("11. Exit");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearing_buffer();
            puts("Invalid input try again");
            continue;
        }
        clearing_buffer();

        switch (choice) {
            case 1:
                add_book();
                break;
            case 2:
                delete_book();
                break;
            case 3:
                update_book();
                break;
            case 4:
                display_books();
                break;
            case 5:
                search_by_id();
                break;
            case 6:
                search_by_title();
                break;
            case 7:
                sort_books(compare_by_id, "ID");
                break;
            case 8:
                sort_books(compare_by_title, "Title");
                break;
            case 9:
                sort_books(compare_by_copies, "Number of copies");
                break;
            case 10:
                inventory_reporting();
                break;
            case 11:
                save_books_to_file();
                puts("Exiting...\n");
                break;
            default:
                puts("Invalid choice. Please try again.");
        }
    } while (choice != 11);
}
