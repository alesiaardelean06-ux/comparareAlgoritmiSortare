#ifndef SORTING_H
#define SORTING_H
#include <stddef.h>
void insertion_sort(int *v, size_t n);
void selection_sort(int *v, size_t n);
void bubble_sort(int *v, size_t n);
void merge_sort(int *v, size_t s, size_t d);
void quicksort(int *v, size_t s, size_t d);
void shell_sort(int *v, size_t n);
void count_sort(int *v, size_t n);
void cocktail_shaker_sort(int *v, size_t n);
void gnome_sort(int *v, size_t n);
#endif 
