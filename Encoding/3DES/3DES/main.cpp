#include "Algo.h"
#include <string>


int main()
{
	string Text = "12345678"; // - 64 bit

	string Key1 = "abcdefg"; // - 56 bit
	string Key2 = "1234567"; // - 56 bit
	string Key3 = "QJGtfUs"; // - 56 bit

	bitset<64> TextBinary = FromStringToBinary(Text);

	bitset<64> KeyBinary1 = KeyFromStringToBinary(Key1);
	bitset<64> KeyBinary2 = KeyFromStringToBinary(Key2);
	bitset<64> KeyBinary3 = KeyFromStringToBinary(Key3);
	
	bitset<56> K1 = KeyPermutation(KeyBinary1);
	bitset<56> K2 = KeyPermutation(KeyBinary2);
	bitset<56> K3 = KeyPermutation(KeyBinary3);





	TextBinary = IPpermutation(TextBinary);

	bitset<28> C[3];
	bitset<28> D[3];
	TextBinary = TripleDESAlgorithm(TextBinary, K1, K2, K3, C, D);

	TextBinary = IPpermutationInv(TextBinary);

	string result = FromBinaryToString(TextBinary);
	cout << result << endl;




	TextBinary = FromStringToBinary(result);

	TextBinary = IPpermutationInv(TextBinary);

	TextBinary = DecryptTripleDES(TextBinary, C, D);

	TextBinary = IPpermutation(TextBinary);

	cout << FromBinaryToSring(TextBinary) << endl;


	system("pause");
	return 0;
}