# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall

# Name of the executable file
TARGET = mypreprocessor

# Source files required to build the project
SOURCES = main.c comment_removal.c file_inclusion.c macro_handler.c

# Compile all source files and generate the executable
all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# Remove the executable and intermediate output files
clean:
	rm -f $(TARGET) temp1.c temp2.c header_temp.c output.i
