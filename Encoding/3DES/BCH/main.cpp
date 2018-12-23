#include <iostream>
using namespace std;


void Print(int* a, int len)
{
	for (int i = len - 1; i >= 0; i--)
		cout << a[i];
	cout << endl;
}


void encode(int* g, int glen, int* data, int datalen, int* encoded_data)
{
	for (int i = 0; i < glen; i++)
		if (g[i] != 0)
			for (int j = 0; j < datalen; j++)
				if (data[j] != 0)
					encoded_data[i+j] = (encoded_data[i + j] + 1) % 2;
}


void decode(int* g, int glen, int* encoded_data, int enc_datalen, int* data)
{
	int* temp = new int[enc_datalen];
	memcpy(temp, encoded_data, enc_datalen * 4);

	while (true)
	{
		int maxdeg = -1;

		for (int i = enc_datalen - 1; i >= 0; i--)
		{
			if (encoded_data[i] != 0)
			{
				maxdeg = i;
				break;
			}
		}

		if (maxdeg == -1)
		{
			cout << "No errors" << endl;
			break;
		}
		else if (maxdeg < glen - 1)
		{
			cout << endl << "There is errors" << endl << "Mod: ";
			Print(encoded_data, enc_datalen);

			int ones = 0;
			for (int i = 0; i <= maxdeg; i++)
				if (encoded_data[i] == 1)
					ones++;

			// Исправляем две ошибки
			if (ones <= 2)
			{
				for (int i = 0; i <= maxdeg; i++)
					temp[i] = (temp[i] + encoded_data[i]) % 2;
				cout << "Corrected: ";
				Print(temp, enc_datalen);

				decode(g, glen, temp, enc_datalen, data);
			}
			else
			{
				cout << "More than 2 errors" << endl;
				memset(data, 0, (enc_datalen - glen) * 4);
			}

			break;
		}

		// Деление столбиком
		int deg = maxdeg - (glen - 1);
		data[deg] = 1;
		for (int i = glen - 1; i >= 0; i--)
		{
			encoded_data[i + deg] -= g[i];
			if (encoded_data[i + deg] < 0)
				encoded_data[i + deg] = -encoded_data[i + deg];
		}
	}

	delete[] temp;
}


int main()
{
	int m = 4;
	int n = 15; // 2**m - 1
	int k = 7;
	int d = 5;

	cout << "Code is (" << n << "," << k << "," << d << ")" << endl;


	int* g = new int[n - k + 1];
	memset(g, 0, (n - k + 1) * 4);
	g[0] = g[4] = g[6] = g[7] = g[8] = 1;
	cout << "g(x) = ";
	Print(g, n - k + 1);


	int* data = new int[k];
	memset(data, 0, k * 4);
	data[0] = data[1] = data[5] = data[6] = 1;
	cout << "data(x) = ";
	Print(data, k);


	int* encoded_data = new int[n];
	memset(encoded_data, 0, n * 4);
	encode(g, 9, data, k, encoded_data);
	cout << "Encoded: ";
	Print(encoded_data, n);


	encoded_data[2] = encoded_data[6] = 1;
	cout << "Make errors: ";
	Print(encoded_data, n);


	memset(data, 0, k * 4);
	decode(g, n - k + 1, encoded_data, n, data);
	cout << "Decoded: ";
	Print(data, k);
	cout << endl;


	delete[] data;
	delete[] encoded_data;
	delete[] g;

	system("pause");
	return 0;
}