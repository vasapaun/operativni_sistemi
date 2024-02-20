#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define check_error(cond, msg) \
    do{ \
        if(!(cond)){ \
            perror(msg); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)

#define ARRAY_MAX (1024)

typedef struct{
    sem_t inDataReady;
    int array[ARRAY_MAX];
    unsigned arrayLen;
} OsData;

void* get_mem_block(const char* shm_in, size_t* size);
void* create_mem_block(const char* shm_out, size_t size);
unsigned num_of_1s(int x);

int main(int argc, char** argv){
    check_error(argc == 3, "argc");

    size_t size = 0;
    OsData* in_data = (OsData*)get_mem_block(argv[1], &size);
    check_error(sem_wait(&(in_data->inDataReady)) != -1, "sem_wait");

    OsData* out_data = (OsData*)create_mem_block(argv[2], size);
    out_data->arrayLen = 0;

    for(int i = 0; i < in_data->arrayLen; i++){
        if(num_of_1s(in_data->array[i]) >= 4){
            out_data->array[out_data->arrayLen++] = in_data->array[i];
        }
    }

    check_error(sem_init(&(out_data->inDataReady), 1, 1) != -1, "sem_init");

    check_error(-1 != munmap(in_data, size), "munmap in_data");
    check_error(-1 != munmap(out_data, size), "munmap out_data");

    return 0;
}

void* get_mem_block(const char* shm_in, size_t* size){
    int shmfd = shm_open(shm_in, O_RDWR, 0);
    check_error(shmfd != -1, "shm_open get");

    struct stat file_info;
    check_error(-1 != fstat(shmfd, &file_info), "fstat get");
    *size = file_info.st_size;

    void* addr = mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    check_error(addr != MAP_FAILED, "mmap get");

    check_error(-1 != close(shmfd), "close get");
    return addr;
}

void* create_mem_block(const char* shm_out, size_t size){
    int shmfd = shm_open(shm_out, O_RDWR | O_TRUNC | O_CREAT, 0644);
    check_error(-1 != shmfd, "shm_open create");

    check_error(-1 != ftruncate(shmfd, size), "ftruncate create");

    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    check_error(MAP_FAILED != addr, "mmap create");

    check_error(-1 != close(shmfd), "close create");
    return addr;
}

unsigned num_of_1s(int x){
    unsigned mask = 1u << (8 * sizeof(unsigned) - 1);
    unsigned count = 0;

    while(mask){
        if(x & mask)    count++;
        mask >>= 1;
    }

    return count;
}
    

















    
