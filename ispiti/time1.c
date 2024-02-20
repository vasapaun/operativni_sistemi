#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SECONDS_IN_DAY (60*60*24)

#define check_error(cond, msg)  \
    do{                         \
        if(!(cond)){            \
            perror(msg);        \
            exit(EXIT_FAILURE); \
        }                       \
    } while(0)

int main(int argc, char**argv){
    check_error(argc == 3, "argc");

    time_t timeInSeconds = atol(argv[1]);
    struct tm* time = localtime(&timeInSeconds);
    
    switch(time->tm_mon + 1){
        case 1:  printf("januar ");     break;
        case 2:  printf("februar ");    break;
        case 3:  printf("mart ");       break;
        case 4:  printf("april ");      break;
        case 5:  printf("maj ");        break;
        case 6:  printf("jun ");        break;
        case 7:  printf("jul ");        break;
        case 8:  printf("avgust ");     break;
        case 9:  printf("septembar ");  break;
        case 10: printf("oktobar ");    break;
        case 11: printf("novembar ");   break;
        case 12: printf("decembar ");   break;
    }

    timeInSeconds += atol(argv[2]) * SECONDS_IN_DAY;
    time = localtime(&timeInSeconds); 
    switch(time->tm_mon + 1){
        case 1:  printf("januar\n");     break;
        case 2:  printf("februar\n");    break;
        case 3:  printf("mart\n");       break;
        case 4:  printf("april\n");      break;
        case 5:  printf("maj\n");        break;
        case 6:  printf("jun\n");        break;
        case 7:  printf("jul\n");        break;
        case 8:  printf("avgust\n");     break;
        case 9:  printf("septembar\n");  break;
        case 10: printf("oktobar\n");    break;
        case 11: printf("novembar\n");   break;
        case 12: printf("decembar\n");   break;
    }

    return 0;
}
