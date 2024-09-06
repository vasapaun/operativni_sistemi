#include <stdio.h>
#include <stdlib.h>
#include <check_error.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_WORD_LEN (257)

bool ends_with(const char* s) 
{
    char* ptr = strrchr(s, '_');
    if(strcmp(ptr, "_test") == 0)   return true;
    else                            return false;
}

void censor(char* s)
{
    int len = strlen(s);
    for(int i = 0; i < len; i++)    s[i] = '#';
}

int main(int argc, char** argv)
{
    check_error(argc == 2, "argc");


    FILE* input_file = fopen(argv[1], "r+");
    check_error(input_file != NULL, "fopen");

    int input_file_fd = fileno(input_file);

    struct flock lock;
    lock.l_whence = SEEK_CUR;
    lock.l_start = 0;

    char* line = (char*) malloc(MAX_WORD_LEN * sizeof(char));
    check_error(line != NULL, "malloc");

    while(fscanf(input_file, "%s", line) == 1)
    {
        printf("%s\n", line);
        fseek(input_file, -strlen(line), SEEK_CUR);

        lock.l_len = strlen(line);
        lock.l_type = F_GETLK;
        fcntl(input_file_fd, F_GETLK, &lock);

        if(ends_with(line) && lock.l_type == F_UNLCK)
        {
            lock.l_type = F_WRLCK;
            fcntl(input_file_fd, F_SETLK, &lock);
            censor(line);
            fwrite(line, sizeof(char), strlen(line), input_file);
        }
            
    }

    close(input_file_fd);
    fclose(input_file);
    free(line);

    return 0;
}
