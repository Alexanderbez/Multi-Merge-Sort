/****************************
 * Alexander Bezobchuk      *
 * UID: 110463917           *
 * ID: Abez0829             *
 * Project 7                *
 ***************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <sys/resource.h> 
#include <unistd.h>
#include "mergesort.h"

/* A simple function to compare integers for our mergesort algorithm. It will
 * later be used to be pointed by Compare_fn */

int int_cmp(const void *a, const void *b)
{
    int first = *(int *)a, second = *(int *)b;
    
    return first - second;
}

/* The tv_delta() function would not work for some reason, possibly a header
 * file missing. I was getting a conversion error. I created this function that 
 * does the exact same thing tv_dela() does, but just renamed it. */

static struct timeval tv_diff(struct timeval start, struct timeval end) 
{ 
    struct timeval delta = end; 
    delta.tv_sec -= start.tv_sec; 
    delta.tv_usec -= start.tv_usec; 
    if (delta.tv_usec < 0) { 
        delta.tv_usec += 1000000; 
        delta.tv_sec--; 
    } 
    return delta; 
} 

int main(int argc, char *argv[])
{
	int *ARRAY, *ARRAY_2, *ARRAY_3, *ARRAY_4, i, k;
	int num = atoi(argv[1]), seed = atoi(argv[2]), m_bound = atoi(argv[3]);
	
	struct rusage start_ru, end_ru; 
  	struct timeval start_wall, end_wall, diff_wall, diff_usr, diff_sys;
    Compare_fn cmp = int_cmp;
	
	ARRAY = malloc(sizeof(int) * num);
    ARRAY_2 = malloc(sizeof(int) * num);
    ARRAY_3 = malloc(sizeof(int) * num);
    ARRAY_4 = malloc(sizeof(int) * num);
	
	/* Fill array with random integers */
	srand(seed);
	for (i = 0; i < num ; i ++)
    {
        k = rand() % m_bound;
		ARRAY[i] = k;
        ARRAY_2[i] = k;
        ARRAY_3[i] = k;
        ARRAY_4[i] = k;
    }
		
	/* SORT USING 0 THREADS (only main thread) */
	
	gettimeofday(&start_wall, NULL); /* Start wall time */
	getrusage(RUSAGE_SELF, &start_ru); /* Start sys and usr time */
	
	mergesort(ARRAY, num, sizeof(int), cmp);
    
	gettimeofday(&end_wall, NULL); /* End wall time */
	getrusage(RUSAGE_SELF,&end_ru); /* End sys and usr time */
    
    diff_wall = tv_diff(start_wall, end_wall);
    diff_usr = tv_diff(start_ru.ru_utime, end_ru.ru_utime);
    diff_sys = tv_diff(start_ru.ru_stime, end_ru.ru_stime);
    
    printf("0 threads:  ");
    printf("%d.%06ds wall;  ", diff_wall.tv_sec, diff_wall.tv_usec);
    printf("%d.%06ds user;  ", diff_usr.tv_sec, diff_usr.tv_usec);
    printf("%d.%06ds sys\n", diff_sys.tv_sec, diff_sys.tv_usec);
    
    
    /* SORT USING 1 THREAD */
    
	gettimeofday(&start_wall, NULL); /* Start wall time */
	getrusage(RUSAGE_SELF, &start_ru); /* Start sys and usr time */
	
	mt_mergesort(ARRAY_2, num, sizeof(int), cmp, 1);
    
	gettimeofday(&end_wall, NULL); /* End wall time */
	getrusage(RUSAGE_SELF,&end_ru); /* End sys and usr time */
    
    diff_wall = tv_diff(start_wall, end_wall);
    diff_usr = tv_diff(start_ru.ru_utime, end_ru.ru_utime);
    diff_sys = tv_diff(start_ru.ru_stime, end_ru.ru_stime);
    
    printf("1 threads:  ");
    printf("%d.%06ds wall;  ", diff_wall.tv_sec, diff_wall.tv_usec);
    printf("%d.%06ds user;  ", diff_usr.tv_sec, diff_usr.tv_usec);
    printf("%d.%06ds sys\n", diff_sys.tv_sec, diff_sys.tv_usec);
    
    /* SORT USING 2 THREADS */
    
    gettimeofday(&start_wall, NULL); /* Start wall time */
	getrusage(RUSAGE_SELF, &start_ru); /* Start sys and usr time */
	
	mt_mergesort(ARRAY_3, num, sizeof(int), cmp, 2);
    
	gettimeofday(&end_wall, NULL); /* End wall time */
	getrusage(RUSAGE_SELF,&end_ru); /* End sys and usr time */
    
    diff_wall = tv_diff(start_wall, end_wall);
    diff_usr = tv_diff(start_ru.ru_utime, end_ru.ru_utime);
    diff_sys = tv_diff(start_ru.ru_stime, end_ru.ru_stime);
    
    printf("2 threads:  ");
    printf("%d.%06ds wall;  ", diff_wall.tv_sec, diff_wall.tv_usec);
    printf("%d.%06ds user;  ", diff_usr.tv_sec, diff_usr.tv_usec);
    printf("%d.%06ds sys\n", diff_sys.tv_sec, diff_sys.tv_usec);
    
    /* SORT USING 4 THREADS */
    
    gettimeofday(&start_wall, NULL); /* Start wall time */
	getrusage(RUSAGE_SELF, &start_ru); /* Start sys and usr time */
	
	mt_mergesort(ARRAY_4, num, sizeof(int), cmp, 4);
    
	gettimeofday(&end_wall, NULL); /* End wall time */
	getrusage(RUSAGE_SELF,&end_ru); /* End sys and usr time */
    
    diff_wall = tv_diff(start_wall, end_wall);
    diff_usr = tv_diff(start_ru.ru_utime, end_ru.ru_utime);
    diff_sys = tv_diff(start_ru.ru_stime, end_ru.ru_stime);
    
    printf("4 threads:  ");
    printf("%d.%06ds wall;  ", diff_wall.tv_sec, diff_wall.tv_usec);
    printf("%d.%06ds user;  ", diff_usr.tv_sec, diff_usr.tv_usec);
    printf("%d.%06ds sys\n", diff_sys.tv_sec, diff_sys.tv_usec);
	
	return 0;
}