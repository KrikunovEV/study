#include <Windows.h>
#include <iostream>
#include <string>
#include "Algo.h"

using namespace std;

HWND hwButton_PAE;
HWND hwEdit_A, hwEdit_B, hwEdit_x, hwEdit_y, hwEdit_d;

#define hmButtonEncode (HMENU)1
#define hmButtonDecode (HMENU)2


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CREATE:
	{
		CreateWindow("STATIC", "PAE: Ax + By = d", WS_CHILD | WS_VISIBLE, 15, 15, 500, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "¬ведите A:", WS_CHILD | WS_VISIBLE, 15, 45, 120, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "¬ведите B:", WS_CHILD | WS_VISIBLE, 15, 75, 120, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "–езультат:", WS_CHILD | WS_VISIBLE, 15, 190, 120, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "x = ", WS_CHILD | WS_VISIBLE, 15, 220, 100, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "y = ", WS_CHILD | WS_VISIBLE, 100, 220, 100, 32, hWnd, NULL, NULL, NULL);
		CreateWindow("STATIC", "d = ", WS_CHILD | WS_VISIBLE, 185, 220, 100, 32, hWnd, NULL, NULL, NULL);

		hwButton_PAE = CreateWindow("BUTTON", "¬перЄд !", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, 15, 130, 200, 40, hWnd, hmButtonEncode, NULL, NULL);

		hwEdit_A = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 45, 50, 20, hWnd, NULL, NULL, NULL);
		hwEdit_B = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 100, 75, 50, 20, hWnd, NULL, NULL, NULL);
		hwEdit_x = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 40, 220, 40, 20, hWnd, NULL, NULL, NULL);
		hwEdit_y = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 125, 220, 40, 20, hWnd, NULL, NULL, NULL);
		hwEdit_d = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 210, 220, 40, 20, hWnd, NULL, NULL, NULL);

		break;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == (int)hmButtonEncode)
			PAE(hwEdit_A, hwEdit_B, hwEdit_x, hwEdit_y, hwEdit_d);

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