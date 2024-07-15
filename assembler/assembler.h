#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define program_size 65536

// define structs
typedef struct token
{
    char token_type;
    char value[100];
} token;

typedef struct Instruction
{
    char instruction;
    u_int16_t diri;
    char fer;
    char ser;
    bool rod;
} Instruction;

// function to split a string by a given delimiter
char **split(char *str, const char *delim, int *num_tokens)
{
    // make a copy of the input string, as strtok modifies the string it processes
    char *str_copy = strdup(str);
    if (str_copy == NULL)
    {
        perror("strdup");
        exit(1);
    }

    // count the number of tokens
    int count = 0;
    char *temp = strdup(str); // temporary copy to count tokens
    if (temp == NULL)
    {
        perror("strdup");
        exit(1);
    }

    char *token = strtok(temp, delim);
    while (token != NULL)
    {
        count++;
        token = strtok(NULL, delim);
    }
    free(temp);

    // allocate memory for the array of token pointers
    char **tokens = malloc((count + 1) * sizeof(char *)); // +1 for the NULL terminator
    if (tokens == NULL)
    {
        perror("malloc");
        exit(1);
    }

    // tokenize the input string and fill the tokens array
    int index = 0;
    token = strtok(str_copy, delim);
    while (token != NULL)
    {
        tokens[index] = strdup(token);
        if (tokens[index] == NULL)
        {
            perror("strdup");
            exit(1);
        }
        index++;
        token = strtok(NULL, delim);
    }
    tokens[index] = NULL; // null-terminate the array

    // free the copied string
    free(str_copy);

    // set the number of tokens output parameter
    *num_tokens = count;

    return tokens;
}

// Function to free the memory allocated for the tokens array
void free_tokens(char **tokens)
{
    int i = 0;
    while (tokens[i] != NULL)
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}

// function to check if a string is in the instruction set
bool is_instruction(char str[])
{
    // define instruction set and check if str is in it
    char instruction_set[][4] = {"lda", "ldb", "ldo", "ldf", "sta", "stb", "ldr", "str", "add", "sub", "mul", "div", "and", "or", "not", "cmp", "jmp", "jpfz", "jpff", "push", "pop", "hlt"};

    for (int a = 0; a < sizeof(instruction_set) / 4; a++)
    {
        if (strcmp(instruction_set[a], str) == 0)
        {
            return true;
        }
    }

    return false;
}

// function to check if a string starts with another string
bool starts_with(const char *a, const char *b)
{
    if (strncmp(a, b, strlen(b)) == 0)
    {
        return 1;
    }
    return 0;
}

// function to check if a string is a valid number
bool is_number(const char *str)
{
    // check if the string is empty
    if (*str == '\0')
    {
        return false;
    }

    // check if the string is a number
    bool hasDigits = false;

    while (*str)
    {
        if (isdigit(*str))
        {
            hasDigits = true;
        }
        else
        {
            return false;
        }
        str++;
    }

    // return true only if there were digits in the string
    return hasDigits;
}

// function to check if a string is a register name
bool is_register(char str[])
{
    // define instruction set and check if str is in it
    char registers[][4] = {"@ao", "@a", "@b", "@ma", "@pm", "@fl", "@sp", "@bk"};

    for (int a = 0; a < sizeof(registers) / 4; a++)
    {
        if (strcmp(registers[a], str) == 0)
        {
            return true;
        }
    }

    return false;
}

// function to split uint16
void split_uint16(uint16_t input, char *high_byte, char *low_byte)
{
    *high_byte = (char)((input >> 8) & 0xFF); // Extract high byte
    *low_byte = (char)(input & 0xFF);         // Extract low byte
}

// function to turn string into a number
uint16_t string_to_uint16(const char *str)
{
    char *endptr;
    int base = 10; // Default base

    // Check if the string starts with "0x" or "0X" to identify it as hexadecimal
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        base = 16;
    }

    return (uint16_t)strtol(str, &endptr, base);
}

u_int16_t mergeCharsToUInt16(char highByte, char lowByte)
{
    u_int16_t result = ((u_int16_t)(highByte & 0xFF) << 8) | (lowByte & 0xFF);

    return result;
}