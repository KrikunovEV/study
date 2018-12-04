#pragma once
#include <math.h>
#include <bitset>
#include <Windows.h>
#include <string>
#include <iostream>
#include <gmp.h>
#pragma comment(lib, "gmp.lib")

using namespace std;

void RSA(HWND hwq, HWND hwp, HWND hwtext, HWND hwencoded, HWND hwdecoded);

bool IsPrime(long long n);

long long NOD(long long A, long long B);

long long PAE(long long e, long long phi);