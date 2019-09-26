#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

HWND hwEdit_sum, hwEdit_time, hwEdit_perc, hwEdit_info;
HWND hwComb_type;
HWND hwStatic_sum, hwStatic_type, hwStatic_time, hwStatic_perc;
HWND hwButton_calculate;

#define hmButton (HMENU)1

enum etCreditType
{
	eDIFF,
	eANYEN
};


void CalculateDiff(double CreditSum, double Perc, int Months);
void CalculateAyen(double CreditSum, double Perc, int Months);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CREATE:
	{
		hwStatic_sum = CreateWindow("STATIC", "����� �������:", WS_CHILD | WS_VISIBLE, 15, 15, 200, 32, hWnd, NULL, NULL, NULL);
		hwStatic_type = CreateWindow("STATIC", "��� �������:", WS_CHILD | WS_VISIBLE, 15, 90, 200, 32, hWnd, NULL, NULL, NULL);
		hwStatic_time = CreateWindow("STATIC", "���� �������(�������):", WS_CHILD | WS_VISIBLE, 15, 165, 200, 32, hWnd, NULL, NULL, NULL);
		hwStatic_perc = CreateWindow("STATIC", "������(%):", WS_CHILD | WS_VISIBLE, 15, 240, 200, 32, hWnd, NULL, NULL, NULL);

		hwEdit_sum = CreateWindow("EDIT", "2000000", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, 15, 40, 200, 22, hWnd, NULL, NULL, NULL);
		hwEdit_time = CreateWindow("EDIT", "72", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, 15, 190, 200, 22, hWnd, NULL, NULL, NULL);
		hwEdit_perc = CreateWindow("EDIT", "12.6", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER, 15, 265, 200, 22, hWnd, NULL, NULL, NULL);
		hwEdit_info = CreateWindow("EDIT", "...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, 300, 15, 800, 500, hWnd, NULL, NULL, NULL);

		hwComb_type = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 15, 115, 200, 100, hWnd, NULL, NULL, NULL);
		SendMessage(hwComb_type, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"������������������");
		SendMessage(hwComb_type, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"�����������");
		SendMessage(hwComb_type, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		hwButton_calculate = CreateWindow("BUTTON", "���������", WS_VISIBLE | WS_CHILD | WS_BORDER, 15, 330, 200, 32, hWnd, hmButton, NULL, NULL);

		break;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == (int)hmButton)
		{
			int nIndex = SendMessage(hwComb_type, CB_GETCURSEL, 0, 0);
			char szType[32];
			SendMessage(hwComb_type, CB_GETLBTEXT, (WPARAM)nIndex, (LPARAM)szType);

			etCreditType eType = (strcmp(szType, "������������������") == 0) ? eDIFF : eANYEN;

			char buf[255];
			GetWindowText(hwEdit_sum, buf, 255);
			double CreditSum = atof(buf);

			GetWindowText(hwEdit_perc, buf, 255);
			double Perc = atof(buf) / 100.0;

			GetWindowText(hwEdit_time, buf, 255);
			int Months = atoi(buf);

			if (eType == eDIFF)
			{
				CalculateDiff(CreditSum, Perc, Months);
			}
			else
			{
				CalculateAyen(CreditSum, Perc, Months);
			}
		}

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
	const char* name = "Calculator";

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

	int width = 1200;
	int height = 600;

	HWND hwnd = CreateWindow(name, name, WS_OVERLAPPEDWINDOW, 50, 50, width, height, NULL, NULL, hInstance, NULL);

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


void CalculateDiff(double CreditSum, double Perc, int Months)
{
	double Sum_keep = CreditSum;
	char szInfo[1000000];

	double MonthSum = CreditSum / (double)Months;
	sprintf(szInfo, "��� �������: ������������������\r\n\r\n����������� ����� �� ��������� �����: %.2f\r\n\r\n", MonthSum);

	sprintf(szInfo, "%s                        ����������� �������:          ����������� ����� � ���������:\r\n", szInfo);

	double* Percs = new double[Months];
	for (int i = 0; i < Months; i++)
	{
		double NumDay = 12.0;

		Percs[i] = CreditSum * Perc / NumDay;
		CreditSum -= MonthSum;
	}

	double* SumPercs = new double[Months];
	for (int i = 0; i < Months; i++)
	{
		SumPercs[i] = Percs[i] + MonthSum;
		sprintf(szInfo, "%s%d-� �����:       %.2f                                    %.2f\r\n", szInfo, i + 1, Percs[i], SumPercs[i]);
	}

	double FullSum = 0;
	for (int i = 0; i < Months; i++)
	{
		FullSum += SumPercs[i];
	}
	sprintf(szInfo, "%s\r\n�������� ����� �� ������� = %.2f\r\n", szInfo, FullSum);

	sprintf(szInfo, "%s��������� = %.2f", szInfo, FullSum - Sum_keep);

	SetWindowText(hwEdit_info, szInfo);
}

void CalculateAyen(double CreditSum, double Perc, int Months)
{
	char szDebug[10000];

	long double Buf = pow(1.0 + Perc / 12.0, (double)Months);
	double Coef = (Perc / 12.0 * Buf) / (Buf - 1);

	char szInfo[1000000];
	sprintf(szInfo, "��� �������: �����������\r\n\r\n����������� ���������: %.2f\r\n\r\n", Coef);

	sprintf(szInfo, "%s����������� �����: %.2f\r\n", szInfo, CreditSum * Coef);
	sprintf(szInfo, "%s�������� �����:        %.2f\r\n", szInfo, CreditSum * Coef * Months);
	sprintf(szInfo, "%s���������:                   %.2f\r\n", szInfo, CreditSum * Coef * Months - CreditSum);

	SetWindowText(hwEdit_info, szInfo);
}