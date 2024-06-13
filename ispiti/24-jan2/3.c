#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <utime.h>
#include <sys/stat.h>
#include <unistd.h>

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

int sigstate;
time_t br_sekundi;
void sighandler(int signum)
{
    switch(signum)
    {
        case SIGUSR1:
            sigstate = 1;
            break;

        case SIGUSR2:
            sigstate = 2;
            break;

        case SIGQUIT:
            exit(EXIT_SUCCESS);
    }

    char* env = getenv("PUTANJA");

    if(sigstate == 1)
    {
        struct stat st;
        check_error(stat(env, &st) == 0, "stat");

        printf("%s\n", env);
    }
    else if(sigstate == 2)
    {
        struct utimbuf ut;
        ut.actime = br_sekundi;
        ut.modtime = br_sekundi;
        check_error(utime(env, &ut) == 0, "utime");
    }

    return;
}


int main(int argc, char **argv)
{
    check_error(argc == 2, "argc");

    br_sekundi = strtoul(argv[1], NULL, 10);

    while(1)
    {
        signal(SIGUSR1, sighandler);
        signal(SIGUSR2, sighandler);
        signal(SIGQUIT, sighandler);
    }

    return 0;
}
