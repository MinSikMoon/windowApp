
#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include <imm.h> //�̰� �� ���Խ�Ű��. 
#include "mTextSource.h"
#include "mstring.h"
#include "mTextPixel.h"
#include "mKeyboard.h"
#include "mLineMethods.h"


#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;



mKeyboard mk1;
mString m1;
mTextSource textSource;
//bool isProced = FALSE;
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// ���� ������ 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;

	switch (Message) {
	case WM_CREATE: {
		//1. textSource�� �ϳ� �־��. �׷��� textSource�� ����Ѵ�. 
		textSource.addText(TEXT("�ȳ��ϼ��� ���� ���ν��Դϴ�."));
		textSource.addText(TEXT("�ȳ��ϼ��� �ι�° �Դϴ� ����"));
		mString m1(TEXT("abcdefghijklmnopABCDEFGH������IJKLMNOPqrstuv1234567890ABCZQLEMONHIHIHI"));
		textSource.replaceTextAt(0, TEXT("�ȳ��ϼ��� ����° �Դϴ� ����"));
		textSource.addText(m1);
		textSource.show();
		//_tprintf(TEXT("%ls \n"), textSource.getTextAt(0));
		break;
	}

	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		mk1.mProc(hwnd, Message, wParam, lParam);
		break;
	}


	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		
		
		//���� mk1, �� Ű���忡 ��� ���ڿ��� ����ϰ� ���ݾ�. 
		//if (mk1.getMstrLength() > 0) //���ڿ� ���̰� 0�̸� ��¾���.
		//int t = autoLineSwitch(hdc, textSource.getTextAt(1), rect.right, -32, 16);
		//textSource.showAllText(hdc, rect.right, 0, 16); //100������ ������ textSource ��ü�� ����
		//printf("loopcnt  = %d \n", t);
		EndPaint(hwnd, &ps);
		break;
	}


				   //======================�������� �ܼ� �����ֱ�======================== 
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		//printf("wparam: %d \n", wParam);
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}




















//-------------------------------------------------WIN MAIN-------------------------------------------------------------------
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	_wsetlocale(LC_ALL, _T("korean"));

	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

			 /* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = _T("WindowClass");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, _T("WindowClass"), _T("Caption"), WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	/*
	This is the heart of our program where all input is processed and
	sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
	this loop will not produce unreasonably high CPU usage
	*/
	while (GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}