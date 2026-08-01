#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define history_file "conversions.bin"

// conversion type identifiers
typedef enum {
    C_TO_F, F_TO_C, KM_TO_MI, MI_TO_KM,
    KG_TO_LB, LB_TO_KG, CM_TO_IN, IN_TO_CM,
    CONV_COUNT
} ConvType;

// our conversion record structure
typedef struct {
    ConvType type;
    double input;
    double result;
} Record;

// function pointer type for a conversion function
typedef double (*ConvFunc)(double);

// dynamic history array with count and capacity
extern Record *history;
extern int hist_count;
extern int hist_capacity;
extern int result_precision;

extern ConvFunc conversions[CONV_COUNT];
extern const char *conv_names[CONV_COUNT];

// conversion functions
double c_to_f(double c);
double f_to_c(double f);
double km_to_mi(double km);
double mi_to_km(double mi);
double kg_to_lb(double kg);
double lb_to_kg(double lb);
double cm_to_in(double cm);
double in_to_cm(double inch);

// helper functions
void clearing_buffer();
int add_record(ConvType t, double input, double result);

// history / file i/o
void view_history();
void save_history(const char *filename);
void load_history(const char *filename);

// callbacks and search/sort
void set_precision_callback(void);
void filter_records();
int compare_by_type(const void *a, const void *b);
int compare_by_result(const void *a, const void *b);
void sort_records();
void search_records();

// core functions
void perform_conversion();
void menu();

#endif
