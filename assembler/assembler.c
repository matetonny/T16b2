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
    token **tokenized_program = (token **)malloc(counter * sizeof(token *));
    for (int b = 0; b < counter; ++b)
    {
        tokenized_program[b] = (token *)malloc(4 * sizeof(token));
    }

    // loop through the input program
    for (int i = 0; i < counter + 1; i++)
    {
        str = input_program[i];
        tokens = split(str, delim, &num_tokens);

        for (int j = 0; j < num_tokens; j++)
        {
            // detect what type of token it is
            if (is_instruction(tokens[j]))
            {
                tokenized_program[i][j].token_type = 'f';
            }
            else if (starts_with(tokens[j], "0x") || is_number(tokens[j]))
            {
                tokenized_program[i][j].token_type = 'i';
            }
            else if (is_register(tokens[j]))
            {
                tokenized_program[i][j].token_type = 'r';
            }
            else if (starts_with(tokens[j], "#"))
            {
                tokenized_program[i][j].token_type = 'm';
            }
            else
            {
                printf("\e[0;31merror: token with no type [%d]", i);
                return 1;
            }

            strcpy(tokenized_program[i][j].value, tokens[j]);
        }
    }

    // define variables to compile the program
    char compiled_program[program_size * 4];
    char current_line_compiled[4];

    // loop through the tokenized program
    for (int i = 0; i < counter; i++)
    {
        if (tokenized_program[i][0].token_type != 'f')
        {
            printf("\e[0;31merror: unexpected token [%d]", i);
            return 1;
        }

        if (strcmp(tokenized_program[i][0].value, "lda") == 0)
        {
            compiled_program[i * 4] = 0x0;
        }
        else if (strcmp(tokenized_program[i][0].value, "ldb") == 0)
        {
            compiled_program[i * 4] = 0x1;
        }
        else if (strcmp(tokenized_program[i][0].value, "ldo") == 0)
        {
            compiled_program[i * 4] = 0x2;
        }
        else if (strcmp(tokenized_program[i][0].value, "ldf") == 0)
        {
            compiled_program[i * 4] = 0x3;
        }
        else if (strcmp(tokenized_program[i][0].value, "sta") == 0)
        {
            compiled_program[i * 4] = 0x4;
        }
        else if (strcmp(tokenized_program[i][0].value, "stb") == 0)
        {
            compiled_program[i * 4] = 0x5;
        }
        else if (strcmp(tokenized_program[i][0].value, "ldr") == 0)
        {
            compiled_program[i * 4] = 0x6;
        }
        else if (strcmp(tokenized_program[i][0].value, "str") == 0)
        {
            compiled_program[i * 4] = 0x7;
        }
        else if (strcmp(tokenized_program[i][0].value, "add") == 0)
        {
            compiled_program[i * 4] = 0x8;
        }
        else if (strcmp(tokenized_program[i][0].value, "sub") == 0)
        {
            compiled_program[i * 4] = 0x9;
        }
        else if (strcmp(tokenized_program[i][0].value, "mul") == 0)
        {
            compiled_program[i * 4] = 0xa;
        }
        else if (strcmp(tokenized_program[i][0].value, "div") == 0)
        {
            compiled_program[i * 4] = 0xb;
        }
        else if (strcmp(tokenized_program[i][0].value, "and") == 0)
        {
            compiled_program[i * 4] = 0xc;
        }
        else if (strcmp(tokenized_program[i][0].value, "or") == 0)
        {
            compiled_program[i * 4] = 0xd;
        }
        else if (strcmp(tokenized_program[i][0].value, "not") == 0)
        {
            compiled_program[i * 4] = 0xe;
        }
        else if (strcmp(tokenized_program[i][0].value, "cmp") == 0)
        {
            compiled_program[i * 4] = 0xf;
        }
        else if (strcmp(tokenized_program[i][0].value, "jmp") == 0)
        {
            compiled_program[i * 4] = 0x10;
        }
        else if (strcmp(tokenized_program[i][0].value, "jmfz") == 0)
        {
            compiled_program[i * 4] = 0x11;
        }
        else if (strcmp(tokenized_program[i][0].value, "jmff") == 0)
        {
            compiled_program[i * 4] = 0x12;
        }
        else if (strcmp(tokenized_program[i][0].value, "push") == 0)
        {
            compiled_program[i * 4] = 0x13;
        }
        else if (strcmp(tokenized_program[i][0].value, "pop") == 0)
        {
            compiled_program[i * 4] = 0x14;
        }
        else if (strcmp(tokenized_program[i][0].value, "hlt") == 0)
        {
            compiled_program[i * 4] = 0x15;
        }

        for (int j = 0; j < 4; j++)
        {

            if (tokenized_program[i][j].value[0] != '\0')
            {
                printf("Token %d, Value %s, Type: %c\n", j, tokenized_program[i][j].value, tokenized_program[i][j].token_type);
            }
        }
    }

    // free allocated memory
    for (int b = 0; b < counter; ++b)
    {
        free(tokenized_program[b]);
    }
    free(tokenized_program);

    return 0;
}
