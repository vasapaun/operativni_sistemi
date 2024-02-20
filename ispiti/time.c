#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define check_error(cond, msg)  \
    do {                        \
        if(!(cond)){            \
            perror(msg);        \
            exit(EXIT_FAILURE); \
        }                       \
    } while(0)

int main(int argc, char** argv){
    check_error(argc==2, "argc");

    time_t argsecs = atol(argv[1]);
    struct tm *lt = localtime(&argsecs);
    int hr = lt->tm_hour;
    if(hr < 7)  printf("noc");
    else if(hr < 9)  printf("jutro");
    else if(hr < 12)  printf("prepodne");
    else if(hr < 19)  printf("popodne");
    else if(hr < 24)  printf("vece");
    return 0;
}
