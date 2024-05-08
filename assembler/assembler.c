#include <stdio.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char const *argv[]){
    // open file
    FILE *pfile;
    pfile = fopen(argv[1], "r");

    // exit if file doesnt open
    if (pfile == NULL){
        printf("Not able to open the file.");
        return 1;
    }

    // put file into input program and remove new lines 
    char line[100];
    char input_program[PROGRAM_SIZE][100];
    int counter = 0;

    while (fgets(line, 100, pfile)) {
        if (strcmp(&line[0], "\n")){
            line[strcspn(line, "\n")] = 0;
            strncpy(input_program[counter], line, 100);
            counter++;
        }
    }

    for (int i = 0; i < counter + 1; i++){
        printf("%s", input_program[i]);
    }

    fclose(pfile);

    return 0;
}
