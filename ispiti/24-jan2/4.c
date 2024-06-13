#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME_LENGTH (20)
#define MAX_THREAD_COUNT    (50)
#define MAX_WORD_LENGTH     (30)

#define check_error(cond, msg)\
    do\
    {\
        if (!(cond))\
        {\
            perror(msg);\
            fprintf(stderr, "file: %s\nfunc: %s\nline: %d\n", __FILE__, __func__, __LINE__);\
            exit(EXIT_FAILURE);\
        }\
    } while (0)

typedef struct {
    char* fpath;
} THREAD_ARGS;

int sum = 0;
pthread_mutex_t mutex;
void* thr_func(void* param_args)
{
    THREAD_ARGS* arg = (THREAD_ARGS*) param_args;

    int internal_sum = 0;
    
    FILE* input = fopen(arg->fpath, "r");
    check_error(input != NULL, "fopen");

    char* rec = (char*) malloc(MAX_WORD_LENGTH * sizeof(char));
    check_error(rec != NULL, "malloc");

    while(fscanf(input, "%s", rec) != EOF) internal_sum += strlen(rec);

    pthread_mutex_lock(&mutex);
    sum += internal_sum;
    pthread_mutex_unlock(&mutex);

    fclose(input);
    free(rec);

    printf("%s: %d\n", arg->fpath, internal_sum);

    return NULL;
}

int main(int argc, char **argv)
{
    char* fajl = (char*) malloc(MAX_FILENAME_LENGTH * sizeof(char));
    check_error(fajl != NULL, "malloc");

    pthread_t* threads = (pthread_t*) malloc(MAX_THREAD_COUNT * sizeof(pthread_t));
    check_error(threads != NULL, "malloc");

    THREAD_ARGS thr_arg;
    thr_arg.fpath = (char*) malloc(MAX_FILENAME_LENGTH * sizeof(char));
    check_error(thr_arg.fpath != NULL, "malloc");

    size_t thr_count = 0;

    pthread_mutex_init(&mutex, NULL);

    while(scanf("%s", fajl) != EOF) 
    {
        strcpy(thr_arg.fpath, fajl);
        pthread_create(&threads[thr_count++], 0, thr_func, &thr_arg);
    }

    free(thr_arg.fpath);

    for(int i = 0; i < thr_count; i++) 
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("%d\n", sum);

    return 0;
}
