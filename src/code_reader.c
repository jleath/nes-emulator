#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "code_reader.h"

typedef struct stat file_stats;

off_t get_file_size(char *filename) {
    struct stat buffer;
    int success = stat(filename, & buffer);
    if (success == -1) {
        return -1;
    }
    return buffer.st_size;
}

uint8_t *read_program_file(char *filename, off_t file_size) {
    FILE *program_file = fopen(filename, "r");
    uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t) * file_size);
    for (int i = 0; i < file_size; i++) {
        buffer[i] = fgetc(program_file);
    }
    return buffer;
}

