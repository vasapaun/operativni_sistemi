#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define check_error(cond, msg) \
    do{     \
        if(!(cond)){    \
            perror(msg);    \
            exit(EXIT_FAILURE); \
        }   \
    } while(0)

#define SECONDS_IN_DAY (24*60*60)

int main(int argc, char** argv){
    check_error(argc==2, "argc");

    time_t *seconds_since_epoch = malloc(sizeof(time_t));
    check_error(time(seconds_since_epoch) != -1, "time");

    printf("%ld\n", *seconds_since_epoch);
    printf("%ld\n", (*seconds_since_epoch)%SECONDS_IN_DAY / 3600);


    return 0;
}
