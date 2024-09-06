#include <stdio.h>
#include <stdlib.h>
#include <check_error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FPATH_LEN (100)

int main()
{
    char* envpath = getenv("PATH");
    char *begin = envpath, *end = strchr(begin, ':'), *curr;

    curr = (char*) malloc(MAX_FPATH_LEN * sizeof(char));
    check_error(curr != NULL, "malloc");

    struct stat sb;

    while(end != NULL)
    {
        curr[0] = '\0';
        strncat(curr, begin, end - begin);

        check_error(stat(curr, &sb) != -1, "stat");

        unsigned flags = S_IRUSR | S_IRGRP | S_IROTH;

        if((sb.st_mode & flags) == flags) printf("%s\n", strrchr(curr, '/') + 1);

        begin = end+1;
        end = strchr(begin, ':');
    }

    free(curr);

    return 0;
}
