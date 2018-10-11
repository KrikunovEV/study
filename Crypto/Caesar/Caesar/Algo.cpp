#include "Algo.h"


void Encode(HWND hwEditWord, HWND hwEditStep, HWND hwEditOut)
{
	char word[100] = { 0 };
	GetWindowText(hwEditWord, word, 100);

	char chStep[100] = { 0 };
	GetWindowText(hwEditStep, chStep, 100);
	int step = chStep[0] - '0';

	for (int i = 0; i < strlen(word); i++) {

		if (word[i] == ' ')
			continue;

		if (word[i] + step > 'z')
			word[i] = 'a' + step - ('z' - word[i]) - 1;
		else
			word[i] += step;
	}

	SetWindowText(hwEditOut, word);
}


void Decode(HWND hwEditWord, HWND hwEditStep, HWND hwEditOut)
{
	char word[100] = { 0 };
	GetWindowText(hwEditWord, word, 100);

	char chStep[100] = { 0 };
	GetWindowText(hwEditStep, chStep, 100);
	int step = chStep[0] - '0';

	for (int i = 0; i < strlen(word); i++) {

		if (word[i] == ' ')
			continue;
		if (word[i] - step < 'a')
			word[i] = 'z' - (step - (word[i] - 'a') - 1);
		else
			word[i] -= step;
	}

	SetWindowText(hwEditOut, word);
}