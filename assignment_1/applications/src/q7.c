#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    // read user argument
    if (argc != 2) {
        return 1;
    }
    
    int N = atoi(argv[1]);
    if (N <= 0) {
        return 1;
    }
    
    int arr[N];

    // init rand and fill array
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 2;
    }

    // get time in microseconds before fork
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long utime = 1000000 * tv.tv_sec + tv.tv_usec;

    // create pipes for communicating with children
    int pipe_1[2];
    int pipe_2[2];
    pipe(pipe_1);
    pipe(pipe_2);

    // child 1, process first half of array
    pid_t pid_1 = fork();
    if (pid_1 == 0) {
        close(pipe_1[0]);

        int sum = 0;
        for (int j = 0; j < N/2; j++) {
            sum += arr[j];
        }

        write(pipe_1[1], &sum, sizeof(sum));
        close(pipe_1[1]);
        return 0;
    }

    // child 2, process second half of aray
    pid_t pid_2 = fork();
    if (pid_2 == 0) {
        close(pipe_2[0]);

        int sum = 0;
        for (int j = N/2; j < N; j++) {
            sum += arr[j];
        }

        write(pipe_2[1], &sum, sizeof(sum));
        close(pipe_2[1]);
        return 0;
    }

    close(pipe_1[1]);
    close(pipe_2[1]);

    // terminate zombies
    wait(NULL);
    wait(NULL);

    // read pipes (sums from children)
    int sum_1, sum_2;
    read(pipe_1[0], &sum_1, sizeof(sum_1));
    read(pipe_2[0], &sum_2, sizeof(sum_2));

    close(pipe_1[0]);
    close(pipe_2[0]);

    int sum = sum_1 + sum_2;

    gettimeofday(&tv, NULL);
    unsigned long total_time = 1000000 * tv.tv_sec + tv.tv_usec - utime;

    printf("\n");
    printf("Sum from Child 1 (first half):    %d\n", sum_1);
    printf("Sum from Child 2 (second half):   %d\n", sum_2);
    printf("Parent total sum:                 %d\n", sum);
    printf("Total time taken:                 %lu us\n", total_time);

    return 0;
}
