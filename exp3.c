#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Functions to calculate Follow
void followfirst(char, int, int);
void follow(char c);

int count, n = 0;

// Stores the final result of the Follow Sets
char calc_follow[10][100];
int m = 0;

// Stores the production rules
char production[10][10];
char f[10];
char ck;
int e;

int main(int argc, char** argv)
{
    int km = 0;
    int i;
    count = 8;

    // The Input grammar
    strcpy(production[0], "X=TnS");
    strcpy(production[1], "X=Rm");
    strcpy(production[2], "T=q");
    strcpy(production[3], "T=#");
    strcpy(production[4], "S=p");
    strcpy(production[5], "S=#");
    strcpy(production[6], "R=om");
    strcpy(production[7], "R=ST");

    int kay;
    char done[count];
    int ptr = -1;

    // Initializing the calc_follow array
    for (int k = 0; k < count; k++) {
        for (kay = 0; kay < 100; kay++) {
            calc_follow[k][kay] = '!';
        }
    }
    int point1 = 0;
    int point2;
    int xxx;
    int land = 0;

    for (e = 0; e < count; e++) {
        ck = production[e][0];
        point2 = 0;
        xxx = 0;

        // Checking if Follow of ck has already been calculated
        for (kay = 0; kay <= ptr; kay++)
            if (ck == done[kay])
                xxx = 1;

        if (xxx == 1)
            continue;
        land += 1;

        // Function call
        follow(ck);
        ptr += 1;

        // Adding ck to the calculated list
        done[ptr] = ck;
        printf(" Follow(%c) = { ", ck);
        calc_follow[point1][point2++] = ck;

        // Printing the Follow Sets of the grammar
        for (i = 0 + km; i < m; i++) {
            int lark = 0, chk = 0;
            for (lark = 0; lark < point2; lark++) {
                if (f[i] == calc_follow[point1][lark]) {
                    chk = 1;
                    break;
                }
            }
            if (chk == 0) {
                printf("%c, ", f[i]);
                calc_follow[point1][point2++] = f[i];
            }
        }
        printf(" }\n\n");
        km = m;
        point1++;
    }
}

void follow(char c)
{
    int i, j;

    // Adding "$" to the follow set of the start symbol
    if (production[0][0] == c) {
        f[m++] = '$';
    }
    for (i = 0; i < 10; i++) {
        for (j = 2; j < 10; j++) {
            if (production[i][j] == c) {
                if (production[i][j + 1] != '\0') {
                    // Calculate the first of the next Non-Terminal in the production
                    followfirst(production[i][j + 1], i, (j + 2));
                }

                if (production[i][j + 1] == '\0' && c != production[i][0]) {
                    // Calculate the follow of the Non-Terminal in the L.H.S. of the production
                    follow(production[i][0]);
                }
            }
        }
    }
}

void followfirst(char c, int c1, int c2)
{
    int k;

    // The case where we encounter a Terminal
    if (!(isupper(c)))
        f[m++] = c;
    else {
        int i = 0, j = 1;
        for (i = 0; i < count; i++) {
            if (calc_follow[i][0] == c)
                break;
        }

        // Including the First set of the Non-Terminal in the Follow of the original query
        while (calc_follow[i][j] != '!') {
            if (calc_follow[i][j] != '#') {
                f[m++] = calc_follow[i][j];
            } else {
                if (production[c1][c2] == '\0') {
                    // Case where we reach the end of a production
                    follow(production[c1][0]);
                } else {
                    // Recursion to the next symbol in case we encounter a "#"
                    followfirst(production[c1][c2], c1, c2 + 1);
                }
            }
            j++;
        }
    }
}

