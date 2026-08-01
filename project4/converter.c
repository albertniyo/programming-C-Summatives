#include "converter.h"
#include <math.h>

// dynamic history array with count and capacity
Record *history = NULL;
int hist_count = 0;
int hist_capacity = 0;
int result_precision = 4;

// conversion functions
double c_to_f(double c){
    // convert celsius to fahrenheit

    return c * 9.0 / 5.0 + 32;
}
double f_to_c(double f){
    // convert fahrenheit to celsius

    return (f - 32) * 5.0 / 9.0;
}
double km_to_mi(double km){
    // convert kilometres to miles

    return km * 0.621371;
}
double mi_to_km(double mi){
    // convert miles to kilometres

    return mi / 0.621371;
}
double kg_to_lb(double kg){
    // convert kilograms to pounds

    return kg * 2.20462;
}
double lb_to_kg(double lb){
    // convert pounds to kilograms

    return lb / 2.20462;
}
double cm_to_in(double cm){
    // convert centimeters to inches

    return cm / 2.54;
}
double in_to_cm(double inch){
    // convert inches to centimeters
    return inch * 2.54;
}

// array of conversion functions indexed by ConvType (function pointers)
ConvFunc conversions[CONV_COUNT] = {
    c_to_f, f_to_c, km_to_mi, mi_to_km,
    kg_to_lb, lb_to_kg, cm_to_in, in_to_cm
};

// human-readable conversion names
const char *conv_names[CONV_COUNT] = {
    "Celsius -> Fahrenheit", "Fahrenheit -> Celsius",
    "Kilometres -> Miles",   "Miles -> Kilometres",
    "Kilograms -> Pounds",   "Pounds -> Kilograms",
    "Centimetres -> Inches", "Inches -> Centimetres"
};

// helper functions
void clearing_buffer(){
    // clearing the input buffer

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int add_record(ConvType t, double input, double result){
    // add a conversion into history, grow array when needed

    if (hist_count >= hist_capacity) {
        int new_cap = hist_capacity == 0 ? 10 : hist_capacity * 2;
        Record *temp = (Record *)realloc(history, new_cap * sizeof(Record));
        if (!temp) {
            puts("memory allocation failed");
            return 0;
        }
        history = temp;
        hist_capacity = new_cap;
    }

    history[hist_count].type = t;
    history[hist_count].input = input;
    history[hist_count].result = result;
    hist_count++;
    return 1;
}

// history / file i/o functions
void view_history(){
    // display conversion history

    if (hist_count == 0) {
        puts("no conversion history");
        return;
    }

    puts("\n------ Conversion History ------");
    for (int i = 0; i < hist_count; i++) {
        printf("%d. %s: %.2f -> %.*f\n", i + 1,
               conv_names[history[i].type],
               history[i].input, result_precision, history[i].result);
    }
}

void save_history(const char *filename){
    // saving history to binary file

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("error opening file %s\n", filename);
        return;
    }

    // fwrite returns item count, not bytes
    if (fwrite(&hist_count, sizeof(int), 1, fp) != 1 ||
        fwrite(history, sizeof(Record), hist_count, fp) != (size_t)hist_count) {
        printf("error writing history to file %s\n", filename);
    } else {
        printf("saved %d record(s) to %s\n", hist_count, filename);
    }

    fclose(fp);
}

void load_history(const char *filename){
    // loading history stored in binary file

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("no saved history found in %s\n", filename);
        return;
    }

    int count;
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        printf("error reading record count from %s\n", filename);
        fclose(fp);
        return;
    }

    free(history);
    history = (Record *)malloc(count * sizeof(Record));
    if (!history) {
        puts("memory allocation failed");
        hist_count = 0;
        hist_capacity = 0;
        fclose(fp);
        return;
    }

    // fread returns item count, not bytes
    if (fread(history, sizeof(Record), count, fp) != (size_t)count) {
        puts("error reading records from file");
        free(history);
        history = NULL;
        hist_count = 0;
        hist_capacity = 0;
        fclose(fp);
        return;
    }

    hist_count = count;
    hist_capacity = count;
    fclose(fp);
    printf("loaded %d record(s) from %s\n", count, filename);
}

