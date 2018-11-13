#include "Algo.h"


void PAE(HWND hwEdit_A, HWND hwEdit_B, HWND hwEdit_x, HWND hwEdit_y, HWND hwEdit_d)
{
	char str[100] = { 0 };

	GetWindowText(hwEdit_A, str, 100);
	int A = atoi(str);

	GetWindowText(hwEdit_B, str, 100);
	int B = atoi(str);

	int div[100];
	int divlen = 0;

	while (A % B != 0) {
		int mod = A % B;
		div[divlen] = A / B;
		divlen++;

		A = B;
		B = mod;
	}
	div[divlen] = A / B;

	int x = 0, y = 1;
	for (int i = divlen - 1; i >= 0; i--) {
		int x_old = x;
		x = y;
		y = x_old - y * div[i];
		sprintf_s(str, "%d %d %d", x, y, div[i]);
		MessageBox(0, str,0,0);
	}

	_itoa_s(B, str, 10);
	SetWindowText(hwEdit_d, str);

	_itoa_s(x, str, 10);
	SetWindowText(hwEdit_x, str);

	_itoa_s(y, str, 10);
	SetWindowText(hwEdit_y, str);
}