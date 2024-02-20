#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

#define check_error(cond, msg)  \
    do{                         \
        if(!(cond)){            \
            perror(msg);        \
            exit(EXIT_FAILURE); \
        }                       \
    } while(0)

int count, signum;
void signal_handler(int signum){
    switch(signum){
        case SIGUSR1: count += 1; break;
        case SIGUSR2: count += 2; break;
        case SIGINT:  count -= 4; break;
        case SIGTERM: printf("%d\n", count); exit(EXIT_SUCCESS); break;
    }
    return;
}

int main(int argc, char** argv){
    while(1){
        check_error(signal(SIGUSR1, &signal_handler) != SIG_ERR, "sigusr1");
        check_error(signal(SIGUSR2, &signal_handler) != SIG_ERR, "sigusr2");
        check_error(signal(SIGINT , &signal_handler) != SIG_ERR, "sigint" );
        check_error(signal(SIGTERM, &signal_handler) != SIG_ERR, "sigterm");
    }
    return 0;
}
