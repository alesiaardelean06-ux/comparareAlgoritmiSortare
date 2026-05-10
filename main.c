#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include "sorting.h"

#define SAVE_EXAMPLE_INPUTS 0

/* Limitele de timp sunt definite in sorting.h*/
typedef int (*SortFunction)(int *, size_t, clock_t, double);

typedef enum
{
    DATA_RANDOM,
    DATA_SORTED,
    DATA_REVERSED,
    DATA_ALMOST_SORTED,
    DATA_FLAT
} DataType;

typedef enum
{
    COMPLEXITY_QUADRATIC,
    COMPLEXITY_GENERAL,
    COMPLEXITY_QUICK  /* Quick Sort are limita proprie */
} ComplexityType;

typedef struct
{
    const char   *name;
    SortFunction  sort;
    ComplexityType complexity;
} Algorithm;

typedef struct
{
    double total_time;
    double average_time;
    size_t completed_repetitions;
    int    time_limit_reached;
} BenchmarkResult;

static int merge_sort_wrapper(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;
    return merge_sort(v, 0, n-1, start, time_limit);
}

static int quicksort_wrapper(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;
    return quicksort(v, 0, n-1, start, time_limit);
}

void swap_int(int *a, int *b)
{
    int aux=*a;
    *a=*b;
    *b=aux;
}

void copy_array(int *dest, const int *src, size_t n)
{
    for(size_t i=0; i<n; i++)
        dest[i]=src[i];
}

int is_sorted(const int *v, size_t n)
{
    for(size_t i=1; i<n; i++)
        if(v[i-1]>v[i])
            return 0;
    return 1;
}

const char *data_type_name(DataType type)
{
    switch(type)
    {
        case DATA_RANDOM:        return "random";
        case DATA_SORTED:        return "sorted";
        case DATA_REVERSED:      return "reversed";
        case DATA_ALMOST_SORTED: return "almost_sorted";
        case DATA_FLAT:          return "flat";
        default:                 return "unknown";
    }
}

const char *complexity_name(ComplexityType complexity)
{
    switch(complexity)
    {
        case COMPLEXITY_QUADRATIC: return "quadratic";
        case COMPLEXITY_GENERAL:   return "general";
        case COMPLEXITY_QUICK:     return "quick";
        default:                   return "unknown";
    }
}

/* Returneaza limita de timp corespunzatoare complexitatii algoritmului */
double time_limit_for_algorithm(const Algorithm *algorithm)
{
    switch(algorithm->complexity)
    {
        case COMPLEXITY_QUADRATIC: return TIME_LIMIT_QUADRATIC_SECONDS;
        case COMPLEXITY_QUICK:     return TIME_LIMIT_QUICK_SECONDS;
        default:                   return TIME_LIMIT_GENERAL_SECONDS;
    }
}

int value_limit(size_t n)
{
    size_t limit=10*n+1;
    if(limit>1000000)
        limit=1000000;
    return (int)limit;
}

size_t repetitions_for_size(size_t n)
{
    if(n<=10)        return 200000;
    if(n<=20)        return 100000;
    if(n<=50)        return  50000;
    if(n<=100)       return  20000;
    if(n<=1000)      return   2000;
    if(n<=10000)     return     50;
    if(n<=100000)    return     10;
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
    if(swaps==0) swaps=1;
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
        case DATA_RANDOM:        fill_random(v, n);        break;
        case DATA_SORTED:        fill_sorted(v, n);        break;
        case DATA_REVERSED:      fill_reversed(v, n);      break;
        case DATA_ALMOST_SORTED: fill_almost_sorted(v, n); break;
        case DATA_FLAT:          fill_flat(v, n);          break;
        default:                 fill_random(v, n);        break;
    }
}

#if SAVE_EXAMPLE_INPUTS
void save_example_input(const int *v, size_t n, DataType type)
{
    char file_name[128];
    snprintf(file_name, sizeof(file_name), "data_%s_%zu.csv",
             data_type_name(type), n);
    FILE *f=fopen(file_name, "w");
    if(f==NULL) return;
    fprintf(f, "index,value\n");
    for(size_t i=0; i<n; i++)
        fprintf(f, "%zu,%d\n", i, v[i]);
    fclose(f);
}
#endif
                                 
                                                                       
 /*- Porneste cronometrul inainte de fiecare repetitie               
   - Transmite start + limita catre functia de sortare                
   - Daca functia returneaza 0 (limita depasita), opreste testul si marcheaza time_limit_reached = 1*/
