#include <Windows.h>
#include <iostream>
#include <string>
#include "Algo.h"

using namespace std;

HWND hwButton_MR;
HWND hwEdit_n, hwEdit_r, hwEdit_out;

#define hmButtonEncode (HMENU)1


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CREATE:
	{
		CreateWindow("STATIC", "¬ведите число:", WS_CHILD | WS_VISIBLE, 15, 15, 200, 32, hWnd, NULL, NULL, NULL);

		hwButton_MR = CreateWindow("BUTTON", "¬перЄд !", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, 15, 80, 200, 40, hWnd, hmButtonEncode, NULL, NULL);

		hwEdit_n = CreateWindow("EDIT", "71", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 45, 50, 20, hWnd, NULL, NULL, NULL);
		hwEdit_r = CreateWindow("EDIT", "15", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 45, 50, 20, hWnd, NULL, NULL, NULL);
		hwEdit_out = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 15, 135, 200, 20, hWnd, NULL, NULL, NULL);

		break;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == (int)hmButtonEncode)
			MillerRabin(hwEdit_n, hwEdit_r, hwEdit_out);

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

	int width = 300;
	int height = 300;

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