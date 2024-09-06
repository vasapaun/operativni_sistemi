#include <stdio.h>
#include <stdlib.h>
#include <check_error.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LEN (128)

int max = 0;
pthread_mutex_t mutex;

typedef struct 
{
    int* arr;
    int arrlen;
} THREAD_ARGS;

int maximum(int a, int b) {
    if(a > b) return a;
    return b;
}

void* thread_func(void* args){
    int* arr = ((THREAD_ARGS*) args)->arr;
    int arrlen = ((THREAD_ARGS*) args)->arrlen;

    int local_max = arr[0];
    for(int i = 1; i < arrlen; i++)
        local_max = maximum(local_max, arr[i]);
        
    pthread_mutex_lock(&mutex);
    max = maximum(local_max, max);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char** argv)
{
    check_error(argc == 2, "argc");

    FILE* input_file = fopen(argv[1], "r");
    check_error(input_file != NULL, "fopen");

    char* line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
    check_error(line != NULL, "malloc");
    size_t bytes_read;

    getline(&line, &bytes_read, input_file);
    int num_of_threads;
    sscanf(line, "%d", &num_of_threads);

    int** nizovi;
    int count;

    pthread_t* threads = (pthread_t*) malloc(num_of_threads * sizeof(pthread_t));
    check_error(threads != NULL, "malloc");

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < num_of_threads; i++)
    {
        getline(&line, &bytes_read, input_file);

        nizovi = (int**) malloc(num_of_threads * sizeof(int*));
        check_error(nizovi != NULL, "malloc");

        sscanf(line, "%d", &count);

        nizovi[i] = (int*) malloc(count * sizeof(int));
        check_error(nizovi[i] != NULL, "malloc");

        char* ptr = strchr(line, ' ') + 1;

        for(int j = 0; j < count; j++) {
            sscanf(ptr, "%d", &nizovi[i][j]);
            ptr = strchr(ptr, ' ') + 1;
        }

        THREAD_ARGS thr_arg;
        thr_arg.arr = nizovi[i];
        thr_arg.arrlen = count;
        thr_check_error(pthread_create(&threads[i], 0, &thread_func, (void*) (&thr_arg)), "pthread_create");
    }

    for(int i = 0; i < num_of_threads; i++) {
        thr_check_error(pthread_join(threads[i], NULL), "join");
    }

    printf("Max: %d\n", max);

    pthread_mutex_destroy(&mutex);

    for(int i = 0; i < num_of_threads; i++) free(nizovi[i]);
    free(nizovi);
    return 0;
}
