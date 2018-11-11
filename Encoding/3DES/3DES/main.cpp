#include "Algo.h"
#include <string>


int main()
{
	string Text = "12345678"; // - 64 bit
	string Key = "ABCDEFG"; // - 56 bit

	bitset<64> TextBinary = FromStringToBinary(Text);
	bitset<64> KeyBinary = KeyFromStringToBinary(Key);
	
	bitset<56> K = KeyPermutation(KeyBinary);
	TextBinary = IPpermutation(TextBinary);

	TextBinary = TripleDESAlgorithm(TextBinary, K);

	TextBinary = IPpermutationInv(TextBinary);

	cout << FromBinaryToString(TextBinary);

	system("pause");
	return 0;
}