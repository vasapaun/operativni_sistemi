#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>

#define check_error(cond, msg)  \
    do{ \
        if(!(cond)){    \
            perror(msg);    \
            exit(EXIT_FAILURE); \
        }   \
    } while(0)

#define pthread_check_error(cmd, msg)   \
    do{ \
        int ret = (cmd);    \
        if(ret > 0){    \
            errno = ret;    \
            check_error(false, msg);    \
        }   \
    } while(0)

long suma = 0;
pthread_mutex_t mutex;

typedef struct{
    int i;
} THREAD_ARGS;

typedef struct{
    long curr_sum;
} THREAD_RET_VAL;

void* thread_func(void* arg){
    THREAD_ARGS* args = (THREAD_ARGS*) arg;
    THREAD_RET_VAL* retval = malloc(sizeof(THREAD_RET_VAL));
    check_error(retval != NULL, "malloc retval");

    pthread_mutex_lock(&mutex);
    suma += args->i;
    retval->curr_sum = suma;
    pthread_mutex_unlock(&mutex);

    return retval;
}

int main(int argc, char** argv){
    pthread_t* threads = malloc(100 * sizeof(pthread_t));
    THREAD_ARGS* thread_args = malloc(100 * sizeof(THREAD_ARGS));
    check_error(threads != NULL && thread_args != NULL, "threads malloc");

    for(int i = 0; i < 100; i++){
        thread_args[i].i = i + 1;
        pthread_check_error(pthread_create(&threads[i], 0, &thread_func, (void*) &thread_args[i]), "pthread_create");
    }

    for(int i = 0; i < 100; i++){
        void* xxx;
        pthread_check_error(pthread_join(threads[i], &xxx), "pthread_join");
        printf("Current sum: %ld\n", ((THREAD_RET_VAL*) xxx)->curr_sum);
    }

    
    return 0;
}
