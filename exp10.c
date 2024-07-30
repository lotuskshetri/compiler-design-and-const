#include <stdio.h>
#include <string.h>

// Function to check if the given string is a comment or not
void isComment(char *line) {
    int len = strlen(line);

    // Check for single-line comment
    if (len >= 2 && line[0] == '/' && line[1] == '/') {
        printf("It is a single-line comment");
        return;
    }

    // Check for multi-line comment
    if (len >= 4 && line[len - 2] == '*' && line[len - 1] == '/' && line[0] == '/' && line[1] == '*') {
        printf("It is a multi-line comment");
        return;
    }

    printf("It is not a comment");
}

// Driver code
int main() {
    // Variable to hold the input string
    char line[256];

    // Prompt the user for input
    printf("Enter a string: ");
    fgets(line, sizeof(line), stdin);

    // Remove the newline character at the end if present
    line[strcspn(line, "\n")] = 0;

    // Function call to check whether the given string is a comment or not
    isComment(line);

    return 0;
}

