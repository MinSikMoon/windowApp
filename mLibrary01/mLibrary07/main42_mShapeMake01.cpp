#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>

#include "mCircle.h"
#include "mRectangle.h"
#include "mLine.h"
#include "mShapeContainer.h"

#include "resource3.h"
#include "mMouse.h"

//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif
using namespace std;

//전역 변수들
HINSTANCE g_Inst;
int g_focusedIdx = -1;
int g_orderFlag = -1;
int g_resizePoint;
POINT g_ul;
POINT g_dr;

//전역 객체
mMouse g_mouse;
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
	case WM_RBUTTONUP: {
		g_mouse.setPulled(false);

		break;
	}
	case WM_MOUSEMOVE: {
		g_mouse.setNewPos(LOWORD(lParam), HIWORD(lParam));

		if (g_mouse.getPulled()) {
			g_mouse.pullingAction();
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		if (g_mouse.getGrapped()) {
			//printf("클릭상태로 마우스 움직임 orderflag = %d focused = %d \n", orderFlag, focusedIdx);

			if (g_focusedIdx != -1) { //선택된 도형이 있다. => 꼭짓점에 다다랐는지 아닌지 항상 체크해준다. : resizePoint를 항상 갱신: -1이면 꼭짓점 아니라는 뜻
				POINT tempPoint = g_mouse.getRelativeNewPos();
				g_resizePoint = g_msc.isClosedAt(g_focusedIdx, tempPoint);
				printf("is closed = %d \n", g_resizePoint);
			}
		
		}
		




		
	}

	

	case WM_LBUTTONDOWN: {
		g_mouse.setOldPos(LOWORD(lParam), HIWORD(lParam));
		g_mouse.setGrap(true);

		POINT tempPoint = g_mouse.getNewPos();


		printf("%d, %d \n", g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY());
		break;
	}

	case WM_LBUTTONUP: {
		g_mouse.setGrap(false);
		g_mouse.setNewPos(LOWORD(lParam), HIWORD(lParam));

		switch (g_orderFlag) {
		case Flag::CIRCLE: {
			g_msc.add(new mCircle(g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y));
			g_focusedIdx = g_msc.getShapeNum() - 1;
			break;
		}
		case Flag::RECTANGLE: {
			g_msc.add(new mRectangle(g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y));
			g_focusedIdx = g_msc.getShapeNum() - 1;
			break;
		}
		case Flag::LINE: {
			g_msc.add(new mLine(g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y));
			g_focusedIdx = g_msc.getShapeNum() - 1;
			break;
		}
		case Flag::MOVE: { //움직여 준다. 
			int tempXdist = g_mouse.getNewPos().x - g_mouse.getOldPos().x;
			int tempYdist = g_mouse.getNewPos().y - g_mouse.getOldPos().y;
			g_msc.moveAt(g_focusedIdx, tempXdist, tempYdist);
			break;
		}

		}
		printf("button up 에서 nothing으로 바꿈!!");
		g_orderFlag = Flag::NOTHING;

		InvalidateRect(hwnd, NULL, TRUE);


		break;
	}


					   //페인트
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		switch (g_orderFlag) {
		case Flag::CIRCLE: {
			mCircle().showProgress(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			break;
		}
		case Flag::RECTANGLE: {
			mRectangle().showProgress(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			break;
		}
		case Flag::LINE: {
			mLine().showProgress(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			break;
		}

		case Flag::MOVE: {
			int tempXdist, tempYdist, tempUlX, tempUlY, tempDrX, tempDrY;

			tempXdist = g_mouse.getXdist();
			tempYdist = g_mouse.getYdist();

			tempUlX = g_ul.x + tempXdist;
			tempUlY = g_ul.y + tempYdist;
			tempDrX = g_dr.x + tempXdist;
			tempDrY = g_dr.y + tempYdist;

			g_msc.showProgressAt(hdc, g_focusedIdx, tempUlX, tempUlY, tempDrX, tempDrY);
			break;
		}

		}


		g_msc.showAllExcept(hdc, g_focusedIdx); //showAll하면 showAt이 두번호출되어서 겹쳐보임. 
		g_msc.showAt(hdc, g_focusedIdx);
		g_msc.showDotAt(hdc, g_focusedIdx);
	
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