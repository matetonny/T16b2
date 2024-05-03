#include <stdio.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char const *argv[]){
    FILE *pfile;
    pfile = fopen(argv[1], "rb");

    char input_program;

    fgets(input_program, PROGRAM_SIZE, pfile);
    fclose(pfile);

    

    return 0;
}
