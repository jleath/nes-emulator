#ifndef CODE_READER_H
#define CODE_READER_H

#include <stdint.h>
#include <sys/types.h>

typedef struct stat file_stats;

off_t get_file_size(char *filename);
uint8_t *read_program_file(char *filename, off_t file_size);

#endif