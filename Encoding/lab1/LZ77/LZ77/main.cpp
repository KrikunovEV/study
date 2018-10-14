#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <algorithm>

using namespace std;


class Triplet {
public:
	Triplet(int _Npos, int _Len, char _Symbol) : Npos(_Npos), Len(_Len), Symbol(_Symbol) { ; }

	unsigned char Npos;
	unsigned char Len;
	char Symbol;

	void Print() {
		cout << '(' << Npos << ',' << Len << ',' << Symbol << ')' << endl;
	}
};


list<Triplet> EncodeWordLZ77(string Word, int DictionaryLen, int BufferLen);
string DecodeWordLZ77(list<Triplet> Triplets);


int main() {
	setlocale(LC_ALL, "rus");

	cout << "Write Word for encoding: ";

	//string Word;
	//getline(cin, Word);


	ifstream f("../we.txt");
	f.seekg(0, ios::end);
	size_t size = f.tellg();
	string Words(size, ' ');
	f.seekg(0);
	f.read(&Words[0], size);
	//Words.erase(remove(Words.begin(), Words.end(), ' '), Words.end());
	//Words.erase(remove(Words.begin(), Words.end(), '\n'), Words.end());

	int DictionaryLen = 256;
	int BufferLen = 256;

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

	while (index < Word.length()) {

		int Npos = 0;
		int Len = 0;

		int start = (index - 1 - DictionaryLen);
		start = start < 0 ? 0 : start;
		for (int i = start; i < index; i++) {
			if (Word[i] == Word[index]) {
				Npos = index - i;

				for (Len = 1; i + Len < index && Len < BufferLen && Word[Len + i] == Word[index + Len] && index + Len < Word.length(); Len++);

				break;
			}
		}

		index += Len;
		Triplets.push_back(Triplet(Npos, Len, Word[index]));
		index++;
	}


	ofstream file ("../encoded.txt", ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "םו םאר¸כ פאיכ";
		system("pause");
	}
	for (Triplet triplet : Triplets) {
		file.write((char*)&triplet.Npos, sizeof(unsigned char));
		file.write((char*)&triplet.Len, sizeof(unsigned char));
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