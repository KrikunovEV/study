#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex m;
int sum = 0;

void hello(int* A, int i, int j)
{
	int my_sum = 0;
	for (; i < j; i++)
		my_sum += A[i];

	lock_guard<mutex> lg(m);
	sum += my_sum;
}


int main()
{
	const int N = 11;
	const int M = N / 2;
	const int Q = N / M;

	int A[N] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	thread t[M];

	for (int i = 0; i < M; i++) {

		int start = i * Q;
		int end = (i + 1) * Q;

		if (end > N)
			end = N;

		t[i] = thread(hello, A, start, end);
	}


	for (int i = 0; i < M; i++) {
		t[i].join();
	}

	cout << "sum = " << sum << endl;
	system("pause");
	return 0;
}