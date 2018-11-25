#include <math.h>
#include <stdio.h>
#include <random>
#include <iostream>

using namespace std;

void Get_p(int* &p, int& m, int& n, int& nmax)
{
	//2**(m-1) - 1 < n <= 2**m - 1
	cout << "Enter m: ";
	cin >> m;

	p = new int[m+1];
	for (int i = 1; i < m; i++)
		p[i] = 0;
	p[0] = p[m] = 1;

	if (m == 2)			p[1] = 1;
	else if (m == 3)	p[1] = 1;
	else if (m == 4)	p[1] = 1;
	else if (m == 5)	p[2] = 1;
	else if (m == 6)	p[1] = 1;
	else if (m == 7)	p[1] = 1;
	else if (m == 8)	p[4] = p[5] = p[6] = 1;
	else if (m == 9)	p[4] = 1;
	else if (m == 10)	p[3] = 1;
	else if (m == 11)	p[2] = 1;
	else if (m == 12)	p[3] = p[4] = p[7] = 1;
	else if (m == 13)	p[1] = p[3] = p[4] = 1;
	else if (m == 14)	p[1] = p[11] = p[12] = 1;
	else if (m == 15)	p[1] = 1;
	else if (m == 16)	p[2] = p[3] = p[5] = 1;
	else if (m == 17)	p[3] = 1;
	else if (m == 18)	p[7] = 1;
	else if (m == 19)	p[1] = p[5] = p[6] = 1;
	else if (m == 20)	p[3] = 1;

	cout << "p(x) = ";
	nmax = 1;
	for (int i = 0; i <= m; i++) {
		nmax *= 2;
		cout << p[i];
	}
	cout << endl;

	nmax = nmax / 2 - 1;
	int nmin = (nmax + 1) / 2 - 1;

	cout << "Enter code length n (" << nmin << " < n <= " << nmax << "): ";
	cin >> n;
}


void GenerateGF(int* g, int m, int nmax, int* alpha_to, int* index_of)
{
	// index->polynomial form : alpha_to[] contains j = alpha ^ i;
	// polynomial form->index form : index_of[j = alpha ^ i] = i

	int mask = 1;

	alpha_to[m] = 0;

	for (int i = 0; i < m; i++) {
		alpha_to[i] = mask;
		index_of[alpha_to[i]] = i;
		if (g[i] != 0)
			alpha_to[m] ^= mask;
		mask <<= 1;
	}

	index_of[alpha_to[m]] = m;
	mask >>= 1;

	for (int i = m + 1; i < nmax; i++) {
		if (alpha_to[i - 1] >= mask)
			alpha_to[i] = alpha_to[m] ^ ((alpha_to[i - 1] ^ mask) << 1);
		else
			alpha_to[i] = alpha_to[i - 1] << 1;
		index_of[alpha_to[i]] = i;
	}

	index_of[0] = -1;
}


void gen_poly(int n, int m, int nmax, int &t, int &k, int &d, int* g, int* alpha_to, int* index_of)
{
	int	ii, jj, ll, kaux;
	int	test, aux, nocycles, root, noterms, rdncy;
	int cycle[1024][21], size[1024], min[1024], zeros[1024];

	cycle[0][0] = 0;
	size[0] = 1;
	cycle[1][0] = 1;
	size[1] = 1;
	jj = 1;


	// build cycles classes
	do {
		ii = 0;
		do {
			ii++;
			cycle[jj][ii] = (cycle[jj][ii - 1] * 2) % nmax;
			size[jj]++;
			aux = (cycle[jj][ii] * 2) % nmax;
		} while (aux != cycle[jj][0]);
		
		ll = 0;
		do {
			ll++;
			test = 0;
			for (ii = 1; ((ii <= jj) && (!test)); ii++)
				for (kaux = 0; ((kaux < size[ii]) && (!test)); kaux++)
					if (ll == cycle[ii][kaux])
						test = 1;
		} while ((test) && (ll < (nmax - 1)));

		if (!(test)) {
			jj++;
			cycle[jj][0] = ll;
			size[jj] = 1;
		}

	} while (ll < (nmax - 1));

	nocycles = jj;

	cout << "Enter the error correcting capability t: ";
	cin >> t;

	d = 2 * t + 1;

	/* Search for roots 1, 2, ..., d-1 in cycle sets */
	kaux = 0;
	rdncy = 0;
	for (ii = 1; ii <= nocycles; ii++) {
		min[kaux] = 0;
		test = 0;
		for (jj = 0; ((jj < size[ii]) && (!test)); jj++)
			for (root = 1; ((root < d) && (!test)); root++)
				if (root == cycle[ii][jj]) {
					test = 1;
					min[kaux] = ii;
				}
		if (min[kaux]) {
			rdncy += size[min[kaux]];
			kaux++;
		}
	}

	noterms = kaux;
	kaux = 1;
	for (ii = 0; ii < noterms; ii++)
		for (jj = 0; jj < size[min[ii]]; jj++) {
			zeros[kaux] = cycle[min[ii]][jj];
			kaux++;
		}

	k = n - rdncy;

	if (k < 0)
	{
		cout << "something is wrong!" << endl;
		system("pause");
	}

	cout << "Code is (" << n << "," << k << "," << d << ")" << endl;

	g[0] = alpha_to[zeros[1]];
	g[1] = 1;
	for (ii = 2; ii <= rdncy; ii++) {
		g[ii] = 1;
		for (jj = ii - 1; jj > 0; jj--)
			if (g[jj] != 0)
				g[jj] = g[jj - 1] ^ alpha_to[(index_of[g[jj]] + zeros[ii]) % nmax];
			else
				g[jj] = g[jj - 1];
		g[0] = alpha_to[(index_of[g[0]] + zeros[ii]) % nmax];
	}

	cout << "g(x) = ";
	for (ii = 0; ii <= rdncy; ii++) {
		cout << g[ii];
		if (ii && ((ii % 50) == 0))
			cout << endl;
	}
	cout << endl;
}


