#ifndef SORTING_H
#define SORTING_H
#include <stddef.h>
#include <time.h>
#define TIME_LIMIT_QUADRATIC_SECONDS 60.0 //Bubble, Selection, Insertion, Gnome, Cocktail
#define TIME_LIMIT_GENERAL_SECONDS 120.0 //Merge, Shell, Count 
#define TIME_LIMIT_QUICK_SECONDS 10.0 // Quick Sort 
int time_limit_exceeded(clock_t start, double time_limit_seconds);
int insertion_sort(int *v, size_t n, clock_t start, double time_limit);
int selection_sort(int *v, size_t n, clock_t start, double time_limit);
int bubble_sort(int *v, size_t n, clock_t start, double time_limit);
int merge_sort(int *v, size_t s, size_t d, clock_t start, double time_limit);
int quicksort(int *v, size_t s, size_t d, clock_t start, double time_limit);
int shell_sort(int *v, size_t n, clock_t start, double time_limit);
int count_sort(int *v, size_t n, clock_t start, double time_limit);
int cocktail_shaker_sort(int *v, size_t n, clock_t start, double time_limit);
int gnome_sort(int *v, size_t n, clock_t start, double time_limit);
#endif
