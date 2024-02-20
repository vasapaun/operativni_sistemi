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
           check_error(fclose(chtopar) != -1, "fclose child");
           check_error(dup2(pipefd[WR], STDOUT_FILENO) != -1, "dup2 child");
           check_error(execlp("du", "du", "-sch", argv[i], NULL) != -1, "execlp");
        }

        else{ // u parent
            int status = 0;
            check_error(wait(&status) != -1, "wait");
            
            if(!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
                printf("Neuspelo %d dete ", i);

            else{
                char* line = NULL;
                size_t n = 0;
                getline(&line, &n, chtopar);
                getline(&line, &n, chtopar);
                printf("%d\n", i);
                int i = i;
                printf("%d\n", i);
                for(; !isblank(line[i]); i++){
                    printf("i : %d \n", i); 
                    if(line[i] == ','){
                        line[i] = '.';
                    }
                }
                
                line[i] = '\0';
                printf("Linija %d: %s ", i, line);
                free(line);
            }
        }
    }
    putchar('\n');

    return 0;
}
