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
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam); //wndproc원형 선언

class mLine {
private:
	//1. 좌표 멤버
	POINT upLeft, downRight; //배경 윈도우의 좌상단, 우하단 
	int canvasWidth, canvasHeight; //배경 윈도우의 너비, 높이
	POINT linePoint[4]; //선분의 좌표

	//2. 윈도우 멤버
	HWND hCanvas; //리젼의 배경이 되어줄 배경 클래스 // 부모는 hwnd
	HRGN hLine; //리젼으로 만드는 선분 //부모는 hCanvas

	//3. 윈도우 클래스 멤버 //proc은 함수 포인터로 하면 좋겠지만 여기서는 그냥 원형으로 대체
	WNDCLASSEX canvasWC;

	//4. 선분 관련, 굵기, 색깔 등
	int lineSize; //라인의 굵기
	COLORREF lineColor; //라인의 색깔 

public:
	//1. 생성자
	mLine() {}
	mLine(HWND hwnd, HINSTANCE hInstance, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		//선 색, 굵기 임시
		lineSize = 2;
		lineColor = RGB(255, 0, 255);

		//1. 배경 윈도우 좌표, 너비, 높이 설정
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;

		canvasWidth = _downRightX - _upLeftX;
		canvasHeight = _downRightY - _upLeftY;
		
		//2. 선분 좌표 4개 설정// 나중에는 두께도 설정가능한 버젼 만들기
		linePoint[0] = { 0, 0}; //좌상단 맨 위
		linePoint[1] = { 0, lineSize};
		linePoint[2] = { canvasWidth , canvasHeight }; //우하단 맨 아래
		linePoint[3] = { canvasWidth , canvasHeight - lineSize };
	
		//3. 배경 윈도우 클래스 작성 및 등록해주기 
		//1. 여기서 child 라는 클래스를 생성해줘야 하는데 과연 내부에서 만들어놓은 child로 될까?
		memset(&canvasWC, 0, sizeof(canvasWC));
		canvasWC.cbSize = sizeof(WNDCLASSEX);
		canvasWC.lpszClassName = TEXT("canvasWnd"); //canvasWnd 이름으로 작성
		canvasWC.lpfnWndProc = WndProc; /* This is where we will send messages to */
		canvasWC.hInstance = hInstance;
		canvasWC.hCursor = LoadCursor(NULL, IDC_HAND); //선분위에 올라가면 손모양으로 바꾸기 
		canvasWC.hbrBackground = CreateSolidBrush(lineColor); //검정색 선분으로 일단 해본다. 
		canvasWC.lpfnWndProc = WndProc; //나중에는 주입받도록도 해야 하지 않을까 싶네.
		RegisterClassEx(&canvasWC); //클래스 등록!

		//2. canvas 클래스 생성
		hCanvas = CreateWindow(TEXT("canvasWnd"), NULL, WS_CHILD | WS_VISIBLE, upLeft.x, upLeft.y, downRight.x, downRight.y, hwnd, (HMENU)NULL, hInstance, NULL);
		//3. 리젼 선분 생성
		hLine = CreatePolygonRgn(linePoint, 4, ALTERNATE);
		SetWindowRgn(hCanvas, hLine, TRUE); //선 긋기
	}
	void make(HWND hwnd, HINSTANCE hInstance, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		//선 색, 굵기 임시
		lineSize = 3;
		lineColor = RGB(255, 0, 255);

		//1. 배경 윈도우 좌표, 너비, 높이 설정
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;

		canvasWidth = _downRightX - _upLeftX;
		canvasHeight = _downRightY - _upLeftY;

		//2. 선분 좌표 4개 설정// 나중에는 두께도 설정가능한 버젼 만들기
		linePoint[0] = { 0, 0 }; //좌상단 맨 위
		linePoint[1] = { 0, lineSize };
		linePoint[2] = { canvasWidth , canvasHeight }; //우하단 맨 아래
		linePoint[3] = { canvasWidth , canvasHeight - lineSize };

		//3. 배경 윈도우 클래스 작성 및 등록해주기 
		//1. 여기서 child 라는 클래스를 생성해줘야 하는데 과연 내부에서 만들어놓은 child로 될까?
		memset(&canvasWC, 0, sizeof(canvasWC));
		canvasWC.cbSize = sizeof(WNDCLASSEX);
		canvasWC.lpszClassName = TEXT("canvasWnd"); //canvasWnd 이름으로 작성
		canvasWC.lpfnWndProc = WndProc; /* This is where we will send messages to */
		canvasWC.hInstance = hInstance;
		canvasWC.hCursor = LoadCursor(NULL, IDC_HAND); //선분위에 올라가면 손모양으로 바꾸기 
		canvasWC.hbrBackground = CreateSolidBrush(lineColor); //검정색 선분으로 일단 해본다. 
		canvasWC.lpfnWndProc = WndProc; //나중에는 주입받도록도 해야 하지 않을까 싶네.
		RegisterClassEx(&canvasWC); //클래스 등록!

									//2. canvas 클래스 생성
		hCanvas = CreateWindow(TEXT("canvasWnd"), NULL, WS_CHILD | WS_VISIBLE, upLeft.x, upLeft.y, downRight.x, downRight.y, hwnd, (HMENU)NULL, hInstance, NULL);
		//3. 리젼 선분 생성
		hLine = CreatePolygonRgn(linePoint, 4, ALTERNATE);
		SetWindowRgn(hCanvas, hLine, TRUE); //선 긋기
	}

	//2. 옮기기
	void move(int xDist, int yDist) {
		upLeft.x += xDist;
		upLeft.y += yDist;
		downRight.x += xDist;
		downRight.y += yDist;
		MoveWindow(hCanvas,upLeft.x, upLeft.y, downRight.x, downRight.y, TRUE);
	}

	//3. 보이기
	void show() {
		ShowWindow(hCanvas,SW_SHOW);
	}

};


//전역 변수들
HINSTANCE g_Inst;
mLine m1;

//이동 관련 전역 변수
int newX, newY; //마우스 좌표 
int oldX, oldY;
BOOL isGrapped = FALSE; //잡혔는지 안잡혔는지 체크

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (Message) {
	case WM_CREATE: {
		m1.make(hwnd, g_Inst, 0, 0, 200, 200);
		break;
	}
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		m1.show();
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_RBUTTONDOWN: {
		isGrapped = TRUE; //잡기 
		printf("grapped!\n");
		oldX = LOWORD(lParam); //일단 오른버튼 잡은 위치 기억해 놓기 
		oldY = HIWORD(lParam);
		break;
	}
	case WM_RBUTTONUP: {
		isGrapped = FALSE; //놓아주기 
		printf("not grapped!! \n");
		break;
	}
	case WM_MOUSEMOVE: { //마우스 움직이기 
		if (isGrapped) {
			hdc = GetDC(hwnd);
			int tempXdist;
			int tempYdist;


			newX = LOWORD(lParam);
			newY = HIWORD(lParam);
			tempXdist = newX - oldX;
			tempYdist = newY - oldY;

			m1.move(tempXdist, tempYdist);
			InvalidateRect(hwnd, NULL, TRUE);
			oldX = newX;
			oldY = newY;

			ReleaseDC(hwnd, hdc);
		}
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