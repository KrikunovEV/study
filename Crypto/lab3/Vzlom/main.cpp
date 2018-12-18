#include <Windows.h>
#include <iostream>
#include <string>
#include "Algo.h"

using namespace std;

HWND hwButton_GO;
HWND hwEdit_N, hwEdit_B, hwEdit_p, hwEdit_q, hwEdit_e, hwEdit_d;

#define hmButtonEncode (HMENU)1


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CREATE:
	{
		CreateWindow("STATIC", "N:", WS_CHILD | WS_VISIBLE, 15, 15, 300, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "B:", WS_CHILD | WS_VISIBLE, 15, 45, 50, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "p:", WS_CHILD | WS_VISIBLE, 15, 180, 50, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "q:", WS_CHILD | WS_VISIBLE, 15, 240, 50, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "е:", WS_CHILD | WS_VISIBLE, 15, 300, 50, 30, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "d:", WS_CHILD | WS_VISIBLE, 15, 360, 50, 30, hWnd, NULL, NULL, NULL);

		hwButton_GO = CreateWindow("BUTTON", "¬перЄд !", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, 15, 100, 200, 40, hWnd, hmButtonEncode, NULL, NULL);

		hwEdit_N = CreateWindow("EDIT", "10001", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 50, 15, 100, 20, hWnd, NULL, NULL, NULL);
		hwEdit_B = CreateWindow("EDIT", "10", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 50, 45, 100, 20, hWnd, NULL, NULL, NULL);
		hwEdit_p = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 50, 180, 300, 20, hWnd, NULL, NULL, NULL);
		hwEdit_q = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 50, 240, 300, 20, hWnd, NULL, NULL, NULL);
		hwEdit_e = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 50, 300, 300, 20, hWnd, NULL, NULL, NULL);
		hwEdit_d = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 50, 360, 300, 20, hWnd, NULL, NULL, NULL);

		break;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == (int)hmButtonEncode)
			Vzlom(hwEdit_B, hwEdit_N, hwEdit_p, hwEdit_q, hwEdit_e, hwEdit_d);

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
	const char* name = "Hack RSA";

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
	int height = 600;

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