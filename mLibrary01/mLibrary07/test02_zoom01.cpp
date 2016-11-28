#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include <WinUser.h>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

//전역 변수들
HINSTANCE g_Inst;
double g_zoom = 1.0;
bool g_isPulling = false;

int g_oldX, g_oldY;
int g_newX, g_newY;

int ulx = 0;
int uly = 0;
int drx = 100;
int dry = 100;

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch (Message) {
	case WM_RBUTTONDOWN: {
		g_isPulling = true; //풀링중 표시
		g_oldX = LOWORD(lParam);
		g_oldY = HIWORD(lParam); //마우스 찍은 위치 기억 //여기가 원점이 된다. 
		break;
	}

	case WM_RBUTTONUP: {
		g_isPulling = false;
		g_newX = LOWORD(lParam);
		g_newY = HIWORD(lParam); //마우스 찍은 위치 기억 //여기가 원점이 된다. 

		int xdist = g_newX - g_oldX; //어느 정도로 움직인지 파악
		int ydist = g_newY - g_oldY;

		
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	case WM_MOUSEMOVE: {
		if (g_isPulling) { //누른 상태라면 

			g_newX = LOWORD(lParam);
			g_newY = HIWORD(lParam); //마우스 찍은 위치 기억 //여기가 원점이 된다. 

			int xdist = g_newX - g_oldX; //어느 정도로 움직인지 파악
			int ydist = g_newY - g_oldY;

			ulx += xdist;
			uly+=ydist;
			drx += xdist;
			dry += ydist;

			g_oldX = g_newX;
			g_oldY = g_newY;
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		}
	}
	case WM_MOUSEWHEEL: {
		if ((short)HIWORD(wParam) > 0) {
			g_zoom += 0.2;
		}
		else if((short)HIWORD(wParam) < 0) {
			g_zoom -= 0.2;
			if (g_zoom < 1)
				g_zoom = 1;
		}


		printf("zoom: %lf \n", g_zoom);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, ulx*g_zoom, uly*g_zoom, drx*g_zoom, dry*g_zoom);
		Ellipse(hdc, (ulx+100)*g_zoom, (uly+100)*g_zoom, (drx+100)*g_zoom, (dry+100)*g_zoom);
		
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