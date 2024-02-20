#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

#define check_error(cond, msg)  \
    do{                         \
        if(!(cond)){            \
            perror(msg);        \
            exit(EXIT_FAILURE); \
        }                       \
    } while(0)

#define pthread_check_error(cond, msg)  \
    do{                                 \
        int _cond = (cond);             \
        if(_cond > 0){                  \
            errno = _cond;              \
            check_error(false, msg);    \
        }                               \
    } while(0)

typedef struct{
    double retval;
}THREAD_RET_VAL;

typedef struct{
    double* vrsta;
    int duzina_vrste;
    double stepen;
}THREAD_ARGS;

double suma = 0;
pthread_mutex_t mutex;

void* thread_func(void* arg){
    THREAD_ARGS* args = (THREAD_ARGS*) arg;
    
    double zbir = 0;
    for(int i = 0; i < args->duzina_vrste; i++){
        zbir += pow(args->vrsta[i], args->stepen);
    }
    
    pthread_mutex_lock(&mutex);
    suma += zbir;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(){
    double p, el;
    int m, n;
    scanf("%lf %d %d", &p, &m, &n);

    double** matrica = (double**) malloc(m * sizeof(double*));
    check_error(matrica != NULL, "malloc");
    for(int i = 0; i < m; i++){
        matrica[i] = (double*) malloc(n * sizeof(double));
        check_error(matrica[i] != NULL, "malloc");
        
        for(int j = 0; j < n; j++){
            scanf("%lf", &el);
            matrica[i][j] = el;
        }
    }

    THREAD_ARGS* thread_args = (THREAD_ARGS*) malloc(m * sizeof(THREAD_ARGS));
    check_error(thread_args != NULL, "malloc");
    pthread_t* threads = (pthread_t*) malloc(m * sizeof(pthread_t));
    check_error(threads != NULL, "malloc");

    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < m; i++){
        thread_args[i].duzina_vrste = n;
        thread_args[i].stepen       = p;
        thread_args[i].vrsta        = matrica[i];
        pthread_check_error(pthread_create(&threads[i], 0, &thread_func, &thread_args[i]), "pthread_create");
    }

    for(int i = 0; i < m; i++){
        void* ret;
        pthread_check_error(pthread_join(threads[i], &ret), "pthread_join");
    }

    printf("%lf\n", pow(suma, 1/p));

    for(int i = 0; i < m; i++)  free(matrica[i]);
    free(matrica);
    free(thread_args);
    free(threads);
    return 0;
}




















