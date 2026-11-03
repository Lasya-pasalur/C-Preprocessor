#include <stdio.h>
#include "preprocessor.h"

// Main function: accepts the input filename through command-line arguments
int main(int argc, char *argv[])
{
    // Check whether the user has provided exactly one input filename
    if(argc != 2)
    {
        printf("Usage: ./mypreprocessor <input_file>\n");
        return 1;
    }

    // Step 1: Remove single-line and multi-line comments
    // Store the comment-free contents in temp1.c
    if(remove_comments(argv[1], "temp1.c") != 0)
        return 1;

    // Step 2: Process header file inclusion
    // Store the included header contents in temp2.c
    if(file_inclusion("temp1.c", "temp2.c") != 0)
        return 1;

    // Step 3: Perform macro substitution
    // Generate the final preprocessed output in output.i
    if(macro_substitution("temp2.c", "output.i") != 0)
        return 1;

    // Display success message after all preprocessing stages are completed
    printf("Preprocessing completed successfully\n");

    return 0;
}