void encode(int n, int k, int* g, int* data, int* bb)
{
	for (int i = 0; i < n - k; i++)
		bb[i] = 0;

	for (int i = k - 1; i >= 0; i--) {
		int feedback = data[i] ^ bb[n - k - 1];
		if (feedback != 0) {
			for (int j = n - k - 1; j > 0; j--)
				if (g[j] != 0)
					bb[j] = bb[j - 1] ^ feedback;
				else
					bb[j] = bb[j - 1];
			bb[0] = g[0] && feedback;
		}
		else {
			for (int j = n - k - 1; j > 0; j--)
				bb[j] = bb[j - 1];
			bb[0] = 0;
		}
	}
}


void decode_bch(int n, int t, int* alpha_to, int* index_of, int nmax, int* recd)
{
	int u, q, t2, count = 0, syn_error = 0;
	int elp[1026][1024], d[1026], l[1026], u_lu[1026], s[1025];
	int root[200], loc[200], err[1024], reg[201];

	t2 = 2 * t;

	cout << "s(x) = ";
	for (int i = 1; i <= t2; i++) {
		s[i] = 0;

		for (int j = 0; j < n; j++)
			if (recd[j] != 0)
				s[i] ^= alpha_to[(i * j) % n];

		if (s[i] != 0)
			syn_error = 1;

		s[i] = index_of[s[i]];
		cout << s[i];
	}
	cout << endl;

	if (syn_error) {
		d[0] = 0;
		d[1] = s[1];
		elp[0][0] = 0;
		elp[1][0] = 1;
		for (int i = 1; i < t2; i++) {
			elp[0][i] = -1;
			elp[1][i] = 0;
		}
		l[0] = 0;
		l[1] = 0;
		u_lu[0] = -1;
		u_lu[1] = 0;
		u = 0;

		do {
			u++;
			if (d[u] == -1) {
				l[u + 1] = l[u];
				for (int i = 0; i <= l[u]; i++) {
					elp[u + 1][i] = elp[u][i];
					elp[u][i] = index_of[elp[u][i]];
				}
			}
			else
			{
				q = u - 1;
				while ((d[q] == -1) && (q > 0))
					q--;

				if (q > 0) {
					int j = q;
					do {
						j--;
						if ((d[j] != -1) && (u_lu[q] < u_lu[j]))
							q = j;
					} while (j > 0);
				}

				if (l[u] > l[q] + u - q)
					l[u + 1] = l[u];
				else
					l[u + 1] = l[q] + u - q;

				for (int i = 0; i < t2; i++)
					elp[u + 1][i] = 0;

				for (int i = 0; i <= l[q]; i++)
					if (elp[q][i] != -1)
						elp[u + 1][i + u - q] =
						alpha_to[(d[u] + n - d[q] + elp[q][i]) % n];

				for (int i = 0; i <= l[u]; i++) {
					elp[u + 1][i] ^= elp[u][i];
					elp[u][i] = index_of[elp[u][i]];
				}
			}
			u_lu[u + 1] = u - l[u + 1];

			if (u < t2) {
				if (s[u + 1] != -1)
					d[u + 1] = alpha_to[s[u + 1]];
				else
					d[u + 1] = 0;
				for (int i = 1; i <= l[u + 1]; i++)
					if ((s[u + 1 - i] != -1) && (elp[u + 1][i] != 0))
						d[u + 1] ^= alpha_to[(s[u + 1 - i]
							+ index_of[elp[u + 1][i]]) % n];
				d[u + 1] = index_of[d[u + 1]];
			}
		} while ((u < t2) && (l[u + 1] <= t));

		u++;
		if (l[u] <= t) {
			for (int i = 0; i <= l[u]; i++)
				elp[u][i] = index_of[elp[u][i]];

			cout << "sigma(x) = ";
			for (int i = 0; i <= l[u]; i++)
				cout << elp[u][i];
			cout << endl;

			cout << "roots: ";
			for (int i = 1; i <= l[u]; i++)
				reg[i] = elp[u][i];
			count = 0;
			for (int i = 1; i <= nmax; i++) {
				q = 1;
				for (int j = 1; j <= l[u]; j++)
					if (reg[j] != -1) {
						reg[j] = (reg[j] + j) % nmax;
						q ^= alpha_to[reg[j]];
					}
				if (!q) {
					root[count] = i;
					loc[count] = nmax - i;
					count++;
					cout << nmax - i;
				}
			}
			cout << endl;

			if (count == l[u])
				for (int i = 0; i < l[u]; i++)
					recd[loc[i]] ^= 1;
			else
				printf("Incomplete decoding: errors detected\n");
		}
	}
}



