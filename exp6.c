#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Struct Definitions

typedef struct ASTNode {
    enum { BIN_OP, NUM, VARIABLE } type;
    union {
        struct { struct ASTNode *left; char op; struct ASTNode *right; } bin_op;
        int num;
        char *var_name;
    } data;
} ASTNode;

typedef struct {
    int temp_counter;
    int label_counter;
    char **instructions;
    int instruction_count;
} IntermediateCodeGenerator;

// Function Prototypes

ASTNode* create_bin_op(ASTNode *left, char op, ASTNode *right);
ASTNode* create_num(int value);
ASTNode* create_variable(char *name);
IntermediateCodeGenerator* create_generator();
char* generate_temp(IntermediateCodeGenerator *gen);
char* generate_label(IntermediateCodeGenerator *gen);
void add_instruction(IntermediateCodeGenerator *gen, const char *instruction);
void generate_code(IntermediateCodeGenerator *gen, ASTNode *ast);
char* visit(IntermediateCodeGenerator *gen, ASTNode *node);
char* visit_bin_op(IntermediateCodeGenerator *gen, ASTNode *node);
void free_ast(ASTNode *node);
void free_generator(IntermediateCodeGenerator *gen);
ASTNode* parse_expression(const char *expr, int *pos);

// Function Implementations

ASTNode* create_bin_op(ASTNode *left, char op, ASTNode *right) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = BIN_OP;
    node->data.bin_op.left = left;
    node->data.bin_op.op = op;
    node->data.bin_op.right = right;
    return node;
}

ASTNode* create_num(int value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NUM;
    node->data.num = value;
    return node;
}

ASTNode* create_variable(char *name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = VARIABLE;
    node->data.var_name = strdup(name);
    return node;
}

IntermediateCodeGenerator* create_generator() {
    IntermediateCodeGenerator *gen = (IntermediateCodeGenerator*)malloc(sizeof(IntermediateCodeGenerator));
    gen->temp_counter = 0;
    gen->label_counter = 0;
    gen->instructions = NULL;
    gen->instruction_count = 0;
    return gen;
}

char* generate_temp(IntermediateCodeGenerator *gen) {
    char *temp = (char*)malloc(10 * sizeof(char));
    sprintf(temp, "t%d", gen->temp_counter++);
    return temp;
}

char* generate_label(IntermediateCodeGenerator *gen) {
    char *label = (char*)malloc(10 * sizeof(char));
    sprintf(label, "L%d", gen->label_counter++);
    return label;
}

void add_instruction(IntermediateCodeGenerator *gen, const char *instruction) {
    gen->instructions = (char**)realloc(gen->instructions, (gen->instruction_count + 1) * sizeof(char*));
    gen->instructions[gen->instruction_count] = strdup(instruction);
    gen->instruction_count++;
}

void generate_code(IntermediateCodeGenerator *gen, ASTNode *ast) {
    visit(gen, ast);
}

char* visit(IntermediateCodeGenerator *gen, ASTNode *node) {
    if (node->type == BIN_OP) {
        return visit_bin_op(gen, node);
    } else if (node->type == NUM) {
        char *buffer = (char*)malloc(20 * sizeof(char));
        sprintf(buffer, "%d", node->data.num);
        return buffer;
    } else if (node->type == VARIABLE) {
        return strdup(node->data.var_name);
    }
    return NULL;
}

char* visit_bin_op(IntermediateCodeGenerator *gen, ASTNode *node) {
    char *left = visit(gen, node->data.bin_op.left);
    char *right = visit(gen, node->data.bin_op.right);
    char *result = generate_temp(gen);

    char instruction[100];
    sprintf(instruction, "%s = %s %c %s", result, left, node->data.bin_op.op, right);
    add_instruction(gen, instruction);

    free(left);
    free(right);

    return result;
}

void free_ast(ASTNode *node) {
    if (node->type == BIN_OP) {
        free_ast(node->data.bin_op.left);
        free_ast(node->data.bin_op.right);
    } else if (node->type == VARIABLE) {
        free(node->data.var_name);
    }
    free(node);
}

void free_generator(IntermediateCodeGenerator *gen) {
    for (int i = 0; i < gen->instruction_count; i++) {
        free(gen->instructions[i]);
    }
    free(gen->instructions);
    free(gen);
}

// Simple Parser for the Expression

ASTNode* parse_factor(const char *expr, int *pos) {
    while (isspace(expr[*pos])) (*pos)++;

    if (isdigit(expr[*pos])) {
        int value = 0;
        while (isdigit(expr[*pos])) {
            value = value * 10 + (expr[*pos] - '0');
            (*pos)++;
        }
        return create_num(value);
    } else if (expr[*pos] == '(') {
        (*pos)++;
        ASTNode *node = parse_expression(expr, pos);
        if (expr[*pos] == ')') {
            (*pos)++;
        }
        return node;
    } else {
        printf("Error: Unexpected character '%c'\n", expr[*pos]);
        exit(EXIT_FAILURE);
    }
}

ASTNode* parse_term(const char *expr, int *pos) {
    ASTNode *left = parse_factor(expr, pos);
    while (isspace(expr[*pos])) (*pos)++;
    while (expr[*pos] == '*') {
        char op = expr[*pos];
        (*pos)++;
        ASTNode *right = parse_factor(expr, pos);
        left = create_bin_op(left, op, right);
    }
    return left;
}

ASTNode* parse_expression(const char *expr, int *pos) {
    ASTNode *left = parse_term(expr, pos);
    while (isspace(expr[*pos])) (*pos)++;
    while (expr[*pos] == '+') {
        char op = expr[*pos];
        (*pos)++;
        ASTNode *right = parse_term(expr, pos);
        left = create_bin_op(left, op, right);
    }
    return left;
}

int main() {
    char input[256];
    printf("Enter an arithmetic expression: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove the newline character

    int pos = 0;
    ASTNode *ast = parse_expression(input, &pos);

    // Generate intermediate code
    IntermediateCodeGenerator *generator = create_generator();
    generate_code(generator, ast);

    // Print intermediate code
    for (int i = 0; i < generator->instruction_count; i++) {
        printf("%s\n", generator->instructions[i]);
    }

    // Free allocated memory
    free_ast(ast);
    free_generator(generator);

    return 0;
}

