#include "sorting.h"
#include <stdlib.h>
#include <stdio.h>

void insertion_sort(int *v, size_t n) 
{
    if(v==NULL || n<=1)
        return;
    int aux;
    for(size_t i=1; i<n; i++) 
    {
        aux=v[i];
        size_t j=i;
        while(j>0 && aux<v[j-1]){
            v[j]=v[j-1];
            j--;
        }
        v[j]=aux;
    }
}


void selection_sort(int *v, size_t n) 
{
    if(v==NULL || n<=1)
        return;
    size_t k;
    int aux;
    for(size_t i=0; i<n-1; i++) 
    {
        k=i; 
        for(size_t j=i+1; j<n; j++) 
            if (v[k]>v[j]) 
                k=j;
        if (k!=i) 
        {
            aux=v[i];
            v[i]=v[k];
            v[k]=aux;
        }
    }
}


void bubble_sort(int *v, size_t n) 
{
    if(v==NULL || n<=1)
        return;
    int aux;
    for(size_t i=n-1; i>0; i--) 
    {
        for(size_t j=0; j<i; j++) {
            if(v[j]>v[j+1]) {
                aux=v[j];
                v[j]=v[j+1];
                v[j+1]=aux;
            }
        }
    }
}


void interclasare(int *v, size_t s, size_t m, size_t d)
{
    size_t i=s, j=m+1, k=0;
    int *t=(int*)malloc((d-s+1)*sizeof(int));
    if(t==NULL)
    {
        printf("Eroare la alocarea memoriei.\n");
        exit(1);
    }
    while(i<=m && j<=d){
        if(v[i]<=v[j])
            t[++k]=v[i++];
        else
            t[++k]=v[j++];    
    }
    while(i<=m)
        t[++k]=v[i++];
    while(j<=d)
        t[++k]=v[j++];
    for(i=0; i<k; i++)
        v[s+i]=t[i];
    free(t);
}

void merge_sort(int *v, size_t s, size_t d)
{
    if(s<d)
    {
        size_t m=s+(d-s)/2;  
        merge_sort(v, s, m);
        merge_sort(v, m+1, d);
        interclasare(v, s, m, d);
    }
}


void pivot(int *v, size_t s, size_t d, size_t *q) 
{
    int pivot=v[d], aux;
    size_t i=s;
    for(size_t j=s; j<d; j++) 
        if(v[j]<=pivot) 
        {
            aux=v[i];
            v[i]=v[j];
            v[j]=aux;
            i++;
        }
    aux=v[i];
    v[i]=v[d];
    v[d]=aux;
    *q = i;
}

void quicksort(int *v, size_t s, size_t d) 
{
    if (s<d){
        size_t q;
        pivot(v, s, d, &q);
        if(q>0) quicksort(v, s, q-1);  
        quicksort(v, q+1, d);
    }
}


void shell_sort(int *v, size_t n)
{
    if(v==NULL || n<=1)
        return;
    size_t gaps[]={701, 301, 132, 57, 23, 10, 4, 1}, nrGaps=8;;
    int aux;
    for(size_t g=0; g<nrGaps; g++)
    {
        size_t gap=gaps[g];
        if (gap>=n)
            continue;
        for(size_t i=gap; i<n; i++)
        {
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
}

int maxim(int *v, size_t n)
{
    int max=v[0];
    for(size_t i=1; i<n; i++)
        if(v[i]>max)
            max=v[i];
    return max;
}

void count_sort(int *v, size_t n)
{
    if(v==NULL || n<=1)
        return;
    int max=maxim(v, n);
    int *f=(int*)calloc(max + 1, sizeof(int));
    if(f==NULL)
    {
        printf("Eroare la alocarea memoriei.\n");
        exit(1);
    }
    for(size_t i=0; i<n; i++)
        f[v[i]]++;
    size_t k=0;
    for(int i=0; i<=max; i++)
        while(f[i]>0)
        {
            v[k++]=i;
            f[i]--;
        }
    free(f);
}


void cocktail_shaker_sort(int *v, size_t n)
{
    if(v==NULL || n<=1)
        return;
    size_t st=0, dr=n-1;
    int sortat=0, aux;
    while(!sortat)
    {
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
                sortat = 0;
            }
        st++;
    }
}


void gnome_sort(int *v, size_t n)
{
    if(v==NULL || n<=1)
        return;
    size_t i=1;
    int aux;
    
    while(i<n)
    {
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
}