BenchmarkResult benchmark_with_time_limit(
    SortFunction sort_function,
    int         *working_array,
    const int   *all_original_data,
    size_t       n,
    size_t       repetitions,
    double       time_limit_seconds)
{
    BenchmarkResult result;
    result.total_time= 0.0;
    result.average_time = 0.0;
    result.completed_repetitions =0;
    result.time_limit_reached= 0;

    for(size_t rep=0; rep<repetitions; rep++)
    {
        const int *current_original = all_original_data + rep * n;
        copy_array(working_array, current_original, n);
        clock_t start = clock();
        int finished = sort_function(working_array, n, start, time_limit_seconds);
        clock_t finish = clock();
        double elapsed = (double)(finish - start) / CLOCKS_PER_SEC;
        if(!finished)
        {
            result.total_time += elapsed;
            result.completed_repetitions++;
            result.time_limit_reached = 1;
            break; 
        }
        if(!is_sorted(working_array, n))
        {
            printf("Eroare: lista nu a fost sortata corect.\n");
            exit(1);
        }

        result.total_time += elapsed;
        result.completed_repetitions++;
        if(result.total_time >= time_limit_seconds)
        {
            result.time_limit_reached = 1;
            break;
        }
    }

    if(result.completed_repetitions)
        result.average_time = result.total_time / result.completed_repetitions;

    return result;
}

int main()
{
    srand(42);

    const size_t sizes[] = {10, 20, 50, 100, 1000, 10000, 100000,
                            1000000, 5000000, 10000000};
    const size_t size_count = sizeof(sizes) / sizeof(sizes[0]);

    Algorithm algorithms[] =
    {
        {"Insertion Sort",       insertion_sort,        COMPLEXITY_QUADRATIC},
        {"Selection Sort",       selection_sort,        COMPLEXITY_QUADRATIC},
        {"Bubble Sort",          bubble_sort,           COMPLEXITY_QUADRATIC},
        {"Merge Sort",           merge_sort_wrapper,    COMPLEXITY_GENERAL  },
        {"Quick Sort",           quicksort_wrapper,     COMPLEXITY_QUICK    },
        {"Shell Sort",           shell_sort,            COMPLEXITY_GENERAL  },
        {"Count Sort",           count_sort,            COMPLEXITY_GENERAL  },
        {"Cocktail Shaker Sort", cocktail_shaker_sort,  COMPLEXITY_QUADRATIC},
        {"Gnome Sort",           gnome_sort,            COMPLEXITY_QUADRATIC}
    };
    const size_t algorithm_count = sizeof(algorithms) / sizeof(algorithms[0]);
    FILE *out = fopen("results.csv", "w");
    if(out == NULL)
    {
        printf("Nu s-a putut deschide fisierul results.csv\n");
        return 1;
    }
    fprintf(out,
        "algorithm,data_distribution,n,"
        "repetitions_requested,repetitions_completed,"
        "complexity,status,"
        "time_limit_seconds,total_time_seconds,average_time_seconds\n");

    for(int t = DATA_RANDOM; t <= DATA_FLAT; t++)
    {
        for(size_t s = 0; s < size_count; s++)
        {
            size_t n = sizes[s];
            size_t repetitions = repetitions_for_size(n);
            const char *distribution = data_type_name((DataType)t);
            if(repetitions != 0 && n > SIZE_MAX / repetitions)
            {
                printf("Eroare: overflow la n=%zu, rep=%zu\n", n, repetitions);
                fclose(out);
                return 1;
            }
            size_t total_elements = n * repetitions;
            if(total_elements > SIZE_MAX / sizeof(int))
            {
                printf("Eroare: overflow la alocare\n");
                fclose(out);
                return 1;
            }
            int *all_original_data = (int *)malloc(total_elements * sizeof(int));
            int *working_array     = (int *)malloc(n * sizeof(int));
            if(all_original_data == NULL || working_array == NULL)
            {
                printf("Eroare la alocarea memoriei.\n");
                free(all_original_data);
                free(working_array);
                fclose(out);
                return 1;
            }
             printf("Testing: type=%s, n=%zu, repetitions=%zu\n",
                   distribution, n, repetitions);
            for(size_t rep = 0; rep < repetitions; rep++)
                generate_data(all_original_data + rep * n, n, (DataType)t);

#if SAVE_EXAMPLE_INPUTS
            save_example_input(all_original_data, n, (DataType)t);
#endif

            for(size_t a=0; a<algorithm_count; a++)
            {
                const Algorithm *algorithm= &algorithms[a];
                double time_limit_s =time_limit_for_algorithm(algorithm);
                printf("  -> %s...\n", algorithm->name);
                BenchmarkResult result = benchmark_with_time_limit(algorithm->sort working_array, all_original_data,n,repetitions,time_limit_s);
                const char *status = result.time_limit_reached ? "time_limit_reached": "ok";
                fprintf(out,"%s,%s,%zu,%zu,%zu,%s,%s,%.2f,%.9f,%.12e\n",algorithm->name,distribution,n,repetitions,result.completed_repetitions,complexity_name(algorithm->complexity),status,time_limit_s,result.total_time,result.average_time);
            }
            free(all_original_data);
            free(working_array);
        }
    }
    fclose(out);
    return 0;
}
