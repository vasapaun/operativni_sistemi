#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define check_error(cond, msg)  \
    do {                        \
        if(!(cond)){            \
            perror(msg);        \
            exit(EXIT_FAILURE); \
        }                       \
    } while(0)

#define RD (0)
#define WR (1)

int main(int argc, char** argv){
    check_error(argc>1, "argc");
    
    int pipefd[2];
    check_error(pipe(pipefd) != -1, "pipe");

    FILE* chtopar = fdopen(pipefd[RD], "r");
    pid_t child;
    for(int i = 1; i < argc; i++){
        child = fork();
        check_error(child != (pid_t)-1, "fork");

        if(child == 0){ //u child procesu smo
            check_error(fclose(chtopar) != -1, "pipe close");
            check_error(dup2(pipefd[WR], STDOUT_FILENO) != -1, "dup2");
            check_error(execlp("du", "du", "-sch", argv[i], NULL) != -1, "execlp");
        }
        
        else{ //u parent procesu smo
            int status = 0;
            check_error(wait(&status) != -1, "wait");
            if(!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
                printf("neuspeh");
            else{
                char *line = NULL;
                size_t n = 0;
                getline(&line, &n, chtopar); // ignorisemo ovo ucitavanje
                getline(&line, &n, chtopar);
                int i = i;
                for (; !isblank(line[i]); i++)
                {
                    if (line[i] == '.') // ?? iz nekog razloga se trazi format ispisa sa zarezom ??
                    {
                        line[i] = ',';
                    }
                }
                line[i] = '\0';
                printf("%s", line);
                free(line);
            }
            putchar(' ');
        }
    }
    putchar('\n');
    return 0;
}