// callbacks and search/sort
void set_precision_callback(void){
    // ask user for precision, then round all stored results

    int precision;
    printf("enter precision (decimal places): ");
    if (scanf("%d", &precision) != 1) {
        clearing_buffer();
        puts("invalid input");
        return;
    }
    clearing_buffer();

    if (precision < 0) {
        precision = 0;
    }

    result_precision = precision;
    double factor = pow(10.0, precision);
    for (int i = 0; i < hist_count; i++) {
        history[i].result = round(history[i].result * factor) / factor;
    }
    printf("All results rounded to %d decimal place(s)\n", precision);
}

void filter_records(){
    // filter history by conversion type or result value range

    int opt;
    puts("Filter by:");
    puts("1. Conversion type");
    puts("2. Result value range");
    printf("choice: ");

    if (scanf("%d", &opt) != 1) {
        clearing_buffer();
        puts("invalid input");
        return;
    }
    clearing_buffer();

    if (opt == 1) {
        int t;
        printf("Enter conversion type index (0-%d): ", CONV_COUNT - 1);
        if (scanf("%d", &t) != 1 || t < 0 || t >= CONV_COUNT) {
            clearing_buffer();
            puts("invalid type");
            return;
        }
        clearing_buffer();

        printf("Records of type '%s':\n", conv_names[t]);
        int found = 0;
        for (int i = 0; i < hist_count; i++) {
            if (history[i].type == (ConvType)t) {
                printf("  %.2f -> %.*f\n", history[i].input, result_precision, history[i].result);
                found = 1;
            }
        }
        if (!found) {
            puts("no records found");
        }
    } else if (opt == 2) {
        double low, high;
        printf("Enter minimum result value: ");
        if (scanf("%lf", &low) != 1) {
            clearing_buffer();
            puts("invalid number");
            return;
        }
        printf("Enter maximum result value: ");
        if (scanf("%lf", &high) != 1) {
            clearing_buffer();
            puts("invalid number");
            return;
        }
        clearing_buffer();

        printf("results in range [%.2f, %.2f]:\n", low, high);
        int found = 0;
        for (int i = 0; i < hist_count; i++) {
            if (history[i].result >= low && history[i].result <= high) {
                printf("  %s: %.2f -> %.*f\n", conv_names[history[i].type],
                       history[i].input, result_precision, history[i].result);
                found = 1;
            }
        }
        if (!found) {
            puts("no results found");
        }
    } else {
        puts("invalid filter choice");
    }
}

int compare_by_type(const void *a, const void *b){
    // comparing two records by conversion type

    return ((Record *)a)->type - ((Record *)b)->type;
}

int compare_by_result(const void *a, const void *b){
    // comparing two records by converted value

    double diff = ((Record *)a)->result - ((Record *)b)->result;
    return (diff > 0) - (diff < 0);
}

void sort_records(){
    // sort history using qsort callbacks

    if (hist_count == 0) {
        puts("no conversion history");
        return;
    }

    int opt;
    puts("Sort by:");
    puts("1. Conversion type");
    puts("2. Converted value");
    printf("choice: ");

    if (scanf("%d", &opt) != 1) {
        clearing_buffer();
        puts("invalid input");
        return;
    }
    clearing_buffer();

    if (opt == 1) {
        qsort(history, hist_count, sizeof(Record), compare_by_type);
        puts("sorted by conversion type");
    } else if (opt == 2) {
        qsort(history, hist_count, sizeof(Record), compare_by_result);
        puts("sorted by converted value");
    } else {
        puts("invalid choice");
    }
}

