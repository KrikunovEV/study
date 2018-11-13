#pragma once
#include <iostream>
#include <bitset>

using namespace std;

bitset<64> KeyFromStringToBinary(string Key);

bitset<56> KeyPermutation(bitset<64> input);
bitset<28> KeyShiftLeft(bitset<28> KeyPart, int round);
bitset<28> KeyShiftRight(bitset<28> KeyPart, int round);
bitset<48> KeyCompress(bitset<28> C, bitset<28> D);

bitset<64> FromStringToBinary(string Text);
string FromBinaryToString(bitset<64> Text);
string FromBinaryToSring(bitset<64> Text);

bitset<64> IPpermutation(bitset<64> input);
bitset<64> IPpermutationInv(bitset<64> input);

bitset<64> DES(bitset<64> Text, bitset<48> RoundKey);
bitset<64> TripleDESAlgorithm(bitset<64> Text, bitset<56> Key1, bitset<56> Key2, bitset<56> Key3, bitset<28> C[3], bitset<28> D[3]);

bitset<64> DecryptTripleDES(bitset<64> Text, bitset<28> C[3], bitset<28> D[3]);

bitset<48> PboxExtension(bitset<32> input);
bitset<32> PboxStraight(bitset<32> input);
bitset<32> Sboxes(bitset<48> input);