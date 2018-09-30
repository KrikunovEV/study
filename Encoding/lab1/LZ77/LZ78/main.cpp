#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
using namespace std;


class Doublet {
public:
	Doublet(int _Pos, int _Symbol) : Pos(_Pos), Symbol(_Symbol) { ; }

	int Pos;
	char Symbol;

	void Print() {
		cout << '(' << Pos << ',' << Symbol << ')' << endl;
	}
};


list<Doublet> EncodeWordLZ78(string Word);
string DecodeWordLZ78(list<Doublet> Doublets);



int main() {

	cout << "Write Word for encoding: ";

	string Word;
	getline(cin, Word);

	list<Doublet> Doublets = EncodeWordLZ78(Word);

	cout << "Doublets: " << endl;
	for (Doublet var : Doublets)
		var.Print();

	string DecodedWord = DecodeWordLZ78(Doublets);
	cout << "Decoded word: " << DecodedWord << endl;

	system("pause");
	return 0;
}


list<Doublet> EncodeWordLZ78(string Word) {

	list<Doublet> Doublets;
	map<string, int> Dictionary;
	string buffer = "";

	for (int i = 0; i < (int)Word.length(); i++) {

		string temp = buffer + Word[i];

		// Не нашёл
		if (Dictionary.find(temp) == Dictionary.end())
		{
			Doublets.push_back(Doublet(Dictionary[buffer], Word[i]));
			Dictionary[temp] = (int)Dictionary.size();
			buffer = "";
		}
		// Нашёл
		else
			buffer = temp;

	}

	cout << "Dictionary:" << endl;
	for (std::pair<string, int> p : Dictionary)
		cout << "Key: " << p.first << ", Value: " << p.second << endl;
	cout << endl;
	
	return Doublets;
}


string DecodeWordLZ78(list<Doublet> Doublets) {
	
	string Word = "";
	vector<string> Dictionary;
	Dictionary.push_back("");
	
	for (Doublet doublet : Doublets)
	{
		string part = Dictionary.at(doublet.Pos) + doublet.Symbol;
		Word += part;
		Dictionary.push_back(part);
	}

	cout << endl << "Dictionary: ";
	for (string s : Dictionary)
		cout << s << endl;
	cout << endl;

	return Word;
}