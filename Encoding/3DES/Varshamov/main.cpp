#include <iostream>
#include <math.h>
#include <bitset>
#include <vector>
using namespace std;

const int n = 4;

template <int T>
int weight(bitset<T> code, int l)
{
	int sum = 0;
	for (int i = 0; i < code.size(); i++)
		sum += (i + 1) * code[i];
	return sum;
}


int main()
{
	int l = n + 1;
	int num_elements = pow(2, n);

	vector<bitset<n>> codes;

	cout << "Codes:" << endl;
	for (int i = 0; i < num_elements; i++)
	{
		bitset<n> code = i;
		int w = weight(code, l);
		if (w % l == 0)
		{
			cout << code.to_string() << endl;
			codes.push_back(code);
		}
	}


	bitset<n + 1> word("01100");
	cout << endl << "Get word: " << word.to_string() << endl;

	bitset<n> correct;

	if (word.size() == n - 1)
	{
		int w = weight(word, n - 1) % l;
		int ones = word.count();

		cout << "w = " << w << "; ones = " << ones << endl;

		if (w <= ones)
		{
			int i = word.size() - 1, num = 0;
			for (; i >= 0; i--)
			{
				correct[i + 1] = word[i];

				if (word[i] == 1)
					num++;

				if (num == w)
				{
					correct[i] = 0;
					i--;
					break;
				}
			}

			for (; i >= 0; i--)
				correct[i] = word[i];
		}
		else
		{
			int i = word.size() - 1, num = 0;
			for (; i >= 0; i--)
			{
				correct[i + 1] = word[i];

				if (word[i] == 0)
					num++;

				if (num == l - w)
				{
					correct[i] = 1;
					i--;
					break;
				}
			}

			for (; i >= 0; i--)
				correct[i] = word[i];
		}
	}
	else if (word.size() == n + 1)
	{
		int w = weight(word, n + 1) % l;
		int ones = word.count();

		if (w == 0)
		{
			for (int i = 0; i < n; i++)
				correct[i] = word[i];
		}
		else if (w == ones)
		{
			for (int i = 0; i < n; i++)
				correct[i] = word[i + 1];
		}
		else if (w < ones) // убираем 0, справа w единиц
		{
			bool zero = false;
			int i = word.size() - 1, num = 0;
			for (; i >= 0; i--)
			{
				if (word[i] == 1)
					num++;
				else if (zero) {
					i--;
					break;
				}

				correct[i - 1] = word[i];

				if (num == w)
					zero = true;
			}

			for (; i >= 0; i--)
				correct[i] = word[i];
		}
		else if (w > ones) // убираем 1, справа l-w нулей
		{
			bool one = false;
			int i = word.size() - 1, num = 0;
			for (; i >= 0; i--)
			{
				if (word[i] == 0)
					num++;
				else if (one) {
					i--;
					break;
				}

				correct[i - 1] = word[i];

				if (num == l - w)
					one = true;
			}

			for (; i >= 0; i--)
				correct[i] = word[i];
		}
	}

	int ind = 0;
	for (int i = 0; i < n; i++)
		if (correct[i] == 1)
			ind += n - i;
	ind %= l;
	if (ind != 0)
		correct[n - ind] = 1 - correct[n - ind];

	int maxcmp = 0;
	ind = 0;
	for (int i = 0; i < codes.size(); i++)
	{
		int cmp = 0;
		for (int j = 0; j < n; j++)
			if (correct[j] == codes[i][j])
				cmp++;
		if (cmp > maxcmp)
		{
			ind = i;
			maxcmp = cmp;
		}
	}

	cout << "Corrected: " << codes[ind] << endl;


	system("pause");
	return 0;
}