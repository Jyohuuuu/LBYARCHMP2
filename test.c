#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

extern void asmMain(int rows, float (*matrix)[3], int (*matrix2));

void c_equivalent(int rows, float (*matrix)[3], int (*matrix2)) {
    for (int i = 0; i < rows; i++) {
        float v_i = matrix[i][0];
        float v_f = matrix[i][1];
        float t = matrix[i][2];
        matrix2[i] = (int)roundf(((v_f - v_i) / t) * 1000.0 / 3600.0);
    }
    
}

int main() {
    int rows = 0, i;
    double asm_time, c_time;
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
        if (scanf_s("%f %f %f", &inputMatrix[i][0], &inputMatrix[i][1], &inputMatrix[i][2]) != 3) {
            fprintf(stderr, "Failed to read row %d\n", i);
            free(inputMatrix);
            return 1;
        }
    }
    // assembly equivalent with timing
    LARGE_INTEGER freq, start, end;
    double time_sum = 0;
    for (int j = 0; j < 30; j++) {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
        asmMain(rows, inputMatrix, ansMatrix);
        QueryPerformanceCounter(&end);

        double asmTime = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
        time_sum += asmTime;
        printf("Assembly time: %.12lf seconds\n", asmTime);
        
    }
    // printf("%.12f\n", time_sum);
    asm_time = (double)time_sum / 30;
    printf("Average Assembly time over 30 runs: %.12lf seconds\n", asm_time);
    printf("Integer acceleration values (m/s^2)\n");
    for (i = 0; i < rows; i++) {
        printf("%d\n", ansMatrix[i]);
	}




    // for c equivalent
     time_sum = 0;
    for (int j = 0; j < 30; j++) {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
        c_equivalent(rows, inputMatrix, ansMatrix);
        QueryPerformanceCounter(&end);

        double asmTime = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
        time_sum += asmTime;
        printf("C time: %.12lf seconds\n", asmTime);
        
    }
    c_time = (double)time_sum / 30;
    printf("Average C time over 30 runs: %.12lf seconds\n", c_time);
    
    for (int j = 0; j < rows; j++) {
        printf("C equivalent %d: %d\n", j, ansMatrix[j]);
    }
    printf("Integer acceleration values (m/s^2)\n");
    for (i = 0; i < rows; i++) {
        printf("%d\n", ansMatrix[i]);
	}
    
    if (c_time > asm_time) {
        printf("Assembly is faster, takes less time\n");
        printf("C: %.12lf > Assembly %.12lf\n", c_time, asm_time);
    } else if (asm_time > c_time) {
        printf("C is faster\n");
        printf("Assembly: %.12lf > C: %.12lf\n", asm_time,  c_time);
    } else {
        printf("Both have the same average time.\n");
    }
    free(inputMatrix);
	free(ansMatrix);
	
    


    return 0;
}