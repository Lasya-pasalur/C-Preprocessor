## Preprocessor Implementation in C

## Project Description

This project implements a simplified C preprocessor using the C programming language.

It processes C source files and generates a preprocessed output file (`output.i`) by performing comment removal, header file inclusion, and macro substitution.

## Features

- **Comment Removal:** Removes single-line (`//`) and multi-line (`/* */`) comments.
- **File Inclusion:** Handles user-defined header file inclusion.
- **Object-like Macros:** Supports simple macro definitions such as `#define PI 3.14`.
- **Function-like Macros:** Supports macros with arguments such as `#define SQUARE(x) ((x)*(x))`.
- **Preprocessed Output:** Generates the final preprocessed code in `output.i`.

## Technologies Used

- C Programming
- GCC Compiler
- File Handling
- String Handling
- Structures and Pointers
- Makefile


## Project Structure

| File                                            | Description                                |
|-------------------------------------------------|--------------------------------------------|                                                       
|  main.c                                         | Controls the preprocessing stages          |
|  comment_removal.c                              | Removes comments from source files         |
|  file_inclusion.c                               | Handles user-defined header file inclusion |
|  macro_handler.c                                | Performs macro substitution                |
|  preprocessor.h                                 | Contains function declarations             |
|  header.h                                       | Contains macro definitions                 |
|  input.c                                        | Sample input C program                     |
|  makefile                                       | Compiles the project                       |
|  output.i                                       | Generated preprocessed output              |

## How to Compile

Compile the project using:

```command
make
```

## How to Execute

Run the program by providing the input C file as a command-line argument:

```command
./mypreprocessor input.c
```

## View the Output

To view the generated preprocessed file:

```command
cat output.i
```

## Preprocessing Flow

```text
Input C File
     |
     v
Comment Removal
     |
     v
File Inclusion
     |
     v
Macro Substitution
     |
     v
Preprocessed Output (output.i)
```

## Concepts Used

- File Handling
- String Manipulation
- Structures
- Pointers
- Functions
- Command-Line Arguments
- GCC Compilation and Makefile
