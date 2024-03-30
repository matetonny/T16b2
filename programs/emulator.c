#include <stdio.h>

int main(int argc, char const *argv[]){
    const int PROGRAM_SIZE = 1000;
    FILE *pfile;
    pfile = fopen("programs/boot.hex", "rb");

    char boot_program[PROGRAM_SIZE];

    fgets(boot_program, PROGRAM_SIZE, pfile);
    printf("%s", boot_program);
    fclose(pfile);

    return 0;
}