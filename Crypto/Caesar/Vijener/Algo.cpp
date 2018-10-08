#include "Algo.h"


void Encode(HWND hwEditWord, HWND hwEditKey, HWND hwEditOut)
{
	char word[100] = { 0 };
	GetWindowText(hwEditWord, word, 100);

	char key[100] = { 0 };
	GetWindowText(hwEditKey, key, 100);

	for (int i = 0, j = 0; i < strlen(word); i++, j++)
	{
		if (word[i] == ' ')
		{
			j--;
			continue;
		}

		if (j == strlen(key))
			j = 0;

		word[i] += key[j] - 'A' + 1;
		if (word[i] > 'Z')
			word[i] = 'A' + (word[i] - 'Z') - 1;
	}

	SetWindowText(hwEditOut, word);
}


void Decode(HWND hwEditWord, HWND hwEditKey, HWND hwEditOut)
{
	char word[100] = { 0 };
	GetWindowText(hwEditWord, word, 100);

	char key[100] = { 0 };
	GetWindowText(hwEditKey, key, 100);

	for (int i = 0, j = 0; i < strlen(word); i++, j++)
	{
		if (word[i] == ' ')
		{
			j--;
			continue;
		}

		if (j == strlen(key))
			j = 0;

		word[i] -= key[j] - 'A' + 1;
		if (word[i] < 'A')
			word[i] = 'Z' - ('A' - word[i]) + 1;
	}

	SetWindowText(hwEditOut, word);
}