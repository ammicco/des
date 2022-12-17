#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void *readNbit(FILE *file, size_t __chunk_size);

uint64_t *read_all_file(FILE *file, size_t __chunk_size);