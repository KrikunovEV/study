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
	eTYPE_DIFF = 0,
	eTYPE_AYEN
};


void CalculateDiff(long double ldSum, byte btPerc, byte btTime);
void CalculateAyen(long double ldSum, byte btPerc, byte btTime);


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CREATE:
	{
		hwStatic_sum = CreateWindow("STATIC", "Сумма кредита:", WS_CHILD | WS_VISIBLE, 15, 15, 200, 32, hWnd, NULL, NULL, NULL);
		hwStatic_type = CreateWindow("STATIC", "Тип платежа:", WS_CHILD | WS_VISIBLE, 15, 90, 200, 32, hWnd, NULL, NULL, NULL);
		hwStatic_time = CreateWindow("STATIC", "Срок кредита(месяцев):", WS_CHILD | WS_VISIBLE, 15, 165, 200, 32, hWnd, NULL, NULL, NULL);
		hwStatic_perc = CreateWindow("STATIC", "Ставка(%):", WS_CHILD | WS_VISIBLE, 15, 240, 200, 32, hWnd, NULL, NULL, NULL);

		hwEdit_sum = CreateWindow("EDIT", "300000", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, 15, 40, 200, 22, hWnd, NULL, NULL, NULL);
		hwEdit_time = CreateWindow("EDIT", "6", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, 15, 190, 200, 22, hWnd, NULL, NULL, NULL);
		hwEdit_perc = CreateWindow("EDIT", "20", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, 15, 265, 200, 22, hWnd, NULL, NULL, NULL);
		hwEdit_info = CreateWindow("EDIT", "...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, 300, 15, 800, 500, hWnd, NULL, NULL, NULL);

		hwComb_type = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 15, 115, 200, 100, hWnd, NULL, NULL, NULL);
		SendMessage(hwComb_type, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Дифференцированный");
		SendMessage(hwComb_type, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Аннуитетный");
		SendMessage(hwComb_type, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		hwButton_calculate = CreateWindow("BUTTON", "Вычислить", WS_VISIBLE | WS_CHILD | WS_BORDER, 15, 330, 200, 32, hWnd, hmButton, NULL, NULL);

		break;
	}


	case WM_COMMAND:
	{
		if (LOWORD(wParam) == (int)hmButton)
		{
			int nIndex = SendMessage(hwComb_type, CB_GETCURSEL, 0, 0);
			char szType[32];
			SendMessage(hwComb_type, CB_GETLBTEXT, (WPARAM)nIndex, (LPARAM)szType);

			etCreditType eType = (strcmp(szType, "Дифференцированный") == 0) ? eTYPE_DIFF : eTYPE_AYEN;

			char buf[255];
			GetWindowText(hwEdit_sum, buf, 255);
			long double ldSum = atof(buf);

			GetWindowText(hwEdit_perc, buf, 255);
			byte btPerc = (byte)atoi(buf);
			if (btPerc > 100)
			{
				MessageBox(hWnd, "Введите ставку ниже 100%", "Предупреждение", MB_OK | MB_ICONWARNING);
				break;
			}

			GetWindowText(hwEdit_time, buf, 255);
			byte btTime = (byte)atoi(buf);
			if (btTime > 240)
			{
				MessageBox(hWnd, "Введите срок кредита меньше 20 лет(240 месяцев)", "Предупреждение", MB_OK | MB_ICONWARNING);
				break;
			}

			if (eType == eTYPE_DIFF)
			{
				CalculateDiff(ldSum, btPerc, btTime);
			}
			else
			{
				CalculateAyen(ldSum, btPerc, btTime);
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


void CalculateDiff(long double ldSum, byte btPerc, byte btTime)
{
	long double ldSum_keep = ldSum;
	char szInfo[1000000];

	long double dMonthPay = ldSum / btTime;
	sprintf(szInfo, "Тип кредита: Дифференцированный\r\n\r\nЕжемесячный платёж по основному долгу: %.2f\r\n\r\n", dMonthPay);

	sprintf(szInfo, "%sНачисленный процент:\r\n", szInfo);
	long double* pPercs = new long double[btTime];
	for (byte i = 0; i < btTime; i++)
	{
		double dNumDay = (i % 2 == 0) ? 31 : 30;
		pPercs[i] = ldSum * (double)btPerc / 100.0 * dNumDay / 365.0;
		sprintf(szInfo, "%s%d-й месяц = %.2f\r\n", szInfo, i + 1, pPercs[i]);

		ldSum -= dMonthPay;
	}

	long double* pSumPercs = new long double[btTime];
	sprintf(szInfo, "%s\r\nЕжемесячный платёж с процентом:\r\n", szInfo);
	for (byte i = 0; i < btTime; i++)
	{
		pSumPercs[i] = pPercs[i] + dMonthPay;
		sprintf(szInfo, "%s%d-й месяц = %.2f\r\n", szInfo, i + 1, pSumPercs[i]);
	}

	long double ldFullSum = 0;
	for (byte i = 0; i < btTime; i++)
	{
		ldFullSum += pSumPercs[i];
	}
	sprintf(szInfo, "%s\r\nИтоговый платёж по кредиту = %.2f\r\n", szInfo, ldFullSum);

	sprintf(szInfo, "%sПереплата = %.2f", szInfo, ldFullSum - ldSum_keep);

	SetWindowText(hwEdit_info, szInfo);
}

void CalculateAyen(long double ldSum, byte btPerc, byte btTime)
{
	long double ldBuf = pow(1 + btPerc/100.0, btTime);
	long double ldCoef = (btPerc/100.0 * ldBuf) / (ldBuf - 1);

	char szInfo[1000000];
	sprintf(szInfo, "Тип кредита: Аннуитетный\r\n\r\nКоэффициент аннуитета: %.2f\r\n\r\n", ldCoef);

	sprintf(szInfo, "%sЕжемесячный платёж: %.2f\r\n", szInfo, ldSum * ldCoef);
	sprintf(szInfo, "%sИтоговый платёж:        %.2f\r\n", szInfo, ldSum * ldCoef * btTime);
	sprintf(szInfo, "%sПереплата:                   %.2f\r\n", szInfo, ldSum * ldCoef * btTime - ldSum);

	SetWindowText(hwEdit_info, szInfo);
}