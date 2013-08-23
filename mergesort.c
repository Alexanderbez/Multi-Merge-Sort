/****************************
 * Alexander Bezobchuk      *
 * UID: 110463917           *
 * ID: Abez0829             *
 * Project 7                *
 ***************************/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <pthread.h>
#include "mergesort.h"

typedef struct{
    void *array;
    size_t num_elem;
    size_t elem_size;
    Compare_fn cmp_f;
} Merge_mem;

void *merge_th(void *);

static void merge(void *arr, size_t num_elem_a, size_t num_elem_b,
                  size_t elem_size, Compare_fn cmp);

void mergesort(void *arr, size_t num_elem, size_t elem_size, Compare_fn cmp) {
    size_t first_half_count, second_half_count;
    char *second_half_arr = arr;
    
    if (arr == NULL || cmp == NULL || num_elem <= 1)
        return;
    first_half_count = num_elem / 2;
    second_half_count = num_elem - first_half_count;
    second_half_arr += first_half_count * elem_size;
    
    mergesort(arr, first_half_count, elem_size, cmp);
    mergesort(second_half_arr, second_half_count, elem_size, cmp);
    
    merge(arr, first_half_count, second_half_count, elem_size, cmp);
}

static void merge(void *arr, size_t num_elem_a, size_t num_elem_b,
                  size_t elem_size, Compare_fn cmp)
{
    char *i_ptr, *j_ptr, *work_ptr;
    char *workspace;
    size_t total_elem = num_elem_a + num_elem_b;
    
    workspace = malloc(total_elem * elem_size);
    if (workspace == NULL)
        err(EX_OSERR, "error allocating workspace");
    
    i_ptr = arr;
    j_ptr = i_ptr + elem_size * num_elem_a;
    work_ptr = workspace;
    
    while (num_elem_a && num_elem_b) {
        if (cmp(i_ptr, j_ptr) < 0) {
            memmove(work_ptr, i_ptr, elem_size);
            i_ptr += elem_size;
            num_elem_a--;
        }
        else {
            memmove(work_ptr, j_ptr, elem_size);
            j_ptr += elem_size;
            num_elem_b--;
        }
        work_ptr += elem_size;
    }
    memmove(work_ptr, i_ptr, elem_size * num_elem_a);
    memmove(work_ptr, j_ptr, elem_size * num_elem_b);
    
    memmove(arr, workspace, elem_size * total_elem);
    free(workspace);
}

void *merge_th(void *merge_struct)
{
    Merge_mem *pars = (Merge_mem *) merge_struct;
    mergesort(pars->array, pars->num_elem, pars->elem_size, pars->cmp_f);
    return;
}

void mt_mergesort(void *arr, size_t num_elem, size_t elem_size, Compare_fn cmp,
                  int num_threads)
{
    pthread_t tids[4];
    Merge_mem merge_ptr[4];
    size_t first_half_count, second_half_count, third_half_count, 
    fourth_half_count;
    char *second_half_arr = arr, *third_half_arr = arr, *fourth_half_arr = arr;
	
	if (arr == NULL || cmp == NULL)
		return;
    
    /* Create one thread, and let that thread to all the work 
     * (i.e. no splitting) */
    
	if (num_threads == 1)
	{
        merge_ptr[0].array = arr;
        merge_ptr[0].elem_size = elem_size;
        merge_ptr[0].num_elem = num_elem;
        merge_ptr[0].cmp_f = cmp;
        
		pthread_create(&tids[0], NULL, merge_th, &merge_ptr[0]);
        pthread_join(tids[0], NULL);
        return;
	}
    
    /* Split array into two equal sub-arrays. Create two threads; sort threads.
     * Then merge */
    
    if (num_threads == 2)
    {
        first_half_count = num_elem / 2;
        second_half_count = num_elem - first_half_count;
        second_half_arr += first_half_count * elem_size;
        
        merge_ptr[0].array = arr;
        merge_ptr[0].elem_size = elem_size;
        merge_ptr[0].num_elem = first_half_count;
        merge_ptr[0].cmp_f = cmp;
        merge_ptr[1].array = second_half_arr;
        merge_ptr[1].elem_size = elem_size;
        merge_ptr[1].num_elem = second_half_count;
        merge_ptr[1].cmp_f = cmp;
        
        pthread_create(&tids[0], NULL, merge_th, &merge_ptr[0]);
        pthread_create(&tids[1], NULL, merge_th, &merge_ptr[1]);
        pthread_join(tids[0], NULL);
        pthread_join(tids[1], NULL);
        merge(arr, first_half_count, second_half_count, elem_size, cmp);
        return;
    }
    
    else 
        if (num_threads == 4)
        {
            /* Split into first two sub-arrays */
            
            first_half_count = num_elem/4;
            second_half_count = (num_elem/2) - first_half_count;
            second_half_arr += first_half_count * elem_size;
            
            merge_ptr[0].array = arr;
            merge_ptr[0].elem_size = elem_size;
            merge_ptr[0].num_elem = first_half_count;
            merge_ptr[0].cmp_f = cmp;
            merge_ptr[1].array = second_half_arr;
            merge_ptr[1].elem_size = elem_size;
            merge_ptr[1].num_elem = second_half_count;
            merge_ptr[1].cmp_f = cmp;
            
            pthread_create(&tids[0], NULL, merge_th, &merge_ptr[0]);
            pthread_create(&tids[1], NULL, merge_th, &merge_ptr[1]);
            
            pthread_join(tids[0], NULL);
            pthread_join(tids[1], NULL);
            merge(arr, first_half_count, second_half_count, elem_size, cmp);
            
            /* Split into second two sub-arrarys */
            
            third_half_count = (num_elem - 
                                (first_half_count + second_half_count))/2;
            third_half_arr += (first_half_count + second_half_count) 
                                * elem_size;
            fourth_half_count = num_elem - (first_half_count + second_half_count
                                            + third_half_count);
            fourth_half_arr += (first_half_count + second_half_count + 
                                third_half_count) * elem_size;
            
            merge_ptr[2].array = third_half_arr;
            merge_ptr[2].elem_size = elem_size;
            merge_ptr[2].num_elem = third_half_count;
            merge_ptr[2].cmp_f = cmp;
            merge_ptr[3].array = fourth_half_arr;
            merge_ptr[3].elem_size = elem_size;
            merge_ptr[3].num_elem = fourth_half_count;
            merge_ptr[3].cmp_f = cmp;
            
            pthread_create(&tids[2], NULL, merge_th, &merge_ptr[2]);
            pthread_create(&tids[3], NULL, merge_th, &merge_ptr[3]);
            

            pthread_join(tids[2], NULL);
            pthread_join(tids[3], NULL);
            merge(third_half_arr, third_half_count, fourth_half_count, 
                  elem_size, cmp);
            
            /* Merge two arrays */
            first_half_count = num_elem/2;
            second_half_count = num_elem - first_half_count;
            merge(arr, first_half_count, second_half_count, elem_size, cmp);

        }
    /* Else num_thread was not 1,2, or 4 */
}
