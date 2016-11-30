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
#include "mZoom.h"

//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

using namespace std;

//전역 변수들
HINSTANCE g_Inst;
int g_focusedIdx = -1;
int g_orderFlag = -1;
int g_resizePoint;
//global 객체들
mMouse g_mouse;
mOriginPoint ORIGIN_POINT;
mShapeContainer g_msc;

double zoomLevel = 1.0;
XFORM xform;
HFONT g_font;

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (Message) {
	case WM_CREATE: {
		g_font = CreateFont(16, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
		
		break;
	}
	//키보드 메시지
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		if (!g_msc.isEmpty())
			g_msc.procAt(hwnd, Message, wParam, lParam, g_focusedIdx);


		break;
	}



	case WM_MOUSEWHEEL: {
		if ((short)HIWORD(wParam) > 0) {
			zoomLevel += 0.05;
		}
		else if ((short)HIWORD(wParam) < 0) {
			zoomLevel -= 0.05;
			if (zoomLevel < 1)
				zoomLevel = 1;
		}


		printf("zoom: %lf \n", zoomLevel);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
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


	case WM_MOUSEMOVE: {
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), zoomLevel); //물리-> 논리//printf("마우스 현재 relative x,y = %d, %d \n", g_mouse.getRelativeNewPos().x, g_mouse.getRelativeNewPos().y);

		if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx) && g_focusedIdx != -1) {
			SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZEALL));
		}
		else {
			SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));
		}

		if (g_mouse.getGrapped()) {
			//. 화면 풀링 로직
			if (g_mouse.getPulled()) {
				ORIGIN_POINT.move(g_mouse.getZoomXdist(zoomLevel), g_mouse.getZoomYdist(zoomLevel)); //논리 -> 물리  
				g_mouse.pullingZoomAction(zoomLevel);
				g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), zoomLevel); //물리 -> 논리

			}
			if (g_orderFlag == Flag::RESIZE) { //논리 좌표들을 변경 
				g_msc.moveResizeAction(g_focusedIdx, g_resizePoint, g_mouse);
			}

			//. 도형이동 로직
			if (g_orderFlag == Flag::MOVE) {
				g_msc.moveAt(g_focusedIdx, g_mouse.getXdist(), g_mouse.getYdist());
				g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), zoomLevel); //물리 -> 논리 
			}
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else {
			if (g_focusedIdx != -1) { //리사이즈 조절 점 장착 
				g_resizePoint = g_msc.isClosedAt(g_focusedIdx, g_mouse.getRelativeNewPos());

				if (g_resizePoint == 1 || g_resizePoint == 3) { //좌상단만 변해준다.  
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZENWSE));
				}
				else if (g_resizePoint == 2 || g_resizePoint == 4) {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZENESW));
				}
				else if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx)) {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZEALL));
				}
				else {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));
				}


			}
		}

		break;
	}
	case WM_RBUTTONDOWN: {
		g_mouse.setPulled(true);
		g_mouse.setGrap(true);
		g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), zoomLevel);

		break;
	}
	case WM_RBUTTONUP: {
		g_mouse.setPulled(false);
		g_mouse.setGrap(false);
		break;
	}


	case WM_LBUTTONDOWN: {
		//1. 1배일 때는 100,100이면 2배일 때는 50,50이 찍혀야 되니까 zoomLevel을 나눠서 넣어줘야 된다. 
		//g_mouse.setNewPos(LOWORD(lParam), HIWORD(lParam)); //
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), zoomLevel);
		g_mouse.setGrap(true);
		printf("relative new x,y : %d, %d \n", g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY()); //잘 됨. 
																									  //. 도형 이동 로직
		if (g_focusedIdx != -1) { //도형이 선택되어 있다면..
			if (g_resizePoint != -1) { //리사이즈 시작한다는 의미
				g_mouse.setTemPos1(g_msc.getUpLeftAt(g_focusedIdx));
				g_mouse.setTemPos2(g_msc.getDownRightAt(g_focusedIdx));
				g_orderFlag = Flag::RESIZE;
			}
			else {
				if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx)) {
					g_orderFlag = Flag::MOVE; //움직이라는 명령 장착 
				}
			}


		}

		//. 도형 선택 로직
		if (g_orderFlag == Flag::NOTHING) {
			g_focusedIdx = g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx); //printf("돌고난후 focused: %d \n", g_focusedIdx); //어느 도형을 찍었는지 판별해준다 //선택 잘 됨.
		}

		g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), zoomLevel);
		break;
	}

	case WM_LBUTTONUP: {
		g_mouse.setGrap(false);
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), zoomLevel);
		g_focusedIdx = g_msc.buMakeShapeAction(g_orderFlag, g_focusedIdx, g_mouse);

		g_orderFlag = Flag::NOTHING; //선택하기 위해서 
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, g_font);
		//. 현재 진행상황 실시간 보여주기
		if (g_mouse.getGrapped()) {
			g_msc.paintShowZoomProgressAction(hdc, g_orderFlag, g_mouse, ORIGIN_POINT.getOriginPoint(), zoomLevel);

		}

		xform.eM11 = zoomLevel;
		xform.eM22 = zoomLevel;
		SetGraphicsMode(hdc, GM_ADVANCED);
		SetWorldTransform(hdc, &xform);


		//g_msc.showAllExcept_relative(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint());
		g_msc.showAllExcept_Zoom(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint(), zoomLevel);
		//g_msc.showAt_relative(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint());
		g_msc.showDotAt_relative(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint());
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