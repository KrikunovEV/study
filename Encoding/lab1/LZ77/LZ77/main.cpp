#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

class Triplet {
public:
	Triplet(int _Npos, int _Len, char _Symbol) : Npos(_Npos), Len(_Len), Symbol(_Symbol) { ; }

	unsigned short Npos;
	unsigned short Len;
	char Symbol;

	void Print() {
		cout << '(' << Npos << ',' << Len << ',' << Symbol << ')' << endl;
	}
};


list<Triplet> EncodeWordLZ77(string Word, int DictionaryLen, int BufferLen);
string DecodeWordLZ77(list<Triplet> Triplets);


int main() {
	setlocale(LC_ALL, "rus");

	//cout << "Write Word for encoding: ";

	//string Word;
	//getline(cin, Word);


	ifstream f("../weq.txt");
	f.seekg(0, ios::end);
	size_t size = f.tellg();
	string Words(size, ' ');
	f.seekg(0);
	f.read(&Words[0], size);
	//Words.erase(remove(Words.begin(), Words.end(), ' '), Words.end());
	//Words.erase(remove(Words.begin(), Words.end(), '\n'), Words.end());

	int DictionaryLen = 1024;
	int BufferLen = 1024;

	list<Triplet> Triplets = EncodeWordLZ77(Words, DictionaryLen, BufferLen);
	
	//for (Triplet triplet : Triplets)
		//triplet.Print();
	//cout << endl;
	system("pause");

	string decodedWord = DecodeWordLZ77(Triplets);
	//cout << "Decoded word: " << decodedWord << endl;

	system("pause");
	return 0;
}


list<Triplet> EncodeWordLZ77(string Word, int DictionaryLen, int BufferLen)
{
	list<Triplet> Triplets;
	int index = 0;

	Triplets.push_back(Triplet(0, 0, Word[index]));
	Triplets.back().Print();
	index++;

	while (index < (int)Word.length()) {

		int Npos = 0;
		int Len = 0;

		// start for dictionary
		int start = index - DictionaryLen - 1;
		start = start < 0 ? 0 : start;

		int diclen = index - start;
		char* dictionary = new char[diclen + 1];
		dictionary[diclen] = 0;
		strncpy(dictionary, &Word[start], diclen);

		// end for buffer
		int end = index + BufferLen;
		end = end > Word.length() ? Word.length() : end;

		int bufflen = end - index;
		char* buffer = new char[bufflen + 1];
		buffer[bufflen] = 0;
		strncpy(buffer, &Word[index], bufflen);


		// find max entry
		for (int offset = 0; offset != bufflen; offset++)
		{
			char* temp = new char[bufflen - offset + 1];
			char* found = 0;

			strncpy(temp, buffer, bufflen - offset);
			temp[bufflen - offset] = 0;

			found = strstr(dictionary, temp);
			//cout << endl << dictionary << endl << temp << endl;

			if (found) {
				Len = bufflen - offset;
				Npos = diclen - (found - dictionary);
				//cout << "found" << endl;
				delete[] temp;
				break;
			}

			delete[] temp;
		}

		/*
		for (int i = start; i < index; i++) {
			if (Word[i] == Word[index]) {
				Npos = index - i;

				for (Len = 1; i + Len < index && Len < BufferLen && Word[Len + i] == Word[index + Len] && index + Len < Word.length(); Len++);

				break;
			}
		}
		*/

		index += Len;
		Triplets.push_back(Triplet(Npos, Len, Word[index]));
		Triplets.back().Print();
		index++;

		delete[] buffer;
		delete[] dictionary;
	}


	ofstream file ("../encoded.txt", ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "םו םאר¸כ פאיכ";
		system("pause");
	}
	for (Triplet triplet : Triplets) {
		file.write((char*)&triplet.Npos, sizeof(unsigned short));
		file.write((char*)&triplet.Len, sizeof(unsigned short));
		file.write((char*)&triplet.Symbol, sizeof(char));
	}
	file.close();


	return Triplets;
}


string DecodeWordLZ77(list<Triplet> Triplets)
{
	string Word = "";

	for (Triplet triplet : Triplets) {

		for (int i = Word.length() - triplet.Npos, len = 0; len < triplet.Len; i++, len++)
			Word += Word[i];

		Word += triplet.Symbol;
	}

	ofstream file("../decoded.txt");
	if (!file.is_open()) {
		cout << "םו םאר¸כ פאיכ";
		system("pause");
	}
	file << Word;
	file.close();

	return Word;
}