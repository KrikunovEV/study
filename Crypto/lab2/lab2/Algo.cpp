#include "Algo.h"


void Encode(HWND hwEditWord, HWND hwEditStep, HWND hwEditOut)
{
	char word[100] = { 0 };
	GetWindowText(hwEditWord, word, 100);

	char gamma[100] = { 0 };
	GetWindowText(hwEditStep, gamma, 100);

	for (int i = 0, j = 0; i < strlen(word); i++, j++) {

		if (word[i] == ' ') {
			j--;
			continue;
		}

		if (j == strlen(gamma))
			j = 0;

		bitset<8> word_bin = word[i] - 'a';
		bitset<8> gamma_bin = gamma[j] - 'a';

		word_bin ^= gamma_bin;
		word[i] = word_bin.to_ulong() + 'a';

		//SetWindowText(hwEditOut, word_bin.to_string<char, char_traits<char>, allocator<char>>().c_str());
	}

	SetWindowText(hwEditOut, word);
}


void Decode(HWND hwEditWord, HWND hwEditStep, HWND hwEditOut)
{
	char word[100] = { 0 };
	GetWindowText(hwEditWord, word, 100);

	char gamma[100] = { 0 };
	GetWindowText(hwEditStep, gamma, 100);

	for (int i = 0, j = 0; i < strlen(word); i++, j++) {

		if (word[i] == ' ') {
			j--;
			continue;
		}

		if (j == strlen(gamma))
			j = 0;

		bitset<8> word_bin = word[i] - 'a';
		bitset<8> gamma_bin = gamma[j] - 'a';

		word_bin ^= gamma_bin;
		word[i] = word_bin.to_ulong() + 'a';

		//SetWindowText(hwEditOut, word_bin.to_string<char, char_traits<char>, allocator<char>>().c_str());
	}

	SetWindowText(hwEditOut, word);
}