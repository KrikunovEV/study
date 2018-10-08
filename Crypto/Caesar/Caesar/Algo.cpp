#include "Algo.h"


void Encode(char* word, int step, HWND hwEdit)
{
	for (int i = 0; i < strlen(word); i++) {

		if (word[i] == ' ')
			continue;
		if (word[i] + step > 'z')
			word[i] = 'a' + step - ('z' - word[i]) - 1;
		else
			word[i] += step;
	}

	SetWindowText(hwEdit, word);
}


void Decode(char* word, int step, HWND hwEdit)
{
	for (int i = 0; i < strlen(word); i++) {

		if (word[i] == ' ')
			continue;
		if (word[i] - step < 'a')
			word[i] = 'z' - (step - (word[i] - 'a') - 1);
		else
			word[i] -= step;
	}

	SetWindowText(hwEdit, word);
}

/*
int main()
{
	for (int i = 0; i < ('z' + 1 - 'a'); cout << (char)('a' + i) << ' ', i++);
	cout << endl;


	int step;
	cout << "step: ";
	cin >> step;

	char str[100];
	cout << "word: ";
	cin.ignore();
	cin.getline(str, 100);

	for (int i = 0; i < strlen(str); i++) {

		if (str[i] == ' ')
			continue;
		if (str[i] + step > 'z')
			str[i] = 'a' + step - ('z' - str[i]) - 1;
		else
			str[i] += step;
	}

	cout << "Encode: " << str << endl;

	for (int i = 0; i < strlen(str); i++) {

		if (str[i] == ' ')
			continue;
		if (str[i] - step < 'a')
			str[i] = 'z' - (step - (str[i] - 'a') - 1);
		else
			str[i] -= step;
	}

	cout << "Decode: " << str << endl;

	system("pause");
	return 0;
}
*/