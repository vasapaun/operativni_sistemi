#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

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
    DIR* dir;
    struct dirent *d;
    dir = opendir(".");

    time_t maks_mtim = 0;
    time_t maks_atim = 0;
    struct stat st;
    while((d = readdir(dir)) != NULL)
    {
        // preskoci neregularne fajlove
        if(d->d_type != DT_REG) continue;

        check_error(stat(d->d_name, &st) == 0, "stat");
        printf("Fajl nadjen: %s, atime: %ld, mtime: %ld\n", d->d_name, st.st_atim.tv_sec, st.st_mtim.tv_sec);
        if(st.st_mtim.tv_sec > maks_mtim)    maks_mtim = st.st_mtim.tv_sec;
        if(st.st_atim.tv_sec > maks_atim)    maks_atim = st.st_atim.tv_sec;
    }
    struct utimbuf novo_vreme;
    novo_vreme.actime = maks_atim;
    novo_vreme.modtime = maks_mtim;

    DIR* dir2;
    struct dirent *d2;
    dir2 = opendir(".");
    // promeni mtime i atime svim regularnim fajlovima
    while((d2 = readdir(dir2)) != NULL)
    {
        // preskoci neregularne fajlove
        if(d2->d_type != DT_REG) continue;

        check_error(0 == utime(d2->d_name, &novo_vreme), "utime"); 
    }

    return 0;
}
