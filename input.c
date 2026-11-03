#include"header.h"

int main()
{
    // Declare variables
    float radius = 5;

    // Calculate area using the PI macro
    float area = PI * radius * radius;

    // Substitute object-like macros
    int limit = MAX;
    int value = VALUE;

    // Substitute function-like macros with arguments
    int square = SQUARE(5);
    int sum = ADD(10, 20);

    // Display the results
    printf("Area = %f\n", area);
    printf("Maximum = %d\n", limit);
    printf("Value = %d\n", value);
    printf("Square = %d\n", square);
    printf("Sum = %d\n", sum);

    return 0;
}
