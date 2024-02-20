#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define check_error(expr, msg) \
    do{ \
        if(!(expr)){ \
        perror(msg); \
        exit(EXIT_FAILURE); \
        } \
    } while(0)

int num = 0;

void signal_handler(int signum){
    switch(signum){
        case SIGUSR1: num += 1; break;
        case SIGUSR2: num += 2; break;
        case SIGINT:  num -= 4; break;
        case SIGTERM: printf("%d\n", num); exit(EXIT_SUCCESS); break;
    }
}

int main(){
    
    while(1){
        check_error(signal(SIGUSR1, &signal_handler) != SIG_ERR, "SIGUSR1");
        check_error(signal(SIGUSR2, &signal_handler) != SIG_ERR, "SIGUSR2");
        check_error(signal(SIGINT , &signal_handler) != SIG_ERR, "SIGINT" );
        check_error(signal(SIGTERM, &signal_handler) != SIG_ERR, "SIGTERM");
    }

    return 0;
}
