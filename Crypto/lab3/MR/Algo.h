#pragma once
#include <Windows.h>
#include <iostream>
#include <bitset>

using namespace std;

void MillerRabin(HWND n, HWND r, HWND out);
int pow(int a, int b, int mod);