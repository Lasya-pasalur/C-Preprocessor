#include <stdio.h>

int remove_comments(char input[], char output[])
{
    int ch, next;

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
        if(ch == '/')
        {
            // Read the next character to identify the type of comment
            next = fgetc(fp);

            // Skip single-line comments (//)
            if(next == '/')
            {
                while((ch = fgetc(fp)) != '\n' && ch != EOF);

                // Preserve the newline after the comment
                if(ch == '\n')
                    fputc('\n', fd);
            }

            // Skip multi-line comments (/* ... */)
            else if(next == '*')
            {
                int prev = 0;

                while((ch = fgetc(fp)) != EOF)
                {
                    // Stop when the closing comment symbol (*/) is found
                    if(prev == '*' && ch == '/')
                        break;

                    prev = ch;
                }
            }

            // If '/' is not part of a comment, write it to the output
            else
            {
                fputc(ch, fd);

                // Put the next character back for normal processing
                if(next != EOF)
                    ungetc(next, fp);
            }
        }
        else
        {
            // Copy all characters that are not part of comments
            fputc(ch, fd);
        }
    }

printf("Comments removed from %s successfully\n", input);
    // Close both files after processing
    fclose(fp);
    fclose(fd);

    return 0;
}
