#include <iostream>
#include <omp.h>

using namespace std;

int main()
{
	const int N = 9;
	int A[N] = { 1, 2, 3, 4, 5, 6, 7, 8 , 9 };


#pragma omp for schedule(dynamic, 3)
	for (int j = 0; j < N; j += 3)
	{
		A[j + 1] += A[j];
		A[j + 2] += A[j + 1];
	}

	for (int j = 2; j < N - 3; j += 3)
		A[j + 3] += A[j];

	for (int i = 0; i < N; i++)
		cout << A[i] << ' ';
	cout << endl;

	system("pause");
	return 0;
}