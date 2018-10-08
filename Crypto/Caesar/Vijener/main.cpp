#include <Windows.h>
#include <iostream>
#include <string>
#include "Algo.h"

using namespace std;

HWND hwButton_encode, hwButton_decode;
HWND hwEdit_word, hwEdit_key, hwEdit_encode, hwEdit_decode;
HWND hwStatic_word, hwStatic_step, hwStatic_encode, hwStatic_decode;

#define hmButtonEncode (HMENU)1
#define hmButtonDecode (HMENU)2


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CREATE:
	{
		hwStatic_word = CreateWindow("STATIC", "������� �����:", WS_CHILD | WS_VISIBLE, 150, 15, 500, 32, hWnd, NULL, NULL, NULL);
		hwStatic_word = CreateWindow("STATIC", "������� key:", WS_CHILD | WS_VISIBLE, 15, 15, 100, 32, hWnd, NULL, NULL, NULL);
		hwStatic_word = CreateWindow("STATIC", "�������������� �����:", WS_CHILD | WS_VISIBLE, 15, 175, 200, 32, hWnd, NULL, NULL, NULL);
		hwStatic_word = CreateWindow("STATIC", "�������������� �����:", WS_CHILD | WS_VISIBLE, 15, 350, 200, 32, hWnd, NULL, NULL, NULL);

		hwButton_encode = CreateWindow("BUTTON", "������������", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, 15, 125, 200, 40, hWnd, hmButtonEncode, NULL, NULL);
		hwButton_decode = CreateWindow("BUTTON", "������������", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, 15, 300, 200, 40, hWnd, hmButtonDecode, NULL, NULL);

		hwEdit_key = CreateWindow("EDIT", "CAT", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 40, 100, 20, hWnd, NULL, NULL, NULL);
		hwEdit_word = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 150, 40, 600, 20, hWnd, NULL, NULL, NULL);
		hwEdit_encode = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 200, 600, 20, hWnd, NULL, NULL, NULL);
		hwEdit_decode = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 375, 600, 20, hWnd, NULL, NULL, NULL);

		break;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == (int)hmButtonEncode)
			Encode(hwEdit_word, hwEdit_key, hwEdit_encode);
		else if (LOWORD(wParam) == (int)hmButtonDecode)
			Decode(hwEdit_word, hwEdit_key, hwEdit_decode);

		break;
	}


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	}


	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	const char* name = "Caesar";

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(0, IDI_SHIELD);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = name;

	RegisterClass(&wndClass);

	int width = 800;
	int height = 500;

	HWND hwnd = CreateWindow(name, name, WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(name, hInstance);
	return (int)msg.wParam;
}