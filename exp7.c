#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define a struct for the instruction
typedef struct {
    char op[10];
    char arg1[10];
    char arg2[10];
    char result[10];
} Instruction;

// Function to generate the final code
void generateFinalCode(char *expression) {
    int tempCount = 1;
    Instruction instructions[10];
    int instrCount = 0;

    // Parse the expression and generate instructions
    for (int i = 0; i < strlen(expression); i++) {
        if (isdigit(expression[i])) {
            // If it's a digit, it's an operand
            char operand[2] = { expression[i], '\0' };
            strcpy(instructions[instrCount].result, operand);
        } else {
            // If it's an operator, create an instruction
            instructions[instrCount].op[0] = expression[i];
            instructions[instrCount].op[1] = '\0';
            sprintf(instructions[instrCount].arg1, "t%d", tempCount - 1);
            sprintf(instructions[instrCount].arg2, "%c", expression[i + 1]);
            sprintf(instructions[instrCount].result, "t%d", tempCount);
            tempCount++;
            instrCount++;
            i++;
        }
    }

    // Print the generated instructions (final code)
    printf("Final Code Generation:\n");
    for (int i = 0; i < instrCount; i++) {
        printf("%s = %s %s %s\n", instructions[i].result, instructions[i].arg1, instructions[i].op, instructions[i].arg2);
    }
}

int main() {
    char expression[100];

    printf("Enter an arithmetic expression (e.g., 2+3*4): ");
    scanf("%s", expression);

    generateFinalCode(expression);

    return 0;
}

