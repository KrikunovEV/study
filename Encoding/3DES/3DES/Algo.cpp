#include "Algo.h"


bitset<64> KeyFromStringToBinary(string Key)
{
	bitset<64> output;
	int len = (int)Key.length();
	int offset = 0;

	for (int i = 0; i < len; i++, offset++)
	{
		bitset<8> bits(Key[i]);
		int ones = 0;

		for (int j = 0; j < 8; j++) {
			output[i * 8 + j + offset] = bits[j];
			if (bits[j] == 1)
				ones++;
		}

		if (ones % 2 == 0)
			output[(i + 1) * 8 + offset] = 1;
		else
			output[(i + 1) * 8 + offset] = 0;
	}

	return output;
}


bitset<64> FromStringToBinary(string Text)
{
	bitset<64> output;
	int len = (int)Text.length();

	for (int i = 0; i < len; i++)
	{
		bitset<8> bits(Text[i]);

		for (int j = 0; j < 8; j++)
			output[i * len + j] = bits[j];
	}

	return output;
}


string FromBinaryToString(bitset<64> Text)
{
	string output = "";

	for (int i = 0; i < 8; i++)
	{
		bitset<8> bits;

		for (int j = 0; j < 8; j++)
			bits[j] = Text[i * 8 + j];

		output += (char)bits.to_ulong();
	}

	return output;
}


string FromBinaryToSring(bitset<64> Text)
{
	return string("12345678");
}


bitset<64> IPpermutation(bitset<64> input)
{
	int indices[64] =
	{
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7
	};

	bitset<64> output;
	for (int i = 0; i < 64; i++)
		output[i] = input[indices[i] - 1];
	
	return output;
}


bitset<64> IPpermutationInv(bitset<64> input)
{
	int indices[64] =
	{
		40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41, 9, 49, 17, 57, 25
	};

	bitset<64> output;
	for (int i = 0; i < 64; i++)
		output[i] = input[indices[i] - 1];

	return output;
}


bitset<48> PboxExtension(bitset<32> input)
{
	int indices[48] =
	{
		32, 1, 2, 3, 4, 5,
		4, 5, 6, 7, 8, 9,
		8, 9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32, 1
	};

	bitset<48> output;
	for (int i = 0; i < 48; i++)
		output[i] = input[indices[i] - 1];

	return output;
}


bitset<32> PboxStraight(bitset<32> input)
{
	int indices[32] =
	{
		16, 7, 20, 21, 29, 12, 28, 17,
		1, 15, 23, 26, 5, 18, 31, 10,
		2, 8, 24, 14, 32, 27, 3, 9,
		19, 13, 30, 6, 22, 11, 4, 25
	};

	bitset<32> output;
	for (int i = 0; i < 32; i++)
		output[i] = input[indices[i] - 1];

	return output;
}


bitset<32> DES(bitset<32> Text, bitset<48> RoundKey)
{
	bitset<48> output = PboxExtension(Text);

	output ^= RoundKey;

	bitset<32>	output2 = Sboxes(output);
				output2 = PboxStraight(output2);

	return output2;
}


bitset<64> TripleDESAlgorithm(bitset<64> Text, bitset<56> Key1, bitset<56> Key2, bitset<56> Key3, bitset<28> C[3], bitset<28> D[3])
{
	// for Key
	bitset<28> C1, C2, C3, D1, D2, D3;
	for (int i = 0; i < 56; i++)
		if (i < 28) {
			C1[i] = Key1[i];
			C2[i] = Key2[i];
			C3[i] = Key3[i];
		}
		else {
			D1[i - 28] = Key1[i];
			D2[i - 28] = Key2[i];
			D3[i - 28] = Key3[i];
		}

	// for DES
	bitset<32> L, R;
	for (int i = 0; i < 64; i++)
		if (i < 32)
			L[i] = Text[i];
		else
			R[i - 32] = Text[i];


	// 16 rounds
	for (int i = 0; i < 16; i++)
	{
		C1 = KeyShiftLeft(C1, i);
		C2 = KeyShiftLeft(C2, i);
		C3 = KeyShiftLeft(C3, i);
		D1 = KeyShiftLeft(D1, i);
		D2 = KeyShiftLeft(D2, i);
		D3 = KeyShiftLeft(D3, i);
		cout << "C: " << C1.to_string() << endl;
		cout << "D: " << D1.to_string() << endl;
		
		bitset<48> RoundKey1 = KeyCompress(C1, D1);
		bitset<48> RoundKey2 = KeyCompress(C2, D2);
		bitset<48> RoundKey3 = KeyCompress(C3, D3);
		cout << "Round Key: " << RoundKey1.to_string() << endl;

		bitset<32> XOR = L ^ DES(DES(DES(R, RoundKey1), RoundKey2), RoundKey3);
		cout << "XOR: " << XOR.to_string() << endl << endl;

		L = R;
		R = XOR;
	}

	C[0] = C1;
	C[1] = C2;
	C[2] = C3;

	D[0] = D1;
	D[1] = D2;
	D[2] = D3;

	bitset<64> output;
	for (int i = 0; i < 64; i++)
		if (i < 32)
			output[i] = L[i];
		else
			output[i] = R[i - 32];

	return output;
}


