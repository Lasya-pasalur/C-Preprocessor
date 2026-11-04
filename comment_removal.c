#include <stdio.h>

int remove_comments(char input[], char output[])
{
    int ch, next;
    int in_string = 0;
    int in_char = 0;

    // Open source file in read mode and output file in write mode
    FILE *fp = fopen(input, "r");
    FILE *fd = fopen(output, "w");

    // Check whether both files are opened successfully
    if(fp == NULL || fd == NULL)
    {
        printf("File opening failed\n");

        if(fp != NULL)
            fclose(fp);

        if(fd != NULL)
            fclose(fd);

        return 1;
    }

    // Read the input file character by character
    while((ch = fgetc(fp)) != EOF)
    {
        // Check whether inside string or character constant
        if(in_string || in_char)
        {
            fputc(ch, fd);

            // Handle escape sequences
            if(ch == '\\')
            {
                next = fgetc(fp);

                if(next != EOF)
                    fputc(next, fd);
            }
            // Check closing double quote
            else if(ch == '"' && in_string)
            {
                in_string = 0;
            }
            // Check closing single quote
            else if(ch == '\'' && in_char)
            {
                in_char = 0;
            }
        }

        // Check beginning of string literal
        else if(ch == '"')
        {
            in_string = 1;
            fputc(ch, fd);
        }

        // Check beginning of character constant
        else if(ch == '\'')
        {
            in_char = 1;
            fputc(ch, fd);
        }

        // Check for comment symbols
        else if(ch == '/')
        {
            next = fgetc(fp);

            // Single-line comment
            if(next == '/')
            {
                while((ch = fgetc(fp)) != '\n' && ch != EOF);

                // Preserve newline
                if(ch == '\n')
                    fputc('\n', fd);
            }

            // Multi-line comment
            else if(next == '*')
            {
                int prev = 0;

                while((ch = fgetc(fp)) != EOF)
                {
                    // Identify closing comment delimiter
                    if(prev == '*' && ch == '/')
                        break;

                    prev = ch;
                }
            }

            // Normal forward slash
            else
            {
                fputc(ch, fd);

                // Put back the next character
                if(next != EOF)
                    ungetc(next, fp);
            }
        }

        // Copy all normal characters
        else
        {
            fputc(ch, fd);
        }
    }

    printf("Comments removed from %s successfully\n", input);

    // Close both files
    fclose(fp);
    fclose(fd);

    return 0;
}
