#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define check_error(cond, msg) \
    do{ \
        if(!(cond)){ \
            perror(msg); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

#define RD_END (0)
#define WR_END (1)

#define MAX_LEN (256)

int main(int argc, char** argv){
    check_error(argc == 2, "./a.out in_file.txt");

    FILE* in_file = fopen(argv[1], "r");
    check_error(in_file != NULL, "in_file open");

    FILE* out_file = fopen("errors.txt", "w+");
    check_error(out_file != NULL, "out_file open");

    char cmd[MAX_LEN], arg[MAX_LEN]; 

    while (2 == fscanf(in_file, "%s %s", cmd, arg)){
        int pipefds[2];
        check_error(-1 != pipe(pipefds), "pipe");

        pid_t child_pid = fork();
        check_error(-1 != child_pid, "fork");

        if (0 == child_pid){ //u child procesu smo
            check_error(-1 != close(pipefds[RD_END]), "close child");
            check_error(-1 != dup2(pipefds[WR_END], STDERR_FILENO), "dup2 child");
            check_error(-1 != execlp(cmd, cmd, arg, NULL), "execlp child");
        }

        else { // u parent procesu smo
            check_error(-1 != close(pipefds[WR_END]), "close parent"); 

            FILE* chtopar = fdopen(pipefds[RD_END], "r");
            check_error(NULL != chtopar, "fdopen parent");

            char* buff = NULL;
            size_t len = 0;

            int status;
            check_error(-1 != wait(&status), "wait");

            if(WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS){
                while(EOF != getline(&buff, &len, chtopar)){
                    fprintf(out_file, "%s", buff);
                }
                free(buff);
            }
        }
    }

    check_error(-1 != fclose(in_file), "fclose");
    check_error(-1 != fclose(out_file), "fclose");
    
    return 0;
}
