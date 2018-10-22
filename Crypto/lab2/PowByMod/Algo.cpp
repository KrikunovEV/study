#include "Algo.h"


void Compute(HWND a_edit, HWND b_edit, HWND mod_edit, HWND out_edit)
{
	char a_char[10] = { 0 };
	GetWindowText(a_edit, a_char, 10);
	int a = atoi(a_char);

	char b_char[10] = { 0 };
	GetWindowText(b_edit, b_char, 10);
	int b = atoi(b_char);
	bitset<10> b_bin = b; // up to 1024
	int len = 9;
	for (; len >= 0; len--)
		if (b_bin[len] == 1)
			break;

	char mod_char[10] = { 0 };
	GetWindowText(mod_edit, mod_char, 10);
	int mod = atoi(mod_char);

	int result = a;
	for (len = len-1; len >= 0; len--) {
		if (b_bin[len] == 0) {
			result = (result*result) % mod;
			char out[10] = { 0 };
			sprintf_s(out, "%d", result);
			MessageBox(0, out, "0", 0);
		}
		else {
			result = (result*result*a) % mod;
			char out[10] = { 0 };
			sprintf_s(out, "%d", result);
			MessageBox(0, out, "1", 0);
		}
	}


	char out[10] = { 0 };
	sprintf_s(out, "%d", result);
	SetWindowText(out_edit, out);
}