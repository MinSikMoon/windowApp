#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

//전역 변수들
HINSTANCE g_Inst;

POINT ORIGIN_POS = { 0,0 }; //원점 
bool isPulling = false;
POINT newPos = { 0,0 };
POINT oldPos = { 0,0 };

int ul = 0;
int rd = 100;

int ox, oy, nx, ny;
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;


	switch (Message) {

	case WM_RBUTTONDOWN: {
		isPulling = true;
		ox = ORIGIN_POS.x + LOWORD(lParam);
		oy = ORIGIN_POS.y + HIWORD(lParam);
		break;
	}
	case WM_MOUSEMOVE: {
		nx = ORIGIN_POS.x + LOWORD(lParam);
		ny = ORIGIN_POS.y + HIWORD(lParam);

		if (isPulling) { //누른 상태라면 
			int xdist = nx - ox;
			int ydist = ny - oy;

			
			ORIGIN_POS.x -= xdist;
			ORIGIN_POS.y -= ydist;
			InvalidateRect(hwnd, NULL, TRUE);
			ox = nx;
			oy = ny;

			

		}
	
		break;
	}
	case WM_RBUTTONUP: {
		isPulling = false;
		nx = ORIGIN_POS.x + LOWORD(lParam);
		ny = ORIGIN_POS.y + HIWORD(lParam);
		int xdist = nx - ox;
		int ydist = ny - oy;
		/*if (xdist > 0) {
			printf("오른쪽 , ");
		}
		else {
			printf("왼쪽 , ");
		}

		if (ydist > 0) {
			printf("아래쪽 \n");
		}
		else {
			printf("위쪽 \n");
		}*/

		printf("원래 원점 %d, %d에  %d, %d 를 더한다. \n", ORIGIN_POS.x, ORIGIN_POS.y, xdist, ydist);

		ORIGIN_POS.x -= xdist;
		ORIGIN_POS.y -= ydist;
	
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	

	
	case WM_LBUTTONDOWN: {
		newPos.x = LOWORD(lParam);
		newPos.y = HIWORD(lParam);
		printf("현재 클릭한 곳은 %d, %d // 현재 화면 원점은 %d, %d \n", ORIGIN_POS.x +  newPos.x,ORIGIN_POS.y + newPos.y, ORIGIN_POS.x, ORIGIN_POS.y);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, -ORIGIN_POS.x + ul, -ORIGIN_POS.y +  ul, -ORIGIN_POS.x + rd, -ORIGIN_POS.y + rd);

		EndPaint(hwnd, &ps);

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