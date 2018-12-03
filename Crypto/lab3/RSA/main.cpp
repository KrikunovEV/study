#include <Windows.h>
#include <iostream>
#include <string>
#include "Algo.h"

using namespace std;

HWND hwButton_PAE;
HWND hwEdit_q, hwEdit_p, hwEdit_text, hwEdit_encoded, hwEdit_decoded;

#define hmButtonEncode (HMENU)1


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CREATE:
	{
		CreateWindow("STATIC", "Text:", WS_CHILD | WS_VISIBLE, 15, 15, 300, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "q:", WS_CHILD | WS_VISIBLE, 15, 45, 50, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "p:", WS_CHILD | WS_VISIBLE, 15, 75, 50, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "Зашифрованное:", WS_CHILD | WS_VISIBLE, 15, 180, 300, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "Расшифрованное:", WS_CHILD | WS_VISIBLE, 15, 240, 300, 30, hWnd, NULL, NULL, NULL);

		hwButton_PAE = CreateWindow("BUTTON", "Вперёд !", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, 15, 130, 200, 40, hWnd, hmButtonEncode, NULL, NULL);

		hwEdit_text = CreateWindow("EDIT", "Hello, my friend !", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 50, 15, 300, 20, hWnd, NULL, NULL, NULL);
		hwEdit_q = CreateWindow("EDIT", "31", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 30, 45, 50, 20, hWnd, NULL, NULL, NULL);
		hwEdit_p = CreateWindow("EDIT", "17", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 30, 75, 40, 20, hWnd, NULL, NULL, NULL);
		hwEdit_encoded = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 210, 300, 20, hWnd, NULL, NULL, NULL);
		hwEdit_decoded = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 270, 300, 20, hWnd, NULL, NULL, NULL);

		break;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == (int)hmButtonEncode)
			RSA(hwEdit_q, hwEdit_p, hwEdit_text, hwEdit_encoded, hwEdit_decoded);

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
	const char* name = "PAE";

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

	int width = 400;
	int height = 400;

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