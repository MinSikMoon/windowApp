#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include <list>
#include <algorithm>

//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif
using namespace std;

//전역 변수들
HINSTANCE g_Inst;
bool g_bIsDrawing;

//좌표 구조체
class mPoint {
	int x, y;
	bool bIsStart;
public:
	//1. 생성자
	mPoint(int _x, int _y, bool b) : x(_x), y(_y), bIsStart(b){}
	
	//2. getter setter
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	bool isStartPoint() {
		return bIsStart;
	}

};

//mPoint 리스트
list<mPoint*> pointList;

//임시 POINT
int drawingX, drawingY;
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//PROC 멤버들
	HDC hdc;
	PAINTSTRUCT ps;
	
	
	
	switch (Message) {
	case WM_LBUTTONDOWN: {
		g_bIsDrawing = true; //그리기 시작 : 클릭한 순간부터 
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);

		drawingX = tempX;
		drawingY = tempY;
		pointList.push_back(new mPoint(tempX, tempY, true)); //시작점 mPoint 기록
		break;
	}

	case WM_MOUSEMOVE: {
		hdc = GetDC(hwnd);
		if (g_bIsDrawing) { //그리는 중이라면 
			int tempX = LOWORD(lParam);
			int tempY = HIWORD(lParam);
			pointList.push_back(new mPoint(tempX, tempY, false)); //시작점이 아닌 mPoint 기록

			MoveToEx(hdc, drawingX, drawingY, NULL);
			LineTo(hdc, tempX, tempY);
			drawingX = tempX;
			drawingY = tempY;
		}
		ReleaseDC(hwnd, hdc);
		break;
	}

	case WM_LBUTTONUP: {
		g_bIsDrawing = false; //그리기 끝
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		
		for (list<mPoint*>::iterator itr = pointList.begin(); itr != pointList.end(); itr++) {
			mPoint* temp = *itr;
			if ((*temp).isStartPoint()) {
				MoveToEx(hdc, (*temp).getX(), (*temp).getY(), NULL); //CP를 옮겨놓는다. 이제 여기서 부터 그려나가기 시작한다. 
				
			}
			else {
				LineTo(hdc, (*temp).getX(), (*temp).getY());
			}
		}


		EndPaint(hwnd, &ps);
		break;
	}






	//======================마지막엔 콘솔 없애주기======================== 
	case WM_DESTROY: {
		for (list<mPoint*>::iterator itr = pointList.begin(); itr != pointList.end(); itr++) {
			delete (*itr);
		}

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
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);

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