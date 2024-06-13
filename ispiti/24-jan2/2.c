#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RD_END (0)
#define WR_END (1)

#define MAX_LINE_LENGTH (200)

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


int main(int argc, char **argv)
{
    char* env = getenv("REG_FAJL");

    int pipefds[2];
    check_error(pipe(pipefds) == 0, "pipe");

    pid_t childpid;
    childpid = fork();
    check_error(childpid != -1, "fork");

    if(childpid == 0) // u child procesu smo
    {
        close(pipefds[RD_END]);
        dup2(pipefds[WR_END], STDOUT_FILENO);
        execlp("cat", "cat", env, NULL);
    }

    else 
    {
        close(pipefds[WR_END]);

        FILE* input = fdopen(pipefds[RD_END], "r");
        check_error(input != NULL, "fdopen");

        char* linija = malloc(MAX_LINE_LENGTH * sizeof(char));
        check_error(linija != NULL, "malloc");

        size_t n_read = 0;

        size_t max_linija_len = 0;
        size_t len = 0;

        while(getline(&linija, &n_read, input) > 0)
        {
            len = strlen(linija);
            printf("%s : len %lu\n", linija, len);
            max_linija_len = (max_linija_len > len) ? max_linija_len : len;
        }

        free(linija);

        printf("%lu\n", max_linija_len);
    }

    return 0;
}
