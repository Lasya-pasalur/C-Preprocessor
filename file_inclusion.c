#include <stdio.h>
#include <string.h>
#include "preprocessor.h"

int file_inclusion(char input[], char output[])
{
    char line[200], filename[50], temp[100];
    FILE *fp, *fd, *hp;

    // Open input and output files
    fp = fopen(input, "r");
    fd = fopen(output, "w");

    if(fp == NULL || fd == NULL)
    {
        printf("File opening failed\n");

        if(fp != NULL)
            fclose(fp);

        if(fd != NULL)
            fclose(fd);

        return 1;
    }

    // Read the input file line by line
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        // Check whether the current line contains an include directive
        if(strncmp(line, "#include", 8) == 0)
        {
            char *start = strchr(line, '"');

            // Process only user-defined header files
            if(start != NULL)
            {
                start++;

                char *end = strchr(start, '"');

                if(end != NULL)
                {
                    // Extract the header filename
                    int len = end - start;

                    if(len >= sizeof(filename))
                    {
                        printf("Header filename too long\n");
                        fclose(fp);
                        fclose(fd);
                        return 1;
                    }

                    strncpy(filename, start, len);
                    filename[len] = '\0';

                    // Generate a temporary filename for the comment-free header
                    snprintf(temp, sizeof(temp), "header_temp.c");

                    // Remove comments from the included header file
                    if(remove_comments(filename, temp) != 0)
                    {
                        fclose(fp);
                        fclose(fd);
                        return 1;
                    }

                    // Open the processed header file
                    hp = fopen(temp, "r");

                    if(hp == NULL)
                    {
                        printf("Header file opening failed\n");
                        fclose(fp);
                        fclose(fd);
                        return 1;
                    }

                    // Copy the header contents into the output file
                    int ch;

                    while((ch = fgetc(hp)) != EOF)
                    {
                        fputc(ch, fd);
                    }

                    fclose(hp);
                }
                else
                {
                    // Preserve the line if the closing quote is missing
                    fputs(line, fd);
                }
            }
            else
            {
                // Preserve standard header files such as <stdio.h>
                fputs(line, fd);
            }
        }
        else
        {
            // Copy normal source code lines unchanged
            fputs(line, fd);
        }
    }

    printf("File inclusion completed successfully\n");

    // Close input and output files
    fclose(fp);
    fclose(fd);

    return 0;
}
