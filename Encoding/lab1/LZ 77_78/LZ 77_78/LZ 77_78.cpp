#include "LZ77.h"

using namespace std;

int main()
{
	char* Word = new char[32];
    cout << "Write word which will be encoded with LZ 77: ";
	cin >> Word;
	
	int DictionaryLen = 7;
	int BufferLen = 5;

	queue<Triplet> Triplets = EncodeWordLZ77(Word, DictionaryLen, BufferLen);




	return 0;
}