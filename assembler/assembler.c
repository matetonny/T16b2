#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"

int main(int argc, char const *argv[])
{
    bool debug = false;

    if (argc == 1)
    {
        printf("\e[0;31merror: No file given.");
        return 1;
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "-d") == 0 || strcmp(argv[2], "-d") == 0)
        {
            debug = true;
        }
    }

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
    bool has_diri;
    char fer;
    char ser;
    char reg1;
    char reg2;
    char reg[4];

    // loop through the tokenized program
    for (int i = 0; i < counter; i++)
    {
        // check if first token is an instruction
        if (tokenized_program[i][0].token_type != 'f')
        {
            printf("\e[0;31merror: unexpected token [%d]", i);
            return 1;
        }

        // reset variables
        has_diri = false;
        reg1 = 50;
        reg2 = 50;
        current_line_compiled[0] = 0;
        current_line_compiled[1] = 0;
        current_line_compiled[2] = 0;
        current_line_compiled[3] = 0;

        // loop through all tokens in the line for compiling
        for (int j = 0; j < 4; j++)
        {
            if (tokenized_program[i][j].value[0] != '\0')
            {
                // debug current token
                if (debug)
                {
                    printf("Token %d, Value %s, Type: %c\n", j, tokenized_program[i][j].value, tokenized_program[i][j].token_type);
                }

                if (tokenized_program[i][j].token_type == 'i')
                {
                    split_uint16(string_to_uint16(tokenized_program[i][j].value), &current_line_compiled[1], &current_line_compiled[2]);
                    has_diri = true;
                }
                else if (tokenized_program[i][j].token_type == 'r')
                {
                    strncpy(reg, tokenized_program[i][j].value, 4);

                    if (reg1 == 50)
                    {
                        assign_reg(reg, &reg1);
                    }
                    else if (reg2 == 50)
                    {
                        assign_reg(reg, &reg2);
                    }
                    else
                    {
                        printf("\e[0;31merror: unexpected token [%d]", i);
                        return 1;
                    }
                }
            }
        }

        // detect instuction
        if (strcmp(tokenized_program[i][0].value, "lda") == 0)
        {
            current_line_compiled[0] = 0x0;
            fer = 0;
            ser = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "ldb") == 0)
        {
            current_line_compiled[0] = 0x1;
            fer = 1;
            ser = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "ldo") == 0)
        {
            current_line_compiled[0] = 0x2;
            fer = 2;
            ser = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "ldf") == 0)
        {
            current_line_compiled[0] = 0x3;
            fer = 5;
            ser = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "sta") == 0)
        {
            current_line_compiled[0] = 0x4;
            fer = 0;
            ser = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "stb") == 0)
        {
            current_line_compiled[0] = 0x5;
            fer = 1;
            ser = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "ldr") == 0)
        {
            current_line_compiled[0] = 0x6;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "str") == 0)
        {
            current_line_compiled[0] = 0x7;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "add") == 0)
        {
            current_line_compiled[0] = 0x8;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "sub") == 0)
        {
            current_line_compiled[0] = 0x9;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "mul") == 0)
        {
            current_line_compiled[0] = 0xa;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "div") == 0)
        {
            current_line_compiled[0] = 0xb;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "and") == 0)
        {
            current_line_compiled[0] = 0xc;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "or") == 0)
        {
            current_line_compiled[0] = 0xd;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "not") == 0)
        {
            current_line_compiled[0] = 0xe;
            fer = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "cmp") == 0)
        {
            current_line_compiled[0] = 0xf;
            fer = reg1;
            ser = reg2;
        }
        else if (strcmp(tokenized_program[i][0].value, "jmp") == 0)
        {
            current_line_compiled[0] = 0x10;
            fer = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "jmfz") == 0)
        {
            current_line_compiled[0] = 0x11;
            fer = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "jmff") == 0)
        {
            current_line_compiled[0] = 0x12;
            fer = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "push") == 0)
        {
            current_line_compiled[0] = 0x13;
            fer = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "pop") == 0)
        {
            current_line_compiled[0] = 0x14;
            fer = reg1;
        }
        else if (strcmp(tokenized_program[i][0].value, "hlt") == 0)
        {
            current_line_compiled[0] = 0x15;
        }

        current_line_compiled[3] = (fer << 4) | (ser << 1) | has_diri;

        // debug current line
        if (debug)
        {
            printf("result:\n");
            for (int v = 7; v >= 0; v--)
            {
                printf("%d", (current_line_compiled[0] >> v) & 1);
            }
            printf("\n");
            for (int v = 7; v >= 0; v--)
            {
                printf("%d", (current_line_compiled[1] >> v) & 1);
            }
            printf("\n");
            for (int v = 7; v >= 0; v--)
            {
                printf("%d", (current_line_compiled[2] >> v) & 1);
            }
            printf("\n");
            for (int v = 7; v >= 0; v--)
            {
                printf("%d", (current_line_compiled[3] >> v) & 1);
            }
            printf("\n");
        }

        // write the current line to the compiled program
        compiled_program[i * 4] = current_line_compiled[0];
        compiled_program[i * 4 + 1] = current_line_compiled[1];
        compiled_program[i * 4 + 2] = current_line_compiled[2];
        compiled_program[i * 4 + 3] = current_line_compiled[3];
    }

    // write to output file
    FILE *output_pfile;

    output_pfile = fopen("output.hex", "wb");

    fwrite(&compiled_program, 1, counter * 4, output_pfile);

    fclose(output_pfile);

    // free memory
    for (int b = 0; b < counter; ++b)
    {
        free(tokenized_program[b]);
    }
    free(tokenized_program);

    return 0;
}