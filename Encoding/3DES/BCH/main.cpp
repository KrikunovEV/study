#include <iostream>
#include <bitset>
// ��������� ����� 6.4
using namespace std;


void encode(int* g, int gk, int* data, int k, int* encoded_data)
{
	for (int i = 0; i < gk; i++)
		if (g[i] != 0)
			for (int j = 0; j < k; j++)
				if (data[j] != 0)
					encoded_data[i+j] += 1;

	for (int i = 0; i < gk + k; i++)
		encoded_data[i] %= 2;
}


void decode(int* g, int gk, int* encoded_data, int datak, int k, int* data)
{
	for (int i = datak - 1; i >= gk - 1; i--)
		for (int j = gk - 1; j >= 0; j--)
			if (g[j] != 0)
					data[i - j] += 1;

	for (int i = 0; i < k; i++)
		data[i] %= 2;
}


int main()
{
	int m = 4;
	int n = 15; // 2**m - 1
	int k = 7;
	int d = 5;

	cout << "Code is (" << n << "," << k << "," << d << ")" << endl;

	int* g = new int[9];
	for (int i = 8; i >= 0; i--)
		g[i] = 0;
	g[0] = g[4] = g[6] = g[7] = g[8] = 1;
	cout << "g(x) = ";
	for (int i = 8; i >= 0; i--)
		cout << g[i];
	cout << endl;

	int* data = new int[k];
	for (int i = 0; i < k; i++)
		data[i] = 0;
	data[0] = data[1] = data[5] = data[6] = 1;
	cout << "data(x) = ";
	for (int i = k - 1; i >= 0; i--)
		cout << data[i];
	cout << endl;


	int len = 9 + k - 1;
	int* encoded_data = new int[k];
	for (int i = 0; i < k; i++)
		data[i] = 0;
	encode(g, 9, data, k, encoded_data);
	for (int i = len - 1; i >= 0; i--)
		cout << encoded_data[i];
	cout << endl;


	for (int i = 0; i < k; i++)
		data[i] = 0;
	decode(g, 9, encoded_data, len, k, data);
	for (int i = k - 1; i >= 0; i--)
		//cout << data[i];
	cout << endl;


	//delete[] data;
	//delete[] g;

	system("pause");
	return 0;
}