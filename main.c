#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

#define SAVE_EXAMPLE_INPUTS 1

typedef void (*SortFunction)(int *, size_t);

typedef enum
{
    DATA_RANDOM,
    DATA_SORTED,
    DATA_REVERSED,
    DATA_ALMOST_SORTED,
    DATA_FLAT

} DataType;

typedef struct
{
    const char *name;
    SortFunction sort;
    size_t max_n;

} Algorithm;

void merge_sort_apelare(int *v, size_t n)
{
    if(v==NULL || n<=1)
        return;
    merge_sort(v, 0, n-1);
}

void quicksort_apelare(int *v, size_t n)
{
    if(v==NULL || n<=1)
        return;
    quicksort(v, 0, n-1);
}

void swap_int(int *a, int *b)
{
    int aux=*a;
    *a=*b;
    *b=aux;
}

int is_sorted(const int *v, size_t n)
{
    for(size_t i=1; i<n; i++)
        if(v[i-1]>v[i])
            return 0;
    return 1;
}

char* data_type_name(DataType type)
{
    switch(type)
    {
        case DATA_RANDOM:        
            return "random";
        case DATA_SORTED:
            return "sorted";
        case DATA_REVERSED:
            return "reversed";
        case DATA_ALMOST_SORTED:
             return "almost_sorted";
        case DATA_FLAT:          
            return "flat";
        default:                 
            return "unknown";
    }
}

int value_limit(size_t n)
{
    size_t limit=10*n+1;
    if(limit>1000000)
        limit=1000000;
    return(int)limit;
}

size_t repetitions_for_size(size_t n)
{
    if(n<=10) 
        return 200000;
    if(n<=20)
        return 100000;
    if(n<=50)      
        return 50000;
    if(n<=100)     
        return 20000;
    if(n<=1000)    
        return 2000;
    if(n<=10000) 
        return 50;
    if(n<=100000)  
        return 10;
    return 1;
}

void fill_random(int *v, size_t n)
{
    int limit=value_limit(n);
    for(size_t i=0; i<n; i++)
        v[i]=rand()%limit;
}

void fill_sorted(int *v, size_t n)
{
    for(size_t i=0; i<n; i++)
        v[i]=(int)i;
}

void fill_reversed(int *v, size_t n)
{
    for(size_t i=0; i<n; i++)
        v[i]=(int)(n-1-i);
}

void fill_almost_sorted(int *v, size_t n)
{
    fill_sorted(v, n);
    size_t swaps=n/20;
    if(swaps==0)
        swaps=1;
    for(size_t k=0; k<swaps; k++)
    {
        size_t i=rand()%n;
        size_t j=rand()%n;
        swap_int(&v[i], &v[j]);
    }
}

void fill_flat(int *v, size_t n)
{
    for(size_t i=0; i<n; i++)
        v[i]=rand()%5;
}

void generate_data(int *v, size_t n, DataType type)
{
    switch(type)
    {
        case DATA_RANDOM:
            fill_random(v, n);
            break;
        case DATA_SORTED:
            fill_sorted(v, n);
            break;
        case DATA_REVERSED:
            fill_reversed(v, n);
            break;
        case DATA_ALMOST_SORTED:
            fill_almost_sorted(v, n);
            break;
        case DATA_FLAT:
            fill_flat(v, n);
            break;
        default:
            fill_random(v, n);
    }
}
#if SAVE_EXAMPLE_INPUTS
void save(const int *v, size_t n, DataType type)
{
    char file_name[128];
    snprintf(file_name, sizeof(file_name), "data_%s_%zu.txt", data_type_name(type), n);

    FILE *f=fopen(file_name,"w");  
    if (f==NULL)
        return;
    fprintf(f, "%zu\n", n);
    for(size_t i=0; i<n; i++)
        fprintf(f, "%d ", v[i]);
    fprintf(f, "\n");
    fclose(f);
}
#endif
double benchmark(SortFunction sort_function, int *copie_tab_original, const int *tab_original, size_t n, size_t reps)
{
    size_t total_elements = n*reps;
    for(size_t i=0; i<total_elements; i++)
        copie_tab_original[i]=tab_original[i];
    clock_t start=clock();
    for(size_t rep=0; rep<reps; rep++)
        sort_function(copie_tab_original+rep*n, n);
    clock_t finish=clock();
    for(size_t rep=0; rep<reps; rep++)
        if(!is_sorted(copie_tab_original+rep*n, n))
        {
            printf("Eroare: lista nu a fost sortata corect.\n");
            exit(1);
        }
    return (double)(finish-start)/CLOCKS_PER_SEC;
}
int main(void)
{
    srand(42);
    const size_t sizes[] = {10, 20, 50, 100, 1000, 10000, 100000, 1000000, 5000000, 10000000};
    const size_t size_count=sizeof(sizes)/sizeof(sizes[0]);
    Algorithm algorithms[]=
    {
        {"Insertion Sort", insertion_sort, 20000},
        {"Selection Sort", selection_sort, 20000},
        {"Bubble Sort", bubble_sort, 20000},
        {"Merge Sort", merge_sort_apelare, 10000000},
        {"Quick Sort", quicksort_apelare, 1000000},
        {"Shell Sort", shell_sort, 10000000},
        {"Count Sort", count_sort, 10000000},
        {"Cocktail Shaker Sort", cocktail_shaker_sort, 20000},
        {"Gnome Sort", gnome_sort, 20000}
    };
    const size_t algorithm_count=sizeof(algorithms)/sizeof(algorithms[0]);
    FILE *out=fopen("results.txt", "w");
    if(out==NULL)
    {
        printf("Nu s-a putut deschide fisierul\n");
        return 1;
    }
    fprintf(out, "SORTING BENCHMARK RESULTS\n");
    for(int t=DATA_RANDOM; t<=DATA_FLAT; t++)
    {
        for(size_t s=0; s<size_count; s++)
        {
            size_t n=sizes[s], reps=repetitions_for_size(n), total_elements=n*reps;
            int *tab_original=(int*)malloc(total_elements*sizeof(int));
            int *copie_tab_original=(int*)malloc(total_elements*sizeof(int));
            if(tab_original==NULL || copie_tab_original==NULL)
            {
                printf("Eroare la alocarea memoriei.\n");
                free(tab_original);
                free(copie_tab_original);
                fclose(out);
                return 1;
            }
            printf("Testing: type=%s, n=%zu, repetitions=%zu\n", data_type_name((DataType)t), n, reps);
            for(size_t rep=0; rep<reps; rep++)
                generate_data(tab_original+rep*n, n, (DataType)t);

            #if SAVE_EXAMPLE_INPUTS
            save_array_to_file(tab_original, n, (DataType)t);
            #endif
            fprintf(out,"Data set: %s | n = %zu | repetitions = %zu\n", data_type_name((DataType)t), n, reps);
            fprintf(out,"algorithm | total_time_in_seconds | average_time_in_seconds | repetitions\n");
            for(size_t a=0; a<algorithm_count; a++)
            {   if(n>algorithms[a].max_n)
                {
                    fprintf(out,"%s | skipped | skipped | 0\n", algorithms[a].name);
                    continue;
                }
                printf("  -> %s...\n", algorithms[a].name);
                double total_time=benchmark(algorithms[a].sort, copie_tab_original, tab_original, n, reps);
                double average_time=total_time/reps;
                fprintf(out, "%s | %.9f | %.12e | %zu\n",algorithms[a].name, total_time, average_time,reps);
            }
            fprintf(out, "\n");
            free(tab_original);
            free(copie_tab_original);
        }
    }
    fclose(out);
    return 0;
}