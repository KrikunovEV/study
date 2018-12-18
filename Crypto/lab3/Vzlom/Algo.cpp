#include "Algo.h"


void Vzlom(HWND hwB, HWND hwN, HWND hwP, HWND hwQ, HWND hwE, HWND hwD)
{
	char filler[100];

	GetWindowText(hwB, filler, 100);
	mpz_t B;
	mpz_init_set_si(B, atoi(filler)); // женя пусечка

	GetWindowText(hwN, filler, 100);
	mpz_t n;
	mpz_init_set_si(n, atoi(filler));// 10001 (1 stadia), (3571, 141257) (not found)

	mpz_t M;
	mpz_init_set_si(M, 1);

	mpz_t prime;
	mpz_init_set_si(prime, 2);

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
	mpz_init_set_si(base, 15);

	mpz_t b, p;
	mpz_init(b);
	mpz_init(p);

	while (true) {
		mpz_t a;
		mpz_init_set(a, base);

		mpz_powm(b, a, M, n);
		mpz_sub_ui(a, b, 1);

		mpz_gcd(p, n, a);

		if (mpz_cmp(p, n) == 0)
			mpz_add_ui(base, base, 1);
		else
			break;
	}

	bool found = false;
	mpz_t q;
	mpz_init(q);

	if (mpz_cmp_si(p, 1) != 0)
	{
		mpz_div(q, n, p);

		char str[300];
		sprintf_s(str, "Нашёл p и q за первую стадию");
		MessageBox(0, str, 0, MB_OK);

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

		mpz_set(prime, B);
		mpz_nextprime(prime, prime);
		do
		{
			mpz_t* pv = new mpz_t[1];
			mpz_init_set(*pv, prime);
			primes.push_back(pv);
			mpz_nextprime(prime, prime);
		} while (mpz_cmp(prime, B2) < 0);

		// deltas
		for (int i = 1; i < primes.size(); i++)
			mpz_sub(*primes[i], *primes[i], *primes[i - 1]);

		char str2[1000000];
		for (int i = 0; i < 1000000; i++)
			str2[i] = 0;
		mpz_t c;
		mpz_init_set_si(c, 1);
		for (int i = 0; i < primes.size(); i++)
		{
			mpz_t powm;
			mpz_init(powm);
			mpz_powm(powm, b, *primes[i], n);
			mpz_mul(c, c, powm);

			mpz_t sub;
			mpz_init(sub);
			mpz_sub_ui(sub, sub, 1);

			mpz_t d;
			mpz_init(d);
			mpz_gcd(d, n, sub);

			//sprintf_s(str2, "%sb = %ld, delta_pow = %ld, c = %ld ~ d = %ld\n", str2, mpz_get_si(b), mpz_get_si(*primes[i]), mpz_get_si(c), mpz_get_si(d));

			if (mpz_cmp_si(d, 1) != 0)
			{
				char str[300];
				//sprintf_s(str, "Нашёл за вторую стадию:\np = %ld\nq = %ld", mpz_get_si(p), mpz_get_si(d));
				MessageBox(0, "Нашёл за вторую стадию", 0, MB_OK);
				mpz_get_str(str, 10, p);
				SetWindowText(hwP, str);

				mpz_set(q, d);
				mpz_get_str(str, 10, q);
				SetWindowText(hwQ, str);
				found = true;
				break;
			}
		}

		if (!found)
		{
			sprintf_s(str2, "%sНе смог разложить", str2);
			//MessageBox(0, str2, 0, MB_OK);
			MessageBox(0, "Не разложил", 0, MB_OK);
			return;
		}
	}
}
