#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// define instructions and program structs
typedef struct Instruction{
    char instruction;
    uint16_t diri;
    char fer;
    char ser;
    bool rod;
} Instruction;

uint16_t mergeCharsToUInt16(char highByte, char lowByte) {
    uint16_t result = ((uint16_t)(highByte & 0xFF) << 8) | (lowByte & 0xFF);

    return result;
}

// define debugging functions
void print_bytes(char bytes[]){
    for (int i = 0; i < sizeof(&bytes); i++){
        printf("%hhx ", bytes[i]);
    }
}

int main(int argc, char const *argv[]){
    // read boot.hex
    const int twoToSixteen = 65536;
    const int PROGRAM_SIZE = twoToSixteen * 4;
    FILE *pfile;
    pfile = fopen("programs/boot.hex", "rb");

    char boot_program[PROGRAM_SIZE];

    fgets(boot_program, PROGRAM_SIZE, pfile);
    fclose(pfile);

    // process boot.hex
    Instruction processed_program[PROGRAM_SIZE / 4];
    
    // loop through the boot program and split it into instruction structs
    for (int i = 0; i < strlen(boot_program); i += 4){
        processed_program[i / 4].instruction = boot_program[i] & 0b00111111;
        processed_program[i / 4].diri = mergeCharsToUInt16(boot_program[i + 1], boot_program[i + 2]);;
        processed_program[i / 4].fer = (boot_program[i + 3] >> 4) & 0b00000111;
        processed_program[i / 4].ser = (boot_program[i + 3] >> 1) & 0b00000111;
        processed_program[i / 4].rod = boot_program[i + 3] & 0b00000001;

        mergeCharsToUInt16(boot_program[i + 1], boot_program[i + 2]);
    }

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

    // loop through the program and complete instructions
    bool running = true;

    for (int i = 0; i < sizeof(processed_program)/sizeof(processed_program[0]); i++){
        // run instruction
        Instruction comp_instruction = processed_program[i];
        if (running){
            switch (comp_instruction.instruction){
                //lda
                case 0x00:
                    *registers[comp_instruction.ser] = a;
                    break;

                //ldb
                case 0x01:
                    *registers[comp_instruction.ser] = b;
                    break;

                //ldo
                case 0x02:
                    *registers[comp_instruction.ser] = ao;
                    break;

                //ldf
                case 0x03:
                    *registers[comp_instruction.ser] = fl;
                    break;

                //sta
                case 0x04:
                    if (!comp_instruction.rod){
                        a = *registers[comp_instruction.ser];
                    } else {
                        a = comp_instruction.diri;
                    }

                    break;

                //stb
                case 0x05:
                    if (!comp_instruction.rod){
                        b = *registers[comp_instruction.ser];
                    } else {
                        b = comp_instruction.diri;
                    }

                    break;

                //ldr
                case 0x06:
                    if (!comp_instruction.rod){
                        ma = *registers[comp_instruction.fer];
                    } else {
                        ma = comp_instruction.diri;
                    }

                    *registers[comp_instruction.ser] = memory[bk][ma];

                    printf("%x", *registers[comp_instruction.ser]);

                    break;
                
                //str
                case 0x07:
                    if (!comp_instruction.rod){
                        ma = *registers[comp_instruction.fer];
                    } else {
                        ma = comp_instruction.diri;
                    }

                    memory[bk][ma] = *registers[comp_instruction.ser];

                    break;

                //add
                case 0x08:
                    if (!comp_instruction.rod){
                        ao = (*registers[comp_instruction.fer] + *registers[comp_instruction.ser]) & 0xffff;
                        fl = (*registers[comp_instruction.fer] + *registers[comp_instruction.ser]) & 0x10000;
                    } else {
                        ao = (comp_instruction.diri + *registers[comp_instruction.ser]) & 0xffff;
                        fl = (comp_instruction.diri + *registers[comp_instruction.ser]) & 0x10000;
                    }

                    break;

                //sub
                case 0x09:
                    if (!comp_instruction.rod){
                        ao = (*registers[comp_instruction.fer] - *registers[comp_instruction.ser]) & 0xffff;
                        fl = (*registers[comp_instruction.fer] - *registers[comp_instruction.ser]) & 0x10000;
                    } else {
                        ao = (comp_instruction.diri - *registers[comp_instruction.ser]) & 0xffff;
                        fl = (*registers[comp_instruction.fer] - *registers[comp_instruction.ser]) & 0x10000;
                    }

                    break;

                //mul
                case 0x0a:
                    if (!comp_instruction.rod){
                        ao = (uint16_t)(((u_int32_t)*registers[comp_instruction.fer] * (u_int32_t)*registers[comp_instruction.ser]) & 0xffff);
                        fl = (uint16_t)(((u_int32_t)*registers[comp_instruction.fer] * (u_int32_t)*registers[comp_instruction.ser]) >> 16);
                    } else {
                        ao = (uint16_t)(((u_int32_t)comp_instruction.diri * (u_int32_t)*registers[comp_instruction.ser]) & 0xffff);
                        fl = (uint16_t)(((u_int32_t)comp_instruction.diri * (u_int32_t)*registers[comp_instruction.ser]) >> 16);
                    }

                    break;
                
                //div
                case 0x0b:
                    if (!comp_instruction.rod){
                        ao = (*registers[comp_instruction.fer] / *registers[comp_instruction.ser]) & 0xffff;
                        fl = (*registers[comp_instruction.fer] % *registers[comp_instruction.ser]) & 0xffff;
                    } else {
                        ao = (comp_instruction.diri / *registers[comp_instruction.ser]) & 0xffff;
                        fl = (comp_instruction.diri % *registers[comp_instruction.ser]) & 0xffff;
                    }

                    break;

                //and
                case 0x0c:
                    if (!comp_instruction.rod){
                        ao = *registers[comp_instruction.fer] & *registers[comp_instruction.ser];
                    } else {
                        ao = comp_instruction.diri & *registers[comp_instruction.ser];
                    }

                    break;

                //or
                case 0x0d:
                    if (!comp_instruction.rod){
                        ao = *registers[comp_instruction.fer] | *registers[comp_instruction.ser];
                    } else {
                        ao = comp_instruction.diri | *registers[comp_instruction.ser];
                    }

                    break;

                //not
                case 0x0e:
                    if (!comp_instruction.rod){
                        ao = ~*registers[comp_instruction.fer];
                    } else {
                        ao = ~comp_instruction.diri;
                    }

                    break;

                // hlt
                case 0x15:
                    running = false;
                    break;
            }

        } else {
            break;
        }
    }

    return 0;
} 