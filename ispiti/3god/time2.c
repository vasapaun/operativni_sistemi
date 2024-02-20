#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define check_error(cond, msg) \
    do{ \
        if(!(cond)){    \
            perror(msg);    \
            exit(EXIT_FAILURE); \
        }   \
    } while(0)  

#define SECONDS_IN_DAY (24 * 60 * 60)
    
void printMonth(int monthNum);

int main(int argc, char** argv){
    check_error(argc == 3, "argc");

    time_t seconds = atol(argv[1]);
    int days = atoi(argv[2]);

    struct tm* time = gmtime(&seconds);

    printMonth(time->tm_mon);
    
    seconds += days * SECONDS_IN_DAY;

    time = gmtime(&seconds);

    printMonth(time->tm_mon);

    putchar('\n');

    return 0;
}

void printMonth(int monthNum){
    switch (monthNum) {
        case 0: printf("januar "); break;
        case 1: printf("februar "); break;
        case 2: printf("mart "); break;
        case 3: printf("april "); break;
        case 4: printf("maj "); break;
        case 5: printf("jun "); break;
        case 6: printf("jul "); break;
        case 7: printf("avgust "); break;
        case 8: printf("septembar "); break;
        case 9: printf("oktobar "); break;
        case 10: printf("novembar "); break;
        case 11: printf("decembar "); break;
    }
}
