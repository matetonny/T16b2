#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//define instruction and program structs
typedef struct Instruction{
    char instruction;
    char diri[2];
    char fer;
    char ser;
    char rod;
} Instruction;

unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

//define debugging functions
void print_bytes(char bytes[]){
    for (int i = 0; i < sizeof(&bytes); i++){
        printf("%c", bytes[i]);
    }
}

int main(int argc, char const *argv[]){
    //read boot.hex
    const int PROGRAM_SIZE = 10000;
    FILE *pfile;
    pfile = fopen("programs/boot.hex", "rb");

    char boot_program[PROGRAM_SIZE];

    fgets(boot_program, PROGRAM_SIZE, pfile);
    fclose(pfile);

    //process boot.hex
    Instruction processed_program[PROGRAM_SIZE / 4];
    
    //loop through the boot program and process it
    for (int i = 0; i < strlen(boot_program); i += 4){
        processed_program[i / 4].instruction = boot_program[i] & 0b00111111;
        processed_program[i / 4].diri[0] = boot_program[i + 1];
        processed_program[i / 4].diri[1] = boot_program[i + 2];
        processed_program[i / 4].fer = (boot_program[i + 3] >> 4) & 0b00000111;
        processed_program[i / 4].ser = (boot_program[i + 3] >> 1) & 0b00000111;
        processed_program[i / 4].rod = boot_program[i + 3] & 0b00000001;

        printf("%hhx\n", processed_program[i / 4].diri[0]);
        printf("%hhx", processed_program[i / 4].diri[1]);
    }

    return 0;
}