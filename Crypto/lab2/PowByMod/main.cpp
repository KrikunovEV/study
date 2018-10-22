#include <Windows.h>
#include <iostream>
#include <string>
#include "Algo.h"

using namespace std;

HWND hwButton_compute;
HWND hwEdit_a, hwEdit_b, hwEdit_mod, hwEdit_out;

#define hmButtonEncode (HMENU)1
#define hmButtonDecode (HMENU)2


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CREATE:
	{
		CreateWindow("STATIC", "Формула: a^b mod n", WS_CHILD | WS_VISIBLE, 30, 15, 200, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "a = ", WS_CHILD | WS_VISIBLE, 30, 52, 50, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "b = ", WS_CHILD | WS_VISIBLE, 30, 99, 50, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "n = ", WS_CHILD | WS_VISIBLE, 30, 146, 50, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "Результат:", WS_CHILD | WS_VISIBLE, 30, 245, 150, 32, hWnd, NULL, NULL, NULL);

		hwButton_compute = CreateWindow("BUTTON", "Вычислить", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, 30, 190, 200, 40, hWnd, hmButtonEncode, NULL, NULL);

		hwEdit_a = CreateWindow("EDIT", "2", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 55, 52, 100, 20, hWnd, NULL, NULL, NULL);
		hwEdit_b = CreateWindow("EDIT", "199", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 55, 99, 100, 20, hWnd, NULL, NULL, NULL);
		hwEdit_mod = CreateWindow("EDIT", "1003", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 55, 146, 100, 20, hWnd, NULL, NULL, NULL);
		hwEdit_out = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 115, 245, 100, 20, hWnd, NULL, NULL, NULL);

		break;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == (int)hmButtonEncode)
			Compute(hwEdit_a, hwEdit_b, hwEdit_mod, hwEdit_out);

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
	const char* name = "Pow by mod";

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

	int width = 280;
	int height = 350;

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