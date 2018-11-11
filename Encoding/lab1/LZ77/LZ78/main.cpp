#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
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


	list<Doublet> Doublets = EncodeWordLZ78(Words);

	//cout << "Doublets: " << endl;
	//for (Doublet var : Doublets)
		//var.Print();

	string DecodedWord = DecodeWordLZ78(Doublets);
	//cout << "Decoded word: " << DecodedWord << endl;

	system("pause");
	return 0;
}


list<Doublet> EncodeWordLZ78(string Word) {

	list<Doublet> Doublets;
	map<string, int> Dictionary;
	string buffer = "";

	for (int i = 0; i < (int)Word.length(); i++) {

		string temp = buffer + Word[i];

		// Íו םאר¸כ
		if (Dictionary.find(temp) == Dictionary.end())
		{
			Doublets.push_back(Doublet(Dictionary[buffer], Word[i]));
			Dictionary[temp] = (int)Dictionary.size();
			buffer = "";
		}
		// Íאר¸כ
		else
			buffer = temp;
	}

	if (buffer.length() != 0) {
		Doublets.push_back(Doublet(Dictionary[buffer], 0));
	}

	/*cout << "Dictionary:" << endl;
	for (std::pair<string, int> p : Dictionary)
		cout << "Key: " << p.first << ", Value: " << p.second << endl;
	cout << endl;*/

	ofstream file("../encoded2.txt", ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "םו םאר¸כ פאיכ";
		system("pause");
	}
	for (Doublet doublet : Doublets) {
		file.write((char*)&doublet.Pos, sizeof(int));
		file.write((char*)&doublet.Symbol, sizeof(char));
	}
	file.close();
	
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

	/*cout << endl << "Dictionary: ";
	for (string s : Dictionary)
		cout << s << endl;
	cout << endl;*/

	ofstream file("../decoded2.txt");
	if (!file.is_open()) {
		cout << "םו םאר¸כ פאיכ";
		system("pause");
	}
	file << Word;
	file.close();

	return Word;
}