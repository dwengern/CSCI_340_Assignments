// C program for Knight Tour problem
// Remember it might not generate any output since it is taking too long for
// larger values of N
#include <stdio.h>
#define N 7

int solveKTUtil(int x, int y, int movei, int sol[N][N], int xMove[], int yMove[]);

/* A utility function to check if i,j are valid indexes
for N*N chessboard */
int isSafe(int x, int y, int sol[N][N]) {
	if (x >= 0 && x < N && y >= 0 && y < N && sol[x][y] == -1) {
		return 1;
	}
	return 0;
}

/* A utility function to print solution matrix sol[N][N] */
void printSolution(int sol[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf(" %2d ", sol[i][j]);
		}
		printf("\n");
	}
}

/* This function solves the Knight Tour problem using
Backtracking. This function mainly uses solveKTUtil()
to solve the problem. It prints "Solutions doesn't exist" if
no complete tour is possible, otherwise prints the
tour. */
void solveKT() {
	int sol[N][N];

	for (int x = 0; x < N; x++) {
		for (int y = 0; y < N; y++) {
			sol[x][y] = -1;
		}
	}

	int xMove[8] = {2, 1, -1, -2, -2, -1, 1, 2};
	int yMove[8] = {1, 2, 2, 1, -1, -2, -2, -1};

	sol[0][0] = 0;

	if (solveKTUtil(0, 0, 1, sol, xMove, yMove) == 0) {
		printf("Solution does not exist\n");
		return;
	}
	printSolution(sol);
	return;
}

/* A recursive utility function to solve Knight Tour problem */
int solveKTUtil(int x, int y, int movei, int sol[N][N], int xMove[N], int yMove[N]) {
  int xNext, yNext, k;

	if (movei == (N * N))
		return 1;

	for (int i = 0; i < 8; i++) {
		xNext = x + xMove[i];
		yNext = y + yMove[i];

		if (isSafe(xNext, yNext, sol) == 1) {
			sol[xNext][yNext] = movei;
			if (solveKTUtil(xNext, yNext, movei + 1, sol, xMove, yMove) == 1) {
				return 1;
			} else {
				sol[xNext][yNext] = -1;
			}
		}
	}
	return 0;
}

/* Driver program to test above functions */
int main() {
	solveKT();
	return 0;
}
