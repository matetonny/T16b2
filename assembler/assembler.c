#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"

int main(int argc, char const *argv[])
{
    // open file
    FILE *pfile;
    pfile = fopen(argv[1], "r");

    // exit if file doesnt open
    if (pfile == NULL)
    {
        printf("\e[0;31merror: Not able to open the file.");
        return 1;
    }

    // put file into input program and remove new lines
    char line[100];
    char input_program[program_size][100];
    int counter = 0;

    while (fgets(line, 100, pfile))
    {
        if (strcmp(&line[0], "\n"))
        {
            line[strcspn(line, "\n")] = 0;
            strncpy(input_program[counter], line, 100);
            counter++;
        }
    }

    fclose(pfile);

    // define variables and allocate memory for tokenizing the program
    char *str;
    const char *delim = " ";
    int num_tokens;
    char **tokens;
    token **tokenized_program = (token **)malloc(program_size * sizeof(token **));
    for (int b = 0; b < 4; ++b)
    {
        tokenized_program[b] = (token *)malloc(4 * sizeof(token));
    }
    int token_counter = 0;

    // loop through the input program and tokenize it
    for (int i = 0; i < counter + 1; i++)
    {
        str = input_program[i];
        tokens = split(str, delim, &num_tokens);

        for (int j = 0; j < num_tokens; j++)
        {
            // detect what type of token it is
            if (is_instruction(tokens[j]))
            {
                tokenized_program[i][token_counter].token_type = 'f';
            }
            else if (starts_with(tokens[j], "0x") || is_number(tokens[j]))
            {
                tokenized_program[i][token_counter].token_type = 'i';
            }
            else if (is_register(tokens[j]))
            {
                tokenized_program[i][token_counter].token_type = 'r';
            }
            else if (starts_with(tokens[j], "#"))
            {
                tokenized_program[i][token_counter].token_type = 'm';
            }
            else
            {
                printf("\e[0;31merror: token with no type");
                return 1;
            }

            strcpy(tokenized_program[i][token_counter].value, tokens[j]);
            printf("%s, %c, %d, %d\n", tokenized_program[i][token_counter].value, tokenized_program[i][token_counter].token_type, token_counter, i);

            token_counter++;
        }
    }

    char compiled_program[program_size * 4];

    // loop through the tokenized program and turn it into bytes
    for (int i = 0; i < sizeof(tokenized_program) / 4; i++)
    {
        token current_line[4];
        for (int j = 0; j < 4; j++)
        {
            current_line[j] = tokenized_program[i][j];
            printf("%c\n", current_line[j].token_type);
            printf("%c\n", tokenized_program[i][j].token_type);
        }
    }

    return 0;
}
