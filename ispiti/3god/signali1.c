#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define check_error(expr, msg) \
    do{ \
        if(!(expr)){ \
        perror(msg); \
        exit(EXIT_FAILURE); \
        } \
    } while(0)

void signal_handler(int signum){
    int x;
    if(signum == SIGUSR1){
        scanf("%d", &x);
        x = (x < 0) ? x*(-1) : x;
        printf("%d\n", x);
    }
    else if(signum == SIGUSR2){
        scanf("%d", &x);
        printf("%d\n", x*x);
    }
    else if(signum == SIGTERM){
        exit(EXIT_SUCCESS);
    }
}

int main(){
    while(1){
        check_error(SIG_ERR != signal(SIGUSR1, &signal_handler), "SIGUSR1");
        check_error(SIG_ERR != signal(SIGUSR2, &signal_handler), "SIGUSR2");
        check_error(SIG_ERR != signal(SIGTERM, &signal_handler), "SIGTERM");
    }

    return 0;
}
