#include <stdio.h>

const int PROGRAM_SIZE = 65536;

typedef struct token {
    char token_type;
    u_int16_t value;
} token