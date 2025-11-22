#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

extern void asmMain(int rows, float (*matrix)[3], int (*matrix2));

void c_equivalent(int rows, float (*matrix)[3], int (*matrix2)) {
    int i;
    for (i = 0; i < rows; i++) {
        float v_i = matrix[i][0];
        float v_f = matrix[i][1];
        float t = matrix[i][2];
        matrix2[i] = (int)roundf(((v_f - v_i) / t) * 1000.0 / 3600.0);
    }
}

int main() {
    int rows, i, j;
    if (scanf_s("%d", &rows) != 1) {
        fprintf(stderr, "Failed to read number of rows\n");
        return 1;
    }

    float (*inputMatrix)[3] = malloc(rows * sizeof(*inputMatrix));
    int *ansMatrixASM = malloc(rows * sizeof(*ansMatrixASM));
    int *ansMatrixC   = malloc(rows * sizeof(*ansMatrixC));

    if (!inputMatrix || !ansMatrixASM || !ansMatrixC) {
        fprintf(stderr, "Memory allocation failed\n");
        free(inputMatrix); free(ansMatrixASM); free(ansMatrixC);
        return 1;
    }

    for (i = 0; i < rows; i++) {
        if (scanf_s("%f, %f, %f", &inputMatrix[i][0], &inputMatrix[i][1], &inputMatrix[i][2]) != 3) {
            fprintf(stderr, "Failed to read row %d\n", i);
            free(inputMatrix); free(ansMatrixASM); free(ansMatrixC);
            return 1;
        }
    }

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    double asm_time_sum = 0.0;
    for (j = 0; j < 30; j++) {
        QueryPerformanceCounter(&start);
        asmMain(rows, inputMatrix, ansMatrixASM);
        QueryPerformanceCounter(&end);

        double asmTime = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
        asm_time_sum += asmTime;
        printf("Assembly run %d time: %.12lf seconds  \n", j + 1, asmTime);
    }
    double avgAsmTime = asm_time_sum / 30.0;
    printf("Average Assembly time: %.12lf seconds  \n\n", avgAsmTime);

    double c_time_sum = 0.0;
    for (j = 0; j < 30; j++) {
        QueryPerformanceCounter(&start);
        c_equivalent(rows, inputMatrix, ansMatrixC);
        QueryPerformanceCounter(&end);

        double cTime = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
        c_time_sum += cTime;
        printf("C run %d time: %.12lf seconds  \n", j + 1, cTime);
    }
    double avgCTime = c_time_sum / 30.0;
    printf("Average C time: %.12lf seconds  \n\n", avgCTime);

    printf("ASM results (m/s^2): ");
    for (i = 0; i < rows; i++) printf("%d ", ansMatrixASM[i]);

    printf("\nC results (m/s^2): ");
    for (i = 0; i < rows; i++) printf("%d ", ansMatrixC[i]);

    if (avgAsmTime < avgCTime)
        printf("\nAssembly is faster on average: %.12lf < %.12lf\n", avgAsmTime, avgCTime);
    else if (avgCTime < avgAsmTime)
        printf("\nC is faster on average: %.12lf < %.12lf\n", avgCTime, avgAsmTime);
    else
        printf("\nBoth have the same average time.\n");

    free(inputMatrix); free(ansMatrixASM); free(ansMatrixC);
    return 0;
}
