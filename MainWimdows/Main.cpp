#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include<CommCtrl.h>
#include "resource.h"

#pragma comment(lib)

CONST CHAR g_sz_WINDOW_CLASS[] = "My first Window";
TOOLINFO g_toolItem;
HWND g_hwndTrackingTT;

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateTrackingToolTip(INT toolID, HWND hwnd, LPCSTR lpsz_text);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR ipCmdLine, INT nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;

	//wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	//wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIcon = (HICON)LoadImage(hInstance, "icon.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "sun.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

		wc.hInstance = hInstance;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = g_sz_WINDOW_CLASS;
	if (RegisterClassEx(&wc) == NULL)
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	
	}
	INT screen_width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);
	INT window_width = screen_width * 3 / 4;
	INT window_height = screen_height * 3 / 4;
	INT window_start_x = screen_width / 8;
	INT window_start_y = screen_height / 8;
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		window_start_x, window_start_y,
		window_width, window_height,
		//CW_USEDEFAULT, CW_USEDEFAULT,
		//CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		g_hwndTrackingTT = CreateTrackingToolTip(IDC_TOOLTIP, hwnd, "MyTooltip");
		return TRUE;
	case WM_MOUSELEAVE:
		SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, FALSE, (LPARAM)&g_toolItem);
		g_trackingMouse = FALSE;
	case WM_MOUSEMOVE: 
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		CONST INT SIZE = 256;
		CHAR sz_title[SIZE]{};
		sprintf(sz_title, "%s Position: %ix%i, Size: %ix%i", g_sz_WINDOW_CLASS,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	}
	break;
	case WM_COMMAND:
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd); break;
	default:		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

HWND CreateTrackingToolTip(INT toolID, HWND hwnd, LPSTR lpszText)
{
	HWND hwndTT = CreateWindowEx(
		WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hwnd,
		NULL,
		GetModuleHandle(NULL),
		NULL

		);

	if (hwndTT == NULL)
	{
		MessageBox(hwnd, "Tooltip creation failed", "Error", MB_OK | MB_ICONERROR);
			return NULL;
	}

	g_toolItem.cbSize = sizeof(TOOLINFO);
	g_toolItem.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
	g_toolItem.hwnd = hwnd;
	g_toolItem.hinst = GetModuleHandle(NULL);
	g_toolItem.lpszText = lpszText;
	g_toolItem.uId = (UINT_PTR)hwnd;
	GetClientRect(hwnd, &g_toolItem.rect);

	//������� ���������� ����������
	SendMessage(hwndTT, TTM_ADDTOOL)
}

