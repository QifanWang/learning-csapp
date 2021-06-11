/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, ii, jj, a0, a1, a2, a3, a4, a5, a6, a7;
  if (M == 32) {

    for (i = 0; i < N; i += 8) {
      for (j = 0; j < M; j += 8) {
        for (ii = i; ii < i + 8; ii++) {
          jj = j;
          a0 = A[ii][jj + 0];
          a1 = A[ii][jj + 1];
          a2 = A[ii][jj + 2];
          a3 = A[ii][jj + 3];
          a4 = A[ii][jj + 4];
          a5 = A[ii][jj + 5];
          a6 = A[ii][jj + 6];
          a7 = A[ii][jj + 7];
          B[jj + 0][ii] = a0;
          B[jj + 1][ii] = a1;
          B[jj + 2][ii] = a2;
          B[jj + 3][ii] = a3;
          B[jj + 4][ii] = a4;
          B[jj + 5][ii] = a5;
          B[jj + 6][ii] = a6;
          B[jj + 7][ii] = a7;
        }
      }
    }
  } else if (M == 64) {
    // for (i = 0; i < N; i += 8) {
    //   for (j = 0; j < M; j += 8) {
    //       for(ii = 0; ii < 4; ++ii) {
    //         a0 = A[i + ii][j + 0];
    //         a1 = A[i + ii][j + 1];
    //         a2 = A[i + ii][j + 2];
    //         a3 = A[i + ii][j + 3];
    //         a4 = A[i + ii][j + 4];
    //         a5 = A[i + ii][j + 5];
    //         a6 = A[i + ii][j + 6];
    //         a7 = A[i + ii][j + 7];

    //         B[j + 0][i + ii] = a0;
    //         B[j + 1][i + ii] = a1;
    //         B[j + 2][i + ii] = a2;
    //         B[j + 3][i + ii] = a3;
    //         B[j + 0][i + ii + 4] = a4;
    //         B[j + 1][i + ii + 4] = a5;
    //         B[j + 2][i + ii + 4] = a6;
    //         B[j + 3][i + ii + 4] = a7;
    //       }

    //       for (ii = 4; ii < 8; ++ii) {
    //         a0 = A[i + ii][j + 0];
    //         a1 = A[i + ii][j + 1];
    //         a2 = A[i + ii][j + 2];
    //         a3 = A[i + ii][j + 3];
    //         a4 = A[i + ii][j + 4];
    //         a5 = A[i + ii][j + 5];
    //         a6 = A[i + ii][j + 6];
    //         a7 = A[i + ii][j + 7];

    //         B[j + 4][i + ii - 4] = a0;
    //         B[j + 5][i + ii - 4] = a1;
    //         B[j + 6][i + ii - 4] = a2;
    //         B[j + 7][i + ii - 4] = a3;
    //         B[j + 4][i + ii] = a4;
    //         B[j + 5][i + ii] = a5;
    //         B[j + 6][i + ii] = a6;
    //         B[j + 7][i + ii] = a7;
    //       }

    //       for (ii = 0; ii < 4; ++ii) {
    //         a0 = B[j + 0][i + ii + 4];
    //         a1 = B[j + 1][i + ii + 4];
    //         a2 = B[j + 2][i + ii + 4];
    //         a3 = B[j + 3][i + ii + 4];

    //         B[j + 0][i + ii + 4] = B[j + 4][i + ii];
    //         B[j + 1][i + ii + 4] = B[j + 5][i + ii];
    //         B[j + 2][i + ii + 4] = B[j + 6][i + ii];
    //         B[j + 3][i + ii + 4] = B[j + 7][i + ii];

    //         B[j + 4][i + ii] = a0;
    //         B[j + 5][i + ii] = a1;
    //         B[j + 6][i + ii] = a2;
    //         B[j + 7][i + ii] = a3;
    //       }
    //   }
    // }

    for (i = 0; i < N; i += 8) {
      for (j = 0; j < M; j += 8) {

        for(jj=j;jj<j+4;++jj){
          a0=A[jj][i];a1=A[jj][i+1];a2=A[jj][i+2];a3=A[jj][i+3];
          a4=A[jj][i+4];a5=A[jj][i+5];a6=A[jj][i+6];a7=A[jj][i+7];

          B[i][jj]=a0;B[i][jj+4]=a4;B[i+1][jj]=a1;B[i+1][jj+4]=a5;
          B[i+2][jj]=a2;B[i+2][jj+4]=a6;B[i+3][jj]=a3;B[i+3][jj+4]=a7;                               
        }

        for(jj=i;jj<i+4;++jj){
            a0=B[jj][j+4];a1=B[jj][j+5];a2=B[jj][j+6];a3=B[jj][j+7];
            a4=A[j+4][jj];a5=A[j+5][jj];a6=A[j+6][jj];a7=A[j+7][jj];

            B[jj][j+4]=a4;B[jj][j+5]=a5;B[jj][j+6]=a6;B[jj][j+7]=a7;
            B[jj+4][j]=a0;B[jj+4][j+1]=a1;B[jj+4][j+2]=a2;B[jj+4][j+3]=a3;
        }
        
        for(jj=i+4;jj<i+8;++jj){
            a1=A[j+4][jj];a2=A[j+5][jj];a3=A[j+6][jj];a4=A[j+7][jj];
            B[jj][j+4]=a1;B[jj][j+5]=a2;B[jj][j+6]=a3;B[jj][j+7]=a4;
        }
      }
    }
  } else {
    for (i = 0; i < N; i += 16) {
      for (j = 0; j < M; j += 16) {
        for (ii = i; ii < i + 16 && ii < N; ii++) {
          for (jj = j; jj < j + 16 && jj < M; jj++) {
            a0 = A[ii][jj];
            B[jj][ii] = a0;
          }
        }
      }
    }
  }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

