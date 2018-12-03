#include "Algo.h"


void RSA(HWND hwq, HWND hwp, HWND hwtext, HWND hwencoded, HWND hwdecoded)
{
	char text[300];
	GetWindowText(hwtext, text, 300);

	char f[100];
	GetWindowText(hwp, f, 100);
	long long p = atoi(f);

	GetWindowText(hwq, f, 100);
	long long q = atoi(f);

	long long n = p * q;
	long long phi = (p - 1) * (q - 1);
	
	// NOD (n, e) = 1,  2 <= e < phi + is prime
	long long e = 7;
	//for (; e < phi; e += 2)
		//if (IsPrime(e) && NOD(e, n) == 1)
			//break;

	// e * d + phi * y = 1î
	long long d = PAE(e, phi);
	if (d < 0)
		d = phi + d;

	long long encrypted_text[300];
	for (int i = 0; i < 300; i++)
		encrypted_text[i] = 0;

	for (int i = 0; i < strlen(text); i++)
		encrypted_text[i] = PowMod(text[i], e, n);
	
	strcpy_s(f, "");
	for (int i = 0; i < strlen(text); i++)
		sprintf_s(f, "%s%c", f, (char)encrypted_text[i]);
	SetWindowText(hwencoded, f);

	for (int i = 0; i < 14; i++)
		text[i] = PowMod(encrypted_text[i], d, n);

	SetWindowText(hwdecoded, text);
}


bool IsPrime(long long n)
{
	if (n == 2)
		return true;

	if (n % 2 == 0 || n < 2)
		return false;

	long long len = (long long)sqrt(n);
	for (long long i = 3; i <= len; i += 2)
		if (n % i == 0)
			return false;

	return true;
}


long long NOD(long long A, long long B)
{
	while (A % B != 0) {
		long long mod = A % B;
		A = B;
		B = mod;
	}

	return B;
}


long long PAE(long long A, long long B)
{
	long long div[100000];
	int divlen = 0;

	while (A % B != 0) {
		int mod = A % B;
		div[divlen] = A / B;
		divlen++;

		A = B;
		B = mod;
	}
	div[divlen] = A / B;

	long long x = 0, y = 1;
	for (int i = divlen - 1; i >= 0; i--) {
		long long x_old = x;
		x = y;
		y = x_old - y * div[i];
	}

	return x;
}


long long PowMod(long long a, long long pow, long long mod)
{
	bitset<64> b_bin = pow; // up to 2**64
	int len = 63;
	for (; len >= 0; len--)
		if (b_bin[len] == 1)
			break;

	long long result = a;
	for (len = len - 1; len >= 0; len--) {
		if (b_bin[len] == 0)
			result = (result*result) % mod;
		else
			result = (result*result*a) % mod;
	}

	return result;
}