#include <iostream>
#include <omp.h>

using namespace std;

int main()
{
	const int N = 10;
	const int T = 3;
	int A[N];

	for (int i = 1; i <= N; i++)
		A[i-1] = i;
	
	// 1 iter
#pragma omp for schedule(dynamic, T)
	for (int j = 0; j < N; j += T)
	{
		for (int i = 0; i < T - 1; i++) {
			if (j + i + 1 < N)
				A[j + i + 1] += A[j + i];
			else 
				break;
		}
	}

	// 2 iter
	for (int j = T - 1; j < N - 1; j += T)
	{
		if (j + T < N)
			A[j + T] += A[j];
		else
			A[N - 1] += A[j];
	}

	// 3 iter
#pragma omp for schedule(dynamic, T)
	for (int j = T - 1; j < N - 1; j += T)
	{
		for (int i = 0; i < T - 1; i++) {
			if (j + i + 1 < N - 1)
				A[j + i + 1] += A[j];
			else
				break;
		}
	}

	for (int i = 0; i < N; i++)
		cout << A[i] << ' ';
	cout << endl;

	system("pause");
	return 0;
}