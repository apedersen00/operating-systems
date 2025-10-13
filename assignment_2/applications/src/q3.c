#include <pthread.h> 
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/time.h>

#define ARR_SIZE 1000000

double arr[ARR_SIZE];
int num_threads;

typedef struct {
    int thread_id;
    double *partial_sums;
} thread_arg_t;

void *thread_func(void *arg);

/**
 * @brief Create an array of 1,000,000 random floats in [0, 1] and calculates the sum.
 *        Then creates a group of worker threads, each summing up their corresponding
 *        part of the array.
 */
int main(int argc, char *argv[])
{
    /* === Read user argument === */
    if (argc != 2)
    {
        return 1;
    }
    
    num_threads = atoi(argv[1]);
    if (num_threads <= 0)
    {
        return 1;
    }

    /* === Initialize array === */
    srand(time(NULL));
    for (int i = 0; i < ARR_SIZE; i++)
    {
        arr[i] = (double)rand() / (double)(RAND_MAX);
    }

    /* === Start timer === */
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long utime = 1000000 * tv.tv_sec + tv.tv_usec;

    /* === Compute serial sum === */
    double sum_serial = 0.0;
    for (int i = 0; i < ARR_SIZE; i++)
    {
        sum_serial += arr[i];
    }

    /* === End timer and print results === */
    gettimeofday(&tv, NULL);
    unsigned long total_time = 1000000 * tv.tv_sec + tv.tv_usec - utime;

    printf("--- Master Sum ---\n");
    printf("Serial sum:   %.6f,\tSerial time: %lu us\n", sum_serial, total_time);
    printf("\n--- Threads ---\n");

    /* Allocate memory for threads and their partial sums === */
    pthread_t *workers = malloc(sizeof(pthread_t) * num_threads);
    double *partial_sums = malloc(sizeof(double) * num_threads);

    /* === Start timer === */
    gettimeofday(&tv, NULL);
    utime = 1000000 * tv.tv_sec + tv.tv_usec;

    /* === Generate thread IDs and start threads === */
    for (int i = 0; i < num_threads; i++)
    {
        thread_arg_t *arg = malloc(sizeof(thread_arg_t));
        arg->thread_id = i;
        arg->partial_sums = partial_sums;
        pthread_create(&workers[i], NULL, thread_func, arg); 
    }
    
    /* === Wait until all threads are finished === */
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(workers[i], NULL);
    }

    /* === Master computes sum from thread partial sums === */
    double sum_parallel = 0.0;
    for (int i = 0; i < num_threads; i++)
    {
        sum_parallel += partial_sums[i];
    }

    /* === End timer and print results === */
    gettimeofday(&tv, NULL);
    total_time = 1000000 * tv.tv_sec + tv.tv_usec - utime;

    printf("\n--- Thread Sum ---\n");
    printf("Parallel Sum: %.6f,\tParallel time: %lu us\n", sum_parallel, total_time);

    /* === Free resources === */
    free(workers);

    return 0;
}

void *thread_func(void *arg)
{ 
    /* === Parse thread arg to get ID and partial sum pointer === */
    thread_arg_t *data = (thread_arg_t *)arg;
    int my_id = data->thread_id;
    double *partial_sums = data->partial_sums;
    free(arg);

    /* === Compute part of array to sum === */
    int chunk_size = ARR_SIZE / num_threads;
    int start = my_id * chunk_size;
    int end = (my_id == num_threads - 1) ? ARR_SIZE : start + chunk_size;

    /* === Do sum === */
    double thread_sum = 0.0;
    for (int i = start; i < end; i++)
    {
        thread_sum += arr[i];
    }

    /* === Write partial sum so it is available to master === */
    partial_sums[my_id] = thread_sum;

    /* === Print thread result and exit === */
    printf("Thread %d sum = %f\n", my_id, thread_sum);
    pthread_exit(0);
}