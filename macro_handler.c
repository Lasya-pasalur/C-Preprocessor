#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Maximum number of macros
#define MAX_MACROS 100
#define MAX_PARAMS 10

// Structure to store macro information
struct macro
{
    char name[50];
    char value[200];
    char params[MAX_PARAMS][50];
    int param_count;
    int is_function;
};

// Check whether a character can start an identifier
int is_start(char ch)
{
    return isalpha((unsigned char)ch) || ch == '_';
}

// Check whether a character can be part of an identifier
int is_part(char ch)
{
    return isalnum((unsigned char)ch) || ch == '_';
}

// Remove leading and trailing spaces
void trim(char str[])
{
    int i = 0;
    int len;

    while(isspace((unsigned char)str[i]))
        i++;

    if(i > 0)
        memmove(str, str + i, strlen(str + i) + 1);

    len = strlen(str);

    while(len > 0 && isspace((unsigned char)str[len - 1]))
        str[--len] = '\0';
}

// Read and store a macro definition
int read_macro(char line[], struct macro *m)
{
    char *p = line;
    int i = 0;

    while(isspace((unsigned char)*p))
        p++;

    if(strncmp(p, "#define", 7) != 0)
        return 0;

    p += 7;

    if(!isspace((unsigned char)*p))
        return 0;

    while(isspace((unsigned char)*p))
        p++;

    // Read macro name
    if(!is_start(*p))
        return 0;

    while(is_part(*p))
    {
        if(i < 49)
            m->name[i++] = *p;

        p++;
    }

    m->name[i] = '\0';

    m->param_count = 0;
    m->is_function = 0;

    // Check whether it is a function-like macro
    if(*p == '(')
    {
        m->is_function = 1;
        p++;

        while(*p != ')' && *p != '\0')
        {
            while(isspace((unsigned char)*p) || *p == ',')
                p++;

            if(*p == ')')
                break;

            i = 0;

            while(is_part(*p))
            {
                if(i < 49)
                    m->params[m->param_count][i++] = *p;

                p++;
            }

            m->params[m->param_count][i] = '\0';

            if(m->param_count >= MAX_PARAMS - 1)
                return 0;

            m->param_count++;
        }

        if(*p != ')')
            return 0;

        p++;
    }

    // Read macro replacement value
    while(*p == ' ' || *p == '\t')
        p++;

    strncpy(m->value, p, sizeof(m->value) - 1);
    m->value[sizeof(m->value) - 1] = '\0';

    trim(m->value);

    return 1;
}

// Read the actual arguments passed to a function-like macro
int read_arguments(char **p, char args[MAX_PARAMS][100])
{
    int depth = 0;
    int count = 0;
    int i = 0;

    (*p)++;

    while(**p != '\0')
    {
        char ch = **p;

        if(ch == '(')
        {
            depth++;
        }
        else if(ch == ')')
        {
            if(depth == 0)
            {
                args[count][i] = '\0';
                trim(args[count]);

                if(count == 0 && args[count][0] == '\0')
                    return 0;

                return count + 1;
            }

            depth--;
        }
        else if(ch == ',' && depth == 0)
        {
            args[count][i] = '\0';
            trim(args[count]);

            count++;
            i = 0;

            if(count >= MAX_PARAMS)
                return -1;

            (*p)++;
            continue;
        }

        if(i < 99)
            args[count][i++] = ch;

        (*p)++;
    }

    return -1;
}

// Replace formal parameters with actual arguments
void substitute_function(FILE *fd, struct macro *m,
                         char args[MAX_PARAMS][100])
{
    char word[100];
    int i = 0, j, k, found;

    while(m->value[i] != '\0')
    {
        if(is_start(m->value[i]))
        {
            j = 0;

            while(is_part(m->value[i]))
            {
                if(j < 99)
                    word[j++] = m->value[i];

                i++;
            }

            word[j] = '\0';
            found = 0;

            for(k = 0; k < m->param_count; k++)
            {
                if(strcmp(word, m->params[k]) == 0)
                {
                    fputs(args[k], fd);
                    found = 1;
                    break;
                }
            }

            if(found == 0)
                fputs(word, fd);
        }
        else
        {
            fputc(m->value[i], fd);
            i++;
        }
    }
}

// Perform macro substitution
int macro_substitution(char input[], char output[])
{
    char line[1000], word[100];
    char args[MAX_PARAMS][100];

    struct macro m[MAX_MACROS];

    int count = 0, i, j, k, found;
    int arg_count;

    FILE *fp = fopen(input, "r");

    if(fp == NULL)
    {
        printf("Input file opening failed\n");
        return 1;
    }

    // First pass: Read and store macro definitions
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        struct macro temp;

        if(read_macro(line, &temp))
        {
            if(count >= MAX_MACROS)
            {
                printf("Maximum macro limit reached\n");
                fclose(fp);
                return 1;
            }

            m[count++] = temp;
        }
    }

    // Move the file pointer to the beginning
    rewind(fp);

    // Open output file
    FILE *fd = fopen(output, "w");

    if(fd == NULL)
    {
        printf("Output file opening failed\n");
        fclose(fp);
        return 1;
    }

    // Second pass: Perform macro substitution
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        char *p = line;

        // Skip macro definition lines
        while(isspace((unsigned char)*p))
            p++;

        if(strncmp(p, "#define", 7) == 0)
            continue;

        i = 0;

        // Read the line character by character
        while(line[i] != '\0')
        {
            // Check for the beginning of an identifier
            if(is_start(line[i]))
            {
                j = 0;

                // Extract the complete identifier
                while(is_part(line[i]))
                {
                    if(j < 99)
                        word[j++] = line[i];

                    i++;
                }

                word[j] = '\0';
                found = 0;

                // Search for the identifier in stored macros
                for(k = 0; k < count; k++)
                {
                    if(strcmp(word, m[k].name) == 0)
                    {
                        // Handle function-like macros
                        if(m[k].is_function)
                        {
                            char *q = line + i;

                            while(isspace((unsigned char)*q))
                                q++;

                            if(*q == '(')
                            {
                                arg_count = read_arguments(&q, args);

                                if(arg_count == m[k].param_count)
                                {
                                    substitute_function(fd, &m[k], args);
                                    i = q - line;
                                    found = 1;
                                    break;
                                }
                            }
                        }
                        // Handle simple object-like macros
                        else
                        {
                            fputs(m[k].value, fd);
                            found = 1;
                            break;
                        }
                    }
                }

                // If no macro is found, write the original identifier
                if(found == 0)
                    fputs(word, fd);
            }
            else
            {
                fputc(line[i], fd);
                i++;
            }
        }
    }

    printf("Macro substitution completed successfully\n");

    fclose(fp);
    fclose(fd);

    return 0;
}
