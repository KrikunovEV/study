#pragma once
#include <Windows.h>
#include <string>
using namespace std;

void Encode(char* word, int step, HWND hwEdit);
void Decode(char* word, int step, HWND hwEdit);