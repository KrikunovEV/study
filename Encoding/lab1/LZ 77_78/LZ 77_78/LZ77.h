#ifndef LZ77
#define LZ77

#include <string>
#include <queue>
#include <iostream>

using namespace std;


class Triplet {
public:
	Triplet(int _Npos, int _Len, int _Symbol) : Npos(_Npos), Len(_Len), Symbol(_Symbol) { ; }

	int Npos;
	int Len;
	char Symbol;
};


queue<Triplet> EncodeWordLZ77(char* Word, int DictionaryLen, int BufferLen);
char* DecodeWordLZ77(queue<Triplet> Triplets, int DictionaryLen, int BufferLen);
void FillBuffer(char* Buffer, int& BufferIndex, char* Word, int& WordIndex);



#endif
