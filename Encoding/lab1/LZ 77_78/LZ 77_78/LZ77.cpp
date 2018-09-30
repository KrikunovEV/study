#include "LZ77.h"


void FillBuffer(char* Buffer, int& BufferIndex, char* Word, int& WordIndex) {

	for (; BufferIndex < (int)strlen(Buffer) && WordIndex < (int)strlen(Word); BufferIndex++, WordIndex++)
		Buffer[BufferIndex] = Word[WordIndex];
}


queue<Triplet> EncodeWordLZ77(char* Word, int DictionaryLen, int BufferLen) {

	char* Buffer = new char[BufferLen + 1];
	for (int i = 0; i < BufferLen; i++)
		Buffer[i] = ' ';
	Buffer[BufferLen] = 0;
	int BufferIndex = 0;


	char* Dictionary = new char[DictionaryLen + 1];
	for (int i = 0; i < DictionaryLen + 1; i++)
		Dictionary[i] = 0;
	int DictionaryIndex = 0;


	int WordIndex = 0;


	std::queue<Triplet> Triplets;

	// Init buffer firstly
	FillBuffer(Buffer, BufferIndex, Word, WordIndex);

	cout << Buffer << endl << BufferIndex;

	return Triplets;
}


char* DecodeWordLZ77(queue<Triplet> Triplets, int DictionaryLen, int BufferLen) {

}