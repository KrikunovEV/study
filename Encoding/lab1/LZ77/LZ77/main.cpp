#include <iostream>
#include <list>
#include <string>
using namespace std;


class Triplet {
public:
	Triplet(int _Npos, int _Len, char _Symbol) : Npos(_Npos), Len(_Len), Symbol(_Symbol) { ; }

	int Npos;
	int Len;
	char Symbol;

	void Print() {
		cout << '(' << Npos << ',' << Len << ',' << Symbol << ')' << endl;
	}
};


list<Triplet> EncodeWordLZ77(string Word, int DictionaryLen, int BufferLen);
string DecodeWordLZ77(list<Triplet> Triplets);


int main() {

	cout << "Write Word for encoding: ";

	string Word;
	getline(cin, Word);

	int DictionaryLen = 7;
	int BufferLen = 5;

	list<Triplet> Triplets = EncodeWordLZ77(Word, DictionaryLen, BufferLen);
	
	for (Triplet triplet : Triplets)
		triplet.Print();
	cout << endl;

	string decodedWord = DecodeWordLZ77(Triplets);
	cout << "Decoded word: " << decodedWord << endl;

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

	return Word;
}