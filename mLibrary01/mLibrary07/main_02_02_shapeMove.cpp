#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include "mMouse.h"
#include "resource3.h"
#include "mOriginPoint.h"
#include "mCircle.h"
#include "mRectangle.h"
#include "mLine.h"
#include "mShapeContainer.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

//전역 변수들
HINSTANCE g_Inst;
int g_focusedIdx = -1;
int g_orderFlag = -1;

//global 객체들
mMouse g_mouse;
mOriginPoint ORIGIN_POINT;
mShapeContainer g_msc;
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (Message) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
			g_focusedIdx = -1;
		case ID_40001: { //원
			g_orderFlag = Flag::CIRCLE;
			break;
		}
		case ID_40002: { //사각
			g_orderFlag = Flag::RECTANGLE;
			break;
		}
		case ID_40003: { //선분
			g_orderFlag = Flag::LINE;
			break;
		}

		}
		break;
	}
	case WM_RBUTTONDOWN: {
		g_mouse.setPulled(true);
		g_mouse.setOldPos(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	case WM_MOUSEMOVE: {
		g_mouse.setNewPos(LOWORD(lParam), HIWORD(lParam)); //현재 찍은 곳 절대 좌표 저장. //printf("마우스 현재 relative x,y = %d, %d \n", g_mouse.getRelativeNewPos().x, g_mouse.getRelativeNewPos().y);
														   //원점 바꾸기.
		//. 화면 풀링 로직
		if (g_mouse.getPulled()) {
			ORIGIN_POINT.move(g_mouse.getXdist(), g_mouse.getYdist());
			g_mouse.pullingAction();
			g_mouse.setOldPos(LOWORD(lParam), HIWORD(lParam));
		}

		//. 도형이동 로직
		if (g_orderFlag == Flag::MOVE) { 
			g_msc.moveAt(g_focusedIdx, g_mouse.getXdist(), g_mouse.getYdist());
			g_mouse.setOldPos(LOWORD(lParam), HIWORD(lParam));
		}
		
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_RBUTTONUP: {
		g_mouse.setPulled(false);
		break;
	}


	case WM_LBUTTONDOWN: {
		g_mouse.setNewPos(LOWORD(lParam), HIWORD(lParam)); //printf("%d, %d \n", g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY());
		g_mouse.setGrap(true);


		//. 도형 이동 로직
		if (g_focusedIdx != -1) { //도형이 선택되어 있다면..
			if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx)) {
				//1. old 포지션으로 마우스 포지션 저장 //어차피 밑에서 할 것임
				g_orderFlag = Flag::MOVE; //움직이라는 명령 장착 
			}
		}

		//. 도형 선택 로직
		if (g_orderFlag == Flag::NOTHING) {
			g_focusedIdx = g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx);
			printf("돌고난후 focused: %d \n", g_focusedIdx); //어느 도형을 찍었는지 판별해준다 //선택 잘 됨.
		}




		g_mouse.setOldPos(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	case WM_LBUTTONUP: {
		g_mouse.setGrap(false);
		g_mouse.setNewPos(LOWORD(lParam), HIWORD(lParam));
		g_focusedIdx = g_msc.buMakeShapeAction(g_orderFlag, g_focusedIdx, g_mouse);
		
		g_orderFlag = Flag::NOTHING; //선택하기 위해서 
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);






		g_msc.showAll_relative(hdc, ORIGIN_POINT.getOriginPoint());
		
		ORIGIN_POINT.show(hdc);
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
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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