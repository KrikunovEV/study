#include <iostream>
#include <random>
#include <time.h>
#include <math.h>
#include <omp.h>

using namespace std;

int main()
{
	// SUM
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