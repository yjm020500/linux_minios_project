#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_POINTS 200000
#define NUM_THREADS 3

long incircle = 0;
long ppt; // points per thread
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *MonteCarlo(void *param) { // threads call this, pointing func.
    long incircle_thread = 0;
    unsigned int rand_state = rand();
    long i;

    for (i = 0; i < ppt; i++) {
        double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;

        if (x * x + y * y < 1) {
            incircle_thread++;
        }
    }
    pthread_mutex_lock(&mutex);
    incircle += incircle_thread;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// est_pi() main function
int main() {
    // if (argc != 3) {
    //     fprintf(stderr, "usage: ./pi <total points> <threads>\n");
    //     exit(1);
    // }
    long totalpoints = NUM_POINTS;
    int thread_count = NUM_THREADS;
    ppt = totalpoints / thread_count;

    pthread_t *threads = malloc(thread_count * sizeof(pthread_t));
    pthread_attr_t attr; // set of thread attributes
    pthread_attr_init(&attr); // set of the default attributes
    int i;
    char select[5];

    do {
        printf("\n종료하려면 exit을 입력해주세요.\n>> ");
        scanf("%s", select);
        if (strcmp(select, "exit"))
            for (i = 0; i < thread_count; i++) {
                // create the thread as NUM_THREADS, which is 4
                pthread_create(&threads[i], &attr, MonteCarlo, (void *) NULL);

                // wait for the thread to exit
                pthread_join(threads[i], NULL);

                printf("\n%d. Estimated value of π: %f\n\n", i+1, (4. * (double)incircle) / ((double)ppt * thread_count));
            }
    } while (strcmp(select, "exit"));
    pthread_mutex_destroy(&mutex);
    free(threads);

    return 0;
}
