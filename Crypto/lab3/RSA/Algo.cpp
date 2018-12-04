#include "Algo.h"

void RSA(HWND hwq, HWND hwp, HWND hwtext, HWND hwencoded, HWND hwdecoded)
{
	char debug[100000];
	for (int i = 0; i < 100000; i++)
		debug[i] = 0;

	gmp_randstate_t rstate;
	gmp_randinit_default(rstate);
	
	char text[300];
	GetWindowText(hwtext, text, 300);

	char f[100];
	GetWindowText(hwp, f, 100);
	int pbit = atoi(f);
	mpz_t p;
	mpz_init(p);
	mpz_urandomb(p, rstate, pbit);
	mpz_nextprime(p, p);
	sprintf_s(debug, "%s%ld\n", debug, mpz_get_si(p));

	GetWindowText(hwq, f, 100);
	int qbit = atoi(f);
	mpz_t q;
	mpz_init(q);
	mpz_urandomb(q, rstate, qbit);
	mpz_nextprime(q, q);
	sprintf_s(debug, "%s%ld\n", debug, mpz_get_si(q));

	mpz_t n, phi;
	mpz_init(n);
	mpz_init(phi);
	mpz_mul(n, p, q);
	sprintf_s(debug, "%s%ld\n", debug, mpz_get_si(n));

	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_mul(phi, p, q);
	sprintf_s(debug, "%s%ld\n", debug, mpz_get_si(phi));
	
	// NOD (n, e) = 1,  2 <= e < phi + is prime
	mpz_t e;
	mpz_init(e);
	mpz_set_ui(e, 7);
	for (; mpz_cmp(e, phi) < 0; mpz_nextprime(e, e)) {
		mpz_t NOD;
		mpz_init(NOD);
		mpz_gcd(NOD, e, n);
		if (mpz_cmp_si(NOD, 1) == 0)
			break;
	}
	sprintf_s(debug, "%s%ld\n", debug, mpz_get_si(e));

	// e * d + phi * y = 1
	mpz_t x, y, d;
	mpz_init(x);
	mpz_init(y);
	mpz_init(d);
	mpz_gcdext(x, d, y, e, phi);

	if (mpz_cmp_si(d, 0) < 0)
		mpz_add(d, d, phi);
	sprintf_s(debug, "%sd = %ld\n", debug, mpz_get_si(d));
	sprintf_s(debug, "%sx = %ld\n", debug, mpz_get_si(x));
	sprintf_s(debug, "%sy = %ld\n", debug, mpz_get_si(y));

	MessageBox(0, debug, 0, 0);

	mpz_t* encrypted = new mpz_t[strlen(text)];
	char* encrypted_text = new char[strlen(text)];
	for (int i = 0; i < strlen(text); i++) {
		encrypted_text[i] = 0;
		mpz_init(encrypted[i]);
	}

	for (int i = 0; i < strlen(text); i++) {
		mpz_t value;
		mpz_init(value);
		mpz_set_ui(value, (int)text[i]);
		mpz_powm(encrypted[i], value, e, n);
		encrypted_text[i] = mpz_get_si(encrypted[i]);
		
	}
	
	strcpy_s(f, "");
	for (int i = 0; i < strlen(text); i++)
		sprintf_s(f, "%s%c", f, (char)encrypted_text[i]);
	SetWindowText(hwencoded, f);

	for (int i = 0; i < strlen(text); i++) {
		mpz_t value;
		mpz_init(value);
		mpz_powm(value, encrypted[i], d, n);
		text[i] = mpz_get_si(value);
		if(!text[i])
			MessageBox(0, text, 0, 0);
	}

	SetWindowText(hwdecoded, text);

	delete[] encrypted;
	delete[] encrypted_text;
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