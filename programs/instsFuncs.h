#include <stdio.h>

// define instructions and program structs
typedef struct Instruction{
    char instruction;
    u_int16_t diri;
    char fer;
    char ser;
    bool rod;
} Instruction;

u_int16_t mergeCharsToUInt16(char highByte, char lowByte) {
    u_int16_t result = ((u_int16_t)(highByte & 0xFF) << 8) | (lowByte & 0xFF);

    return result;
}

const int twoToSixteen = 65536;
const int PROGRAM_SIZE = twoToSixteen * 4;

// define registers, stack, memory
uint16_t a = 0;
uint16_t b = 0;
uint16_t ao = 0;
uint16_t ma = 0;
uint16_t pm = 0;
uint16_t fl = 0;
u_int16_t sp = 0;
u_int16_t bk = 0;

uint16_t *registers[8] = {
    &a,
    &b,
    &ao,
    &ma,
    &pm,
    &fl,
    &sp,
    &bk
};

uint16_t memory[16][twoToSixteen];
uint16_t stack[256];