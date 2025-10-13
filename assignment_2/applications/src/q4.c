#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define NUM_BINS 30

int num_threads;
int arr_size;
double *arr;

typedef struct {
    int thread_id;
    int (*partial_histogram)[NUM_BINS];
} thread_arg_t;

void print_histogram(const char *title, int *hist, unsigned long time_us)
{
    printf("\n--- %s ---\n", title);
    for (int i = 0; i < NUM_BINS; i++)
    {
        printf("Bin %2d: %-8d  ", i, hist[i]);
        if ((i + 1) % 5 == 0 || i == NUM_BINS - 1)
            printf("\n");
    }
    printf("%s time: %lu us\n", (title[0] == 'S') ? "Serial" : "Parallel", time_us);
}

void *thread_func(void *arg);

int main(int argc, char *argv[])
{
    /* === Read user argument === */
    if (argc != 3) {
        return 1;
    }

    num_threads = atoi(argv[1]);
    arr_size = atoi(argv[2]);
    if (num_threads <= 0 || arr_size <= 0)
    {
        return 1;
    }

    /* === Initialize array === */
    arr = malloc(sizeof(double) * arr_size);
    srand(time(NULL));
    for (int i = 0; i < arr_size; i++)
    {
        arr[i] = (double)rand() / (double)(RAND_MAX);
    }

    /* === Start timer === */
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long utime = 1000000 * tv.tv_sec + tv.tv_usec;

    /* === Compute serial histogram === */
    int hist_serial[NUM_BINS] = {0};
    for (int i = 0; i < arr_size; i++)
    {
        int bin = (int)(arr[i] * NUM_BINS);
        if (bin == NUM_BINS)
        {
            bin = NUM_BINS - 1;
        }
        hist_serial[bin]++;
    }

    /* === End timer and print results === */
    gettimeofday(&tv, NULL);
    unsigned long total_time = 1000000 * tv.tv_sec + tv.tv_usec - utime;

    print_histogram("Serial Histogram", hist_serial, total_time);

    /* Allocate memory for threads and their partial histograms === */
    pthread_t *workers = malloc(sizeof(pthread_t) * num_threads);
    int (*partial_histograms)[NUM_BINS] = calloc(num_threads, sizeof(*partial_histograms));

    /* === Start timer === */
    gettimeofday(&tv, NULL);
    utime = 1000000 * tv.tv_sec + tv.tv_usec;

    for (int i = 0; i < num_threads; i++)
    {
        thread_arg_t *arg = malloc(sizeof(thread_arg_t));
        arg->thread_id = i;
        arg->partial_histogram = partial_histograms;
        pthread_create(&workers[i], NULL, thread_func, arg);
    }

    /* === Wait until all threads are finished === */
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(workers[i], NULL);
    }

    /* === Compute histogram from partial histograms === */
    int hist_parallel[NUM_BINS] = {0};
    for (int t = 0; t < num_threads; t++)
    {
        for (int b = 0; b < NUM_BINS; b++)
        {
            hist_parallel[b] += partial_histograms[t][b];
        }
    }

    /* === End timer and print results === */
    gettimeofday(&tv, NULL);
    total_time = 1000000 * tv.tv_sec + tv.tv_usec - utime;

    print_histogram("Parallel Histogram", hist_serial, total_time);

    /* Cleanup */
    free(arr);
    free(workers);
    free(partial_histograms);

    return 0;
}

void *thread_func(void *arg)
{
    /* === Parse thread arg to get ID and partial histogram pointer */
    thread_arg_t *data = (thread_arg_t *)arg;
    int tid = data->thread_id;
    int (*partial_histograms)[NUM_BINS] = data->partial_histogram;
    free(arg);

    int chunk_size = arr_size / num_threads;
    int start = tid * chunk_size;
    int end = (tid == num_threads - 1) ? arr_size : start + chunk_size;

    /* === Do histogram === */
    for (int i = start; i < end; i++)
    {
        int bin = (int)(arr[i] * NUM_BINS);
        if (bin == NUM_BINS) bin = NUM_BINS - 1;
        partial_histograms[tid][bin]++;
    }

    pthread_exit(NULL);
}