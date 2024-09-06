#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <check_error.h>

#define MAX_FILE_SIZE (8192)

char* strcut(char* original, int a, int b);

int main(int argc, char** argv)
{
    check_error(argc == 4, "argc");

    FILE* input_file = fopen(argv[1], "r");

    char* file_content = (char*) malloc(MAX_FILE_SIZE * sizeof(char));
    check_error(file_content != NULL, "malloc");

    fread(file_content, sizeof(char), MAX_FILE_SIZE, input_file);

    int a = atoi(argv[2]);
    int b = atoi(argv[3]);
    char* edited_file_content = strcut(file_content, a, b);

    free(file_content);
    fclose(input_file);

    input_file = fopen(argv[1], "w");
    check_error(input_file != NULL, "fopen");

    fwrite(edited_file_content, sizeof(char), strlen(edited_file_content), input_file);

    free(edited_file_content);

    fclose(input_file);

    return 0;
}

char* strcut(char* original, int a, int b)
{
    char* new = (char*) malloc(strlen(original) - (b - a));

    check_error(strncat(new, original, a) != NULL, "strncat");

    int i, j;

    for(i = b + 1, j = a; original[i] != '\0'; i++, j++)
    {
        new[j] = original[i];
    }

    new[j] = '\n';

    return new;
}