void search_records(){
    // search history by type or exact converted value

    if (hist_count == 0) {
        puts("no conversion history");
        return;
    }

    int opt;
    puts("Search by:");
    puts("1. Conversion type");
    puts("2. Converted value");
    printf("choice: ");

    if (scanf("%d", &opt) != 1) {
        clearing_buffer();
        puts("invalid input");
        return;
    }
    clearing_buffer();

    if (opt == 1) {
        int t;
        printf("enter type index (0-%d): ", CONV_COUNT - 1);
        if (scanf("%d", &t) != 1 || t < 0 || t >= CONV_COUNT) {
            clearing_buffer();
            puts("invalid type");
            return;
        }
        clearing_buffer();

        printf("searching for type '%s':\n", conv_names[t]);
        int found = 0;
        for (int i = 0; i < hist_count; i++) {
            if (history[i].type == (ConvType)t) {
                printf("  %.2f -> %.*f\n", history[i].input, result_precision, history[i].result);
                found = 1;
            }
        }
        if (!found) {
            puts("no records found");
        }
    } else if (opt == 2) {
        double val;
        printf("enter converted value to search: ");
        if (scanf("%lf", &val) != 1) {
            clearing_buffer();
            puts("invalid number");
            return;
        }
        clearing_buffer();

        int found = 0;
        for (int i = 0; i < hist_count; i++) {
            if (fabs(history[i].result - val) < 1e-9) {
                printf("  %s: %.2f -> %.*f\n", conv_names[history[i].type],
                       history[i].input, result_precision, history[i].result);
                found = 1;
            }
        }
        if (!found) {
            puts("no record with that exact value");
        }
    } else {
        puts("invalid choice");
    }
}

// core functions
void perform_conversion(){
    // pick a conversion, run it through the function pointer table, save to history

    int choice;
    double input, result;
    ConvFunc func;

    puts("Available conversions:");
    for (int i = 0; i < CONV_COUNT; i++) {
        printf("%d. %s\n", i, conv_names[i]);
    }
    printf("Choice: ");

    if (scanf("%d", &choice) != 1 || choice < 0 || choice >= CONV_COUNT) {
        clearing_buffer();
        puts("Invalid choice");
        return;
    }
    clearing_buffer();

    printf("Enter value to convert: ");
    if (scanf("%lf", &input) != 1) {
        clearing_buffer();
        puts("Invalid number");
        return;
    }
    clearing_buffer();

    // use function pointer to compute result
    func = conversions[choice];
    result = func(input);
    printf("Result: %.4f\n", result);

    if (!add_record(choice, input, result)) {
        puts("Warning: history not updated");
    }
}

void menu(){
    // menu-based interface for navigation

    int choice;
    do {
        puts("\n------ Unit Conversion Toolkit ------");
        puts("1. Perform a conversion");
        puts("2. View conversion history");
        puts("3. Search records");
        puts("4. Sort records");
        puts("5. Apply callback operations");
        puts("6. Save history");
        puts("7. Load history");
        puts("8. Exit");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearing_buffer();
            puts("invalid input try again");
            continue;
        }
        clearing_buffer();

        switch (choice) {
            case 1:
                perform_conversion();
                break;
            case 2:
                view_history();
                break;
            case 3:
                search_records();
                break;
            case 4:
                sort_records();
                break;
            case 5: {
                int cb;
                puts("Callback operations:");
                puts("1. Set precision for all results");
                puts("2. Filter records");
                printf("Choice: ");

                if (scanf("%d", &cb) != 1) {
                    clearing_buffer();
                    puts("invalid input");
                    break;
                }
                clearing_buffer();

                if (cb == 1) {
                    set_precision_callback();
                } else if (cb == 2) {
                    filter_records();
                } else {
                    puts("invalid callback choice");
                }
                break;
            }
            case 6:
                save_history(history_file);
                break;
            case 7:
                load_history(history_file);
                break;
            case 8:
                save_history(history_file);
                free(history);
                history = NULL;
                hist_count = 0;
                hist_capacity = 0;
                puts("Exiting...\n");
                break;
            default:
                puts("Invalid choice try again");
        }
    } while (choice != 8);
}