int main()
{
	int* p = 0;
	int m;
	int n;
	int nmax;

	Get_p(p, m, n, nmax);

	int alpha_to[10000];
	int index_of[10000];

	GenerateGF(p, m, nmax, alpha_to, index_of);

	int g[10000];
	int t, k, d;

	gen_poly(n, m, nmax, t, k, d, g, alpha_to, index_of);             /* Compute the generator polynomial of BCH code */


	int bb[10000];
	int data[10000];
	for (int i = 0; i < k; i++)
		data[i] = (rand() & 65536) >> 16;

	encode(n, k, g, data, bb);

	// recd[] are the coefficients of c(x) = x**(length-k)*data(x) + b(x)
	int recd[10000];

	for (int i = 0; i < n - k; i++)
		recd[i] = bb[i];

	for (int i = 0; i < k; i++)
		recd[i + n - k] = data[i];

	cout << "Code polynomial:\nc(x) = ";
	for (int i = 0; i < n; i++) {
		cout << recd[i];
		if (i && ((i % 50) == 0))
			cout << endl;
	}
	cout << endl;


	int numerr, errpos[1024], decerror = 0;

	cout << "Enter the number of errors:" << endl;
	cin >> numerr;
	cout << "Enter error indecies (from 0 to " << n - 1 << "): ";
	/*
	 * recd[] are the coefficients of r(x) = c(x) + e(x)
	 */
	for (int i = 0; i < numerr; i++)
		cin >> errpos[i];
	if (numerr)
		for (int i = 0; i < numerr; i++)
			recd[errpos[i]] ^= 1;
	cout << "r(x) = ";
	for (int i = 0; i < n; i++) {
		cout << recd[i];
		if (i && ((i % 50) == 0))
			cout << endl;
	}
	cout << endl;

	decode_bch(n, t, alpha_to, index_of, nmax, recd);

	/*
	 * print out original and decoded data
	 */
	cout << "original data  = ";
	for (int i = 0; i < k; i++) {
		cout << data[i];
		if (i && ((i % 50) == 0))
			cout << endl;
	}
	cout << endl;

	cout << "decoded data = ";
	for (int i = n - k; i < n; i++) {
		cout << recd[i];
		if ((i - n + k) && (((i - n + k) % 50) == 0))
			cout << endl;
	}
	cout << endl;

	/*
	 * DECODING ERRORS? we compare only the data portion
	 
	for (i = length - k; i < length; i++)
		if (data[i - length + k] != recd[i])
			decerror++;
	if (decerror)
		printf("There were %d decoding errors in message positions\n", decerror);
	else
		printf("Succesful decoding\n");
		*/
	system("pause");
	return 0;
}
