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
double sum_parallel = 0.0;
int num_threads;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *arg);

/**
 * @brief Create an array of 1,000,000 random floats in [0, 1] and calculates the sum.
 *        Then creates a group of worker threads, each summing up their corresponding
 *        part of the array.
 */
int main(int argc, char *argv[])
{
    // read user argument
    if (argc != 2)
    {
        return 1;
    }
    
    num_threads = atoi(argv[1]);
    if (num_threads <= 0)
    {
        return 1;
    }

    double sum_serial = 0;

    // initialize array
    srand(time(NULL));
    for (int i = 0; i < ARR_SIZE; i++)
    {
        arr[i] = (double)rand()/(double)(RAND_MAX);
    }

    // serial sum
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long utime = 1000000 * tv.tv_sec + tv.tv_usec;

    for (int i = 0; i < ARR_SIZE; i++)
    {
        sum_serial += arr[i];
    }

    gettimeofday(&tv, NULL);
    unsigned long total_time = 1000000 * tv.tv_sec + tv.tv_usec - utime;

    printf("--- Master Sum ---\n");
    printf("Serial sum:   %.6f,\tSerial time: %lu us\n", sum_serial, total_time);
    printf("\n--- Threads ---\n");

    /* Create a pool of num_threads workers and keep them in workers */ 
    pthread_t *workers = malloc(sizeof(pthread_t) * num_threads);

    // timer begin
    gettimeofday(&tv, NULL);
    utime = 1000000 * tv.tv_sec + tv.tv_usec;

    for (int i = 0; i < num_threads; i++)
    {
        int *tid = malloc(sizeof(int));
        *tid = i;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&workers[i], &attr, thread_func, tid); 
    }
    
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(workers[i], NULL);
    }

    gettimeofday(&tv, NULL);
    total_time = 1000000 * tv.tv_sec + tv.tv_usec - utime;

    printf("\n--- Thread Sum ---\n");
    printf("Parallel Sum: %.6f,\tParallel time: %lu us\n", sum_parallel, total_time);

    free(workers);

    return 0;
}

void *thread_func(void *arg) { 
    // assign each thread an id so that they are unique in range [0, num_thread - 1 ]
    int my_id = *(int *)arg;
    free(arg);

    int chunk_size = ARR_SIZE / num_threads;
    int start = my_id * chunk_size;
    int end = (my_id == num_threads - 1) ? ARR_SIZE : start + chunk_size;

    double my_sum = 0.0;
    for (int i = start; i < end; i++)
    {
        my_sum += arr[i];
    }

    pthread_mutex_lock(&lock);
    sum_parallel += my_sum;
    pthread_mutex_unlock(&lock);

    printf("Thread %d sum = %f\n", my_id, my_sum);
    pthread_exit(0);
}