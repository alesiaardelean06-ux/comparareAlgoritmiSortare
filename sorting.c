#include "sorting.h"
#include <stdlib.h>
#include <stdio.h>

int time_limit_exceeded(clock_t start, double time_limit_seconds)
{
    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    return elapsed >= time_limit_seconds;
}

int insertion_sort(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;
    int aux;
    for(size_t i=1; i<n; i++)
    {
        /* verificare timp la inceputul fiecarei iteratii */
        if(time_limit_exceeded(start, time_limit))
            return 0;

        aux=v[i];
        size_t j=i;
        while(j>0 && aux<v[j-1])
        {
            v[j]=v[j-1];
            j--;
        }
        v[j]=aux;
    }
    return 1;
}
int selection_sort(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;
    size_t k;
    int aux;
    for(size_t i=0; i<n-1; i++)
    {
        /* verificare timp la inceputul fiecarei iteratii */
        if(time_limit_exceeded(start, time_limit))
            return 0;

        k=i;
        for(size_t j=i+1; j<n; j++)
            if(v[k]>v[j])
                k=j;
        if(k!=i)
        {
            aux=v[i];
            v[i]=v[k];
            v[k]=aux;
        }
    }
    return 1;
}
int bubble_sort(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;
    int aux;
    for(size_t i=n-1; i>0; i--)
    {
        /* verificare timp la inceputul fiecarei treceri */
        if(time_limit_exceeded(start, time_limit))
            return 0;

        for(size_t j=0; j<i; j++)
        {
            if(v[j]>v[j+1])
            {
                aux=v[j];
                v[j]=v[j+1];
                v[j+1]=aux;
            }
        }
    }
    return 1;
}
static void interclasare(int *v, size_t s, size_t m, size_t d)
{
    size_t i=s, j=m+1, k=0;
    int *t=(int*)malloc((d-s+1)*sizeof(int));
    if(t==NULL)
    {
        printf("Eroare la alocarea memoriei.\n");
        exit(1);
    }
    while(i<=m && j<=d)
    {
        if(v[i]<=v[j])
            t[k++]=v[i++];
        else
            t[k++]=v[j++];
    }
    while(i<=m)
        t[k++]=v[i++];
    while(j<=d)
        t[k++]=v[j++];
    for(i=0; i<k; i++)
        v[s+i]=t[i];
    free(t);
}
int merge_sort(int *v, size_t s, size_t d, clock_t start, double time_limit)
{
    if(s<d)
    {
        /* verificare timp inainte de a continua recursivitatea */
        if(time_limit_exceeded(start, time_limit))
            return 0;

        size_t m=s+(d-s)/2;

        if(!merge_sort(v, s, m, start, time_limit))
            return 0;
        if(!merge_sort(v, m+1, d, start, time_limit))
            return 0;

        interclasare(v, s, m, d);
    }
    return 1;
}
static int pivot_partition(int *v, size_t s, size_t d, size_t *q,
                           clock_t start, double time_limit)
{
    int piv=v[d], aux;
    size_t i=s;
    for(size_t j=s; j<d; j++)
    {
        /* verificare timp in bucla de partitionare */
        if(time_limit_exceeded(start, time_limit))
            return 0;

        if(v[j]<=piv)
        {
            aux=v[i];
            v[i]=v[j];
            v[j]=aux;
            i++;
        }
    }
    aux=v[i];
    v[i]=v[d];
    v[d]=aux;
    *q=i;
    return 1;
}
int quicksort(int *v, size_t s, size_t d, clock_t start, double time_limit)
{
    if(s<d)
    {
        /* verificare timp inainte de partitionare */
        if(time_limit_exceeded(start, time_limit))
            return 0;

        size_t q;
        if(!pivot_partition(v, s, d, &q, start, time_limit))
            return 0;

        if(q>0)
        {
            if(!quicksort(v, s, q-1, start, time_limit))
                return 0;
        }
        if(!quicksort(v, q+1, d, start, time_limit))
            return 0;
    }
    return 1;
}
int shell_sort(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;

    size_t gaps[]={701,301,132,57,23,10,4,1};
    size_t nrGaps=8;
    int aux;

    for(size_t g=0; g<nrGaps; g++)
    {
        size_t gap=gaps[g];
        if(gap>=n)
            continue;

        for(size_t i=gap; i<n; i++)
        {
            /* verificare timp la fiecare element procesat */
            if(time_limit_exceeded(start, time_limit))
                return 0;

            aux=v[i];
            size_t j=i;
            while(j>=gap && v[j-gap]>aux)
            {
                v[j]=v[j-gap];
                j-=gap;
            }
            v[j]=aux;
        }
    }
    return 1;
}

/* ------------------------------------------------------------------ *
 *  Count Sort                                                          *
 *  Verificare timp: dupa calculul maximului si dupa umplerea vectorului*
 * ------------------------------------------------------------------ */
static int maxim(int *v, size_t n)
{
    int max=v[0];
    for(size_t i=1; i<n; i++)
        if(v[i]>max)
            max=v[i];
    return max;
}

int count_sort(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;

    /* verificare timp inainte de alocare */
    if(time_limit_exceeded(start, time_limit))
        return 0;

    int max=maxim(v, n);
    int *f=(int*)calloc(max+1, sizeof(int));
    if(f==NULL)
    {
        printf("Eroare la alocarea memoriei.\n");
        exit(1);
    }

    for(size_t i=0; i<n; i++)
        f[v[i]]++;

    /* verificare timp inainte de reconstructia vectorului */
    if(time_limit_exceeded(start, time_limit))
    {
        free(f);
        return 0;
    }

    size_t k=0;
    for(int i=0; i<=max; i++)
    {
        while(f[i]>0)
        {
            v[k++]=i;
            f[i]--;
        }
    }

    free(f);
    return 1;
}

/* ------------------------------------------------------------------ *
 *  Cocktail Shaker Sort                                               *
 *  Verificare timp: la inceputul fiecarui ciclu while                 *
 * ------------------------------------------------------------------ */
int cocktail_shaker_sort(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;

    size_t st=0, dr=n-1;
    int sortat=0, aux;

    while(!sortat)
    {
        /* verificare timp la inceputul fiecarui ciclu */
        if(time_limit_exceeded(start, time_limit))
            return 0;

        sortat=1;
        for(size_t i=st; i<dr; i++)
            if(v[i]>v[i+1])
            {
                aux=v[i];
                v[i]=v[i+1];
                v[i+1]=aux;
                sortat=0;
            }

        if(sortat)
            break;

        sortat=1;
        dr--;
        for(size_t i=dr; i>st; i--)
            if(v[i-1]>v[i])
            {
                aux=v[i-1];
                v[i-1]=v[i];
                v[i]=aux;
                sortat=0;
            }
        st++;
    }
    return 1;
}

/* ------------------------------------------------------------------ *
 *  Gnome Sort                                                          *
 *  Verificare timp: la fiecare pas al buclei principale               *
 * ------------------------------------------------------------------ */
int gnome_sort(int *v, size_t n, clock_t start, double time_limit)
{
    if(v==NULL || n<=1)
        return 1;

    size_t i=1;
    int aux;

    while(i<n)
    {
        /* verificare timp la fiecare pas */
        if(time_limit_exceeded(start, time_limit))
            return 0;

        if(i==0 || v[i-1]<=v[i])
            i++;
        else
        {
            aux=v[i];
            v[i]=v[i-1];
            v[i-1]=aux;
            i--;
        }
    }
    return 1;
}
