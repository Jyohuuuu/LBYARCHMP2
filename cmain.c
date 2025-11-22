#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

extern void asmMain(int rows, float (*matrix)[3], int (*matrix2));

int main() {
    int rows = 0, i;
    if (scanf_s("%d", &rows) != 1) {
        fprintf(stderr, "Failed to read number of rows\n");
        return 1;
    }

    float (*inputMatrix)[3] = malloc(rows * sizeof(*inputMatrix));
    if (inputMatrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    int (*ansMatrix) = malloc(rows * sizeof(*ansMatrix));
    if (ansMatrix == NULL) {
        fprintf(stderr, "Memory allocation for intArray failed\n");
        free(inputMatrix);
        return 1;
    }

    for (i = 0; i < rows; i++) {
        if (scanf_s("%f ,%f ,%f", &inputMatrix[i][0], &inputMatrix[i][1], &inputMatrix[i][2]) != 3) {
            fprintf(stderr, "Failed to read row %d\n", i);
            free(inputMatrix);
            return 1;
        }
    }

    asmMain(rows, inputMatrix, ansMatrix);
    printf("Integer acceleration values (m/s^2)\n");
    for (i = 0; i < rows; i++) {
        printf("%d\n", ansMatrix[i]);
	}
    free(inputMatrix);
	free(ansMatrix);
	
    return 0;
}
