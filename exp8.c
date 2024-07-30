#include <stdio.h>
#include <stdlib.h>

int main() {
    char input[100];
    
    printf("Enter a number: ");
    scanf("%s", input);

    // Convert to int
    int intValue = atoi(input);
    // Convert to float
    float floatValue = atof(input);
    // Convert to double
    double doubleValue = strtod(input, NULL);

    // Display the results
    printf("Input as int: %d\n", intValue);
    printf("Input as float: %.2f\n", floatValue);
    printf("Input as double: %.2f\n", doubleValue);

    return 0;
}

