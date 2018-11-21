#include "Algo.h"


void MillerRabin(HWND n, HWND r, HWND out)
{
	char str[100] = { 0 };

	GetWindowText(n, str, 100);
	int N = atoi(str);

	if (N == 2 || N == 3) {
		SetWindowText(out, "Число простое");
		return;
	}
	else if (N < 2 || N % 2 == 0) {
		SetWindowText(out, "Число составное");
		return;
	}

	GetWindowText(r, str, 100);
	int R = atoi(str);

	int* a = new int[R - 1];
	for (int i = 2; i < R + 1; i++)
		a[i - 2] = i;

	for (int i = 0; i < R - 1; i++) {

		int t = N - 1;
		int s = 0;

		while (t % 2 == 0)
		{
			t /= 2;
			s += 1;
		}

		int b = pow(a[i], int(t), N);

		if (b == 1)
			continue;
		else
			for (int j = 1; j < s; j++) {

				b = pow(b, 2, N);

				if (b == 1) {
					SetWindowText(out, "Число составное");
					return;
				}

				if (b == N - 1)
					break;
			}

	}
	delete[] a;

	SetWindowText(out, "Число вероятно простое");
}


int pow(int a, int b, int mod)
{
	bitset<13> b_bin = b; // up to 8192
	int len = 9;
	for (; len >= 0; len--)
		if (b_bin[len] == 1)
			break;


	int result = a;
	for (len = len - 1; len >= 0; len--) {
		if (b_bin[len] == 0)
			result = (result*result) % mod;
		else
			result = (result*result*a) % mod;
	}


	return result;
}