#include <iostream>
#include <random>
#include <time.h>
#include <math.h>
#include <omp.h>

using namespace std;

int main()
{
	// TRANSPOSE flatten matrix

	const int N = 5;
	const int M = 3;

	int* matrix = new int[N*M];
	for (int i = 0; i < N*M; i++)
		matrix[i] = i;

	for (int i = 0; i < N; i++) // row
		for (int j = i; j < M; j++) // column
		{
			int temp = matrix[i*N + j];
			matrix[i*N + j] = matrix[j*N + i];
			matrix[j*N + i] = temp;
		}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++)
			cout << matrix[i*N + j] << ' ';
		cout << endl;
	}


	/*
	// TRANSPOSE 2D matrix
	const int N = 5;
	int matrix[N][N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			matrix[i][j] = i * N + j;


#pragma omp parallel for
	for (int i = 0; i < N; i++)
#pragma omp parallel for
		for (int j = i; j < N; j++) {
			int temp = matrix[i][j];
			matrix[i][j] = matrix[j][i];
			matrix[j][i] = temp;
		}



	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << matrix[i][j] << ' ';
		cout << endl;
	}
	*/ 


	// SUM
	/*
	int N = 15;
	int* a = new int[N];
	for (int i = 0; i < N; i++)
		a[i] = i + 1;

	for (int i = 1; i <= N; i *= 2)
	{

		for (int j = 0; j + i < N; j += 2*i) {
			a[j] += a[j + i];
		}

		for (int j = 0; j < N; j++)
			cout << a[j] << ' ';
		cout << endl;
	}
	*/

	// MAX
	/*
	int N = 15;
	int* a = new int[N];
	for (int i = 0; i < N; i++)
		a[i] = i + 1;

	for (int i = 1; i <= N; i *= 2)
	{

		for (int j = 0; j + i < N; j += 2 * i) {

			if (a[j] < a[j + i])
				a[j] = a[j + i];
		}

		for (int j = 0; j < N; j++)
			cout << a[j] << ' ';
		cout << endl;
	}
	*/
	system("pause");
	return 0;
}