bitset<28> KeyShiftLeft(bitset<28> KeyPart, int round)
{
	bitset<28> output = KeyPart;

	if (round == 0 || round == 1 || round == 8 || round == 15) {
		int shifted = output[27];
		output <<= 1;
		output[0] = shifted;
	}
	else {
		int shifted1 = output[27];
		int shifted2 = output[26];
		output <<= 2;
		output[0] = shifted2;
		output[1] = shifted1;
	}

	return output;
}


bitset<28> KeyShiftRight(bitset<28> KeyPart, int round)
{
	bitset<28> output = KeyPart;

	if (round == 0 || round == 1 || round == 8 || round == 15) {
		int shifted = output[0];
		output >>= 1;
		output[27] = shifted;
	}
	else {
		int shifted1 = output[0];
		int shifted2 = output[1];
		output >>= 2;
		output[27] = shifted2;
		output[26] = shifted1;
	}

	return output;
}


bitset<48> KeyCompress(bitset<28> C, bitset<28> D)
{
	int indices[48] =
	{
		14, 17, 11, 24, 1, 5, 3, 28,
		15, 6, 21, 10, 23, 19, 12, 4,
		26, 8, 16, 7, 27, 20, 13, 2,
		41, 52, 31, 37, 47, 55, 30, 40,
		51, 45, 33, 48, 44, 49, 39, 56,
		34, 53, 46, 42, 50, 36, 29, 32
	};

	bitset<56> bits;
	for (int i = 0; i < 56; i++)
		if (i < 28)
			bits[i] = C[i];
		else
			bits[i] = D[i - 28];

	bitset<48> output;
	for (int i = 0; i < 48; i++)
		output[i] = bits[indices[i] - 1];

	return output;
}


bitset<56> KeyPermutation(bitset<64> input)
{
	int indices[56] =
	{
		57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
		10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
		63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
		14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
	};

	bitset<56> output;
	for (int i = 0; i < 56; i++)
		output[i] = input[indices[i] - 1];

	return output;
}


bitset<32> Sboxes(bitset<48> input)
{
	int S[8][4][16] =
	{
		{
			{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
			{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
			{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
			{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
		},
		{
			{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
			{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
			{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
			{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
		},
		{
			{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
			{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
			{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
			{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
		},
		{
			{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
			{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
			{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
			{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
		},
		{
			{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
			{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
			{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
			{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
		},
		{
			{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
			{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
			{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
			{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
		},
		{
			{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
			{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
			{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
			{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
		},
		{
			{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
			{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
			{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
			{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
		}
	};

	bitset<32> output;
	for (int i = 0; i < 8; i++) {
		bitset<2> row;
		bitset<4> column;

		row[0] = input[i * 6];
		row[1] = input[i * 6 + 5];

		column[0] = input[i * 6 + 1];
		column[1] = input[i * 6 + 2];
		column[2] = input[i * 6 + 3];
		column[3] = input[i * 6 + 4];

		bitset<4> bits = S[i][row.to_ulong()][column.to_ulong()];

		for (int j = 0; j < 4; j++)
			output[i * 4 + j] = bits[j];
	}

	return output;
}


bitset<64> DecryptTripleDES(bitset<64> Text, bitset<28> C[3], bitset<28> D[3])
{
	// for Key
	bitset<28> C1, C2, C3, D1, D2, D3;

	C1 = C[0];
	C2 = C[1];
	C3 = C[2];

	D1 = D[0];
	D2 = D[1];
	D3 = D[2];

	// for DES
	bitset<32> L, R;
	for (int i = 0; i < 64; i++)
		if (i < 32)
			L[i] = Text[i];
		else
			R[i - 32] = Text[i];


	// 16 rounds
	for (int i = 0; i < 16; i++)
	{
		// Lnew = R
		// Rnew = L ^ DES(R)

		// R = Lnew
		// L = Rnew ^ DES(Lnew)



		bitset<48> RoundKey1 = KeyCompress(C1, D1);
		bitset<48> RoundKey2 = KeyCompress(C2, D2);
		bitset<48> RoundKey3 = KeyCompress(C3, D3);

		bitset<32> XOR = R ^ DES(DES(DES(L, RoundKey3), RoundKey2), RoundKey1);

		R = L;
		L = XOR;

		C1 = KeyShiftRight(C1, 15 - i);
		C2 = KeyShiftRight(C2, 15 - i);
		C3 = KeyShiftRight(C3, 15 - i);
		D1 = KeyShiftRight(D1, 15 - i);
		D2 = KeyShiftRight(D2, 15 - i);
		D3 = KeyShiftRight(D3, 15 - i);
	}

	bitset<64> output;
	for (int i = 0; i < 64; i++)
		if (i < 32)
			output[i] = L[i];
		else
			output[i] = R[i - 32];

	return output;
}