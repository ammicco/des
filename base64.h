#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "manipulate_string.h"

static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *encode(const unsigned char *, size_t);

char *decode(const unsigned char *, size_t);