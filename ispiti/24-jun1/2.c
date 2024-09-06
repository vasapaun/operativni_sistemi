#include <check_error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define __USE_XOPEN_EXTENDED 1
#define _XOPEN_SOURCE 800
#define NEDELJA_U_SEKUNDAMA (7 * 24 * 60 * 60)

char* ekst;
int sum = 0;

int nftwHelper(const char* fpath, const struct stat* sb, int typeflag, struct FTW* ftwbuf)
{
    if(strcmp(ekst, strrchr(fpath, '.')) != 0
    || typeflag != FTW_F
    || time(NULL) - sb->st_mtime >= NEDELJA_U_SEKUNDAMA) return 0;

    sum += sb->st_size;
    
    //nastavi
    return 0;
}

int main(int argc, char** argv)
{
    check_error(argc == 3, "argc");

    ekst = argv[2];

    nftw(argv[1], nftwHelper, 20, 0);

    printf("Size: %d\n", sum);

    return 0;
}
