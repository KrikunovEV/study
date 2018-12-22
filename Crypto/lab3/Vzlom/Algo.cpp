#include "Algo.h"


void Vzlom(HWND hwB, HWND hwN, HWND hwP, HWND hwQ, HWND hwE, HWND hwD, HWND hwDecode)
{
	char filler[100];

	GetWindowText(hwB, filler, 100);
	mpz_t B;
	mpz_init_set_si(B, atoi(filler));

	GetWindowText(hwN, filler, 100);
	mpz_t n;
	mpz_init_set_si(n, atoi(filler));// 10001 (1 stage), 163237 (2 stage)
	mpz_init_set_str(n, "344572667627327574872986520507", 10);

	mpz_t M;
	mpz_init_set_si(M, 1);

	mpz_t prime;
	mpz_init_set_si(prime, 2);

	// compute M
	while (mpz_cmp(prime, B) < 0)
	{
		mpz_t temp;
		mpz_init_set(temp, prime);

		while (mpz_cmp(temp, B) < 0)
			mpz_mul(temp, temp, prime);
		mpz_div(temp, temp, prime);

		mpz_mul(M, M, temp);

		mpz_nextprime(prime, prime);
	}

	mpz_t base;
	mpz_init_set_si(base, 2);

	mpz_t b, p;
	mpz_init(b);
	mpz_init(p);

	// find base (handle p == N)
	while (true) {
		mpz_t a;
		mpz_init_set(a, base);

		mpz_powm(b, a, M, n);
		mpz_sub_ui(a, b, 1);

		mpz_gcd(p, n, a);

		if (mpz_cmp(p, n) == 0)
			mpz_add_ui(base, base, 3);
		else
			break;
	}

	bool found = false;
	mpz_t q;
	mpz_init(q);

	if (mpz_cmp_si(p, 1) != 0)
	{
		MessageBox(0, "Нашёл p и q за первую стадию", 0, MB_OK);
		char str[1000];

		mpz_div(q, n, p);

		mpz_get_str(str, 10, p);
		SetWindowText(hwP, str);

		mpz_get_str(str, 10, q);
		SetWindowText(hwQ, str);

		found = true;
	}

	if (!found)
	{
		mpz_t B2;
		mpz_init(B2);
		mpz_mul(B2, B, B);
		vector<mpz_t*> primes;

		// get primes between B B2
		mpz_set(prime, B);
		mpz_nextprime(prime, prime);
		do
		{
			mpz_t* pv = new mpz_t[1];
			mpz_init_set(*pv, prime);
			primes.push_back(pv);
			mpz_nextprime(prime, prime);
		} while (mpz_cmp(prime, B2) < 0);
		
		char str2[10000] = { 0 };

		mpz_t c;
		mpz_init(c);
		for (int i = 0; i < primes.size(); i++)
		{
			mpz_powm(c, b, *primes[i], n);
			mpz_sub_ui(c, c, 1);

			mpz_t d;
			mpz_init(d);
			mpz_gcd(d, n, c);

			//sprintf_s(str2, "%sd = %ld _ ", str2, mpz_get_si(d));

			if (mpz_cmp_si(d, 1) != 0)
			{
				MessageBox(0, "Нашёл за вторую стадию", 0, MB_OK);
				char str[1000];

				mpz_set(p, d);
				mpz_get_str(str, 10, p);
				SetWindowText(hwP, str);

				mpz_div(q, n, p);
				mpz_get_str(str, 10, q);
				SetWindowText(hwQ, str);
				found = true;
				break;
			}
		}

		//MessageBox(0, str2, 0, 0);

		if (!found)
		{
			MessageBox(0, "Не разложил", 0, MB_OK);
			return;
		}
	}

	mpz_t  phi;
	mpz_init(phi);
	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_mul(phi, p, q);

	// NOD (n, e) = 1,  2 <= e < phi
	mpz_t e;
	mpz_init_set_si(e, 7);
	mpz_init_set_str(e, "303498613397661458186613409505", 10);
	/*for (; mpz_cmp(e, phi) < 0; mpz_nextprime(e, e)) {
		mpz_t NOD;
		mpz_init(NOD);
		mpz_gcd(NOD, e, n);
		if (mpz_cmp_si(NOD, 1) == 0)
			break;
	}*/

	// e * d + phi * y = 1
	mpz_t x, y, d;
	mpz_init(x);
	mpz_init(y);
	mpz_init(d);
	mpz_gcdext(x, d, y, e, phi);

	if (mpz_cmp_si(d, 0) < 0)
		mpz_add(d, d, phi);

	char str[1000];

	mpz_get_str(str, 10, e);
	SetWindowText(hwE, str);

	mpz_get_str(str, 10, d);
	SetWindowText(hwD, str);

	mpz_t SW;
	mpz_init_set_str(SW, "42393687358080034300726554172", 10);

	mpz_powm(SW, SW, d, n);

	mpz_get_str(str, 10, SW);

	char decode[100];
	for (int i = 0; i < 100; i++)
		decode[i] = 0;

	//16 61 66 56 49 56 30 66 56 58
	for (int i = 0; i < strlen(str); i += 2)
	{
		char ch = (str[i] - '0') * 10 + (str[i + 1] - '0');
		
		if (ch == 16)
			decode[i / 2] = 'А';
		else if (ch == 61)
			decode[i / 2] = 'н';
		else if (ch == 66)
			decode[i / 2] = 'т';
		else if (ch == 56)
			decode[i / 2] = 'и';
		else if (ch == 49)
			decode[i / 2] = 'б';
		else if (ch == 30)
			decode[i / 2] = 'О';
		else if (ch == 58)
			decode[i / 2] = 'к';
	}
	
	SetWindowText(hwDecode, decode);
}
