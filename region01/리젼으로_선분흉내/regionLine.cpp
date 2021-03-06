#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include <wingdi.h>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

//전역 변수들
HINSTANCE g_Inst;
HWND hChild;
HRGN hWndRgn;
POINT p1[4];
POINT p2[4];
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_CREATE: {
		//1. 상대적 위치를 적어야 되는 것 같음. 자식 윈도우 안에서의 
		POINT tp1 = { 1,0 };
		POINT tp2 = { 0, 1 };
		POINT tp3 = { 99, 100 };
		POINT tp4 = { 100, 99 };
		p1[0] = tp1;
		p1[1] = tp2;
		p1[2] = tp3;
		p1[3] = tp4;
		POINT tp5 = { 11, 10 };
		POINT tp6 = { 10, 11 };
		POINT tp7 = { 59, 60 };
		POINT tp8 = { 60, 59 };
		p2[0] = tp5;
		p2[1] = tp6;
		p2[2] = tp7;
		p2[3] = tp8;

		//1. child 한번 만들어보기 //100,100에 너비 100, 높이 100짜리
		hChild = CreateWindow(TEXT("child01"), NULL, WS_CHILD | WS_VISIBLE, 100, 100, 100, 100, hwnd, (HMENU)NULL, g_Inst, NULL);

		//2. 폴리곤 리젼 하나 만들기
		hWndRgn = CreatePolygonRgn(p1, 4, ALTERNATE);
		SetWindowRgn(hChild, hWndRgn, TRUE);

		//3. 자식 윈도우 옮겨보기 //리젼도 같이 따라서 움직인다. 
		MoveWindow(hChild, 0, 0, 100, 100, TRUE);

		//4. 리젼 offset 테스트해보기 // 리젼도 움직일까
		OffsetRgn(hWndRgn, 10, 10); //10,10 움직여라?
		SetWindowRgn(hChild, hWndRgn, TRUE); //안움직이는 군 그러면 point를 다시 만들어줄까.

		//5. 새로운 point 배열로 다시 리젼을 설정해볼까? //움직인다 =========>
		hWndRgn = CreatePolygonRgn(p2, 4, ALTERNATE);
		SetWindowRgn(hChild, hWndRgn, TRUE);
		break;
	}



					//======================마지막엔 콘솔 없애주기======================== 
	case WM_DESTROY: {

		PostQuitMessage(0);
		break;
	}

					 /* All other messages (a lot of them) are processed using default procedures */
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}



//-------------------------------------------------WIN MAIN-------------------------------------------------------------------
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	_wsetlocale(LC_ALL, _T("korean"));
	g_Inst = hInstance;
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

	//child01 class //검은 색의 아무것도 없는 child class
	wc.lpszClassName = TEXT("child01");
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	//proc은 winProc을 쓰자.
	RegisterClassEx(&wc);

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
