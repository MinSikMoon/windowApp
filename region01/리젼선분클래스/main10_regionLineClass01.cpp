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
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam); //wndproc���� ����

class mLine {
private:
	//1. ��ǥ ���
	POINT upLeft, downRight; //��� �������� �»��, ���ϴ� 
	int canvasWidth, canvasHeight; //��� �������� �ʺ�, ����
	POINT linePoint[4]; //������ ��ǥ

	//2. ������ ���
	HWND hCanvas; //������ ����� �Ǿ��� ��� Ŭ���� // �θ�� hwnd
	HRGN hLine; //�������� ����� ���� //�θ�� hCanvas

	//3. ������ Ŭ���� ��� //proc�� �Լ� �����ͷ� �ϸ� �������� ���⼭�� �׳� �������� ��ü
	WNDCLASSEX canvasWC;

	//4. ���� ����, ����, ���� ��
	int lineSize; //������ ����
	COLORREF lineColor; //������ ���� 

public:
	//1. ������
	mLine() {}
	mLine(HWND hwnd, HINSTANCE hInstance, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		//�� ��, ���� �ӽ�
		lineSize = 2;
		lineColor = RGB(255, 0, 255);

		//1. ��� ������ ��ǥ, �ʺ�, ���� ����
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;

		canvasWidth = _downRightX - _upLeftX;
		canvasHeight = _downRightY - _upLeftY;
		
		//2. ���� ��ǥ 4�� ����// ���߿��� �β��� ���������� ���� �����
		linePoint[0] = { 0, 0}; //�»�� �� ��
		linePoint[1] = { 0, lineSize};
		linePoint[2] = { canvasWidth , canvasHeight }; //���ϴ� �� �Ʒ�
		linePoint[3] = { canvasWidth , canvasHeight - lineSize };
	
		//3. ��� ������ Ŭ���� �ۼ� �� ������ֱ� 
		//1. ���⼭ child ��� Ŭ������ ��������� �ϴµ� ���� ���ο��� �������� child�� �ɱ�?
		memset(&canvasWC, 0, sizeof(canvasWC));
		canvasWC.cbSize = sizeof(WNDCLASSEX);
		canvasWC.lpszClassName = TEXT("canvasWnd"); //canvasWnd �̸����� �ۼ�
		canvasWC.lpfnWndProc = WndProc; /* This is where we will send messages to */
		canvasWC.hInstance = hInstance;
		canvasWC.hCursor = LoadCursor(NULL, IDC_HAND); //�������� �ö󰡸� �ո������ �ٲٱ� 
		canvasWC.hbrBackground = CreateSolidBrush(lineColor); //������ �������� �ϴ� �غ���. 
		canvasWC.lpfnWndProc = WndProc; //���߿��� ���Թ޵��ϵ� �ؾ� ���� ������ �ͳ�.
		RegisterClassEx(&canvasWC); //Ŭ���� ���!

		//2. canvas Ŭ���� ����
		hCanvas = CreateWindow(TEXT("canvasWnd"), NULL, WS_CHILD | WS_VISIBLE, upLeft.x, upLeft.y, downRight.x, downRight.y, hwnd, (HMENU)NULL, hInstance, NULL);
		//3. ���� ���� ����
		hLine = CreatePolygonRgn(linePoint, 4, ALTERNATE);
		SetWindowRgn(hCanvas, hLine, TRUE); //�� �߱�
	}
	void make(HWND hwnd, HINSTANCE hInstance, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		//�� ��, ���� �ӽ�
		lineSize = 3;
		lineColor = RGB(255, 0, 255);

		//1. ��� ������ ��ǥ, �ʺ�, ���� ����
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;

		canvasWidth = _downRightX - _upLeftX;
		canvasHeight = _downRightY - _upLeftY;

		//2. ���� ��ǥ 4�� ����// ���߿��� �β��� ���������� ���� �����
		linePoint[0] = { 0, 0 }; //�»�� �� ��
		linePoint[1] = { 0, lineSize };
		linePoint[2] = { canvasWidth , canvasHeight }; //���ϴ� �� �Ʒ�
		linePoint[3] = { canvasWidth , canvasHeight - lineSize };

		//3. ��� ������ Ŭ���� �ۼ� �� ������ֱ� 
		//1. ���⼭ child ��� Ŭ������ ��������� �ϴµ� ���� ���ο��� �������� child�� �ɱ�?
		memset(&canvasWC, 0, sizeof(canvasWC));
		canvasWC.cbSize = sizeof(WNDCLASSEX);
		canvasWC.lpszClassName = TEXT("canvasWnd"); //canvasWnd �̸����� �ۼ�
		canvasWC.lpfnWndProc = WndProc; /* This is where we will send messages to */
		canvasWC.hInstance = hInstance;
		canvasWC.hCursor = LoadCursor(NULL, IDC_HAND); //�������� �ö󰡸� �ո������ �ٲٱ� 
		canvasWC.hbrBackground = CreateSolidBrush(lineColor); //������ �������� �ϴ� �غ���. 
		canvasWC.lpfnWndProc = WndProc; //���߿��� ���Թ޵��ϵ� �ؾ� ���� ������ �ͳ�.
		RegisterClassEx(&canvasWC); //Ŭ���� ���!

									//2. canvas Ŭ���� ����
		hCanvas = CreateWindow(TEXT("canvasWnd"), NULL, WS_CHILD | WS_VISIBLE, upLeft.x, upLeft.y, downRight.x, downRight.y, hwnd, (HMENU)NULL, hInstance, NULL);
		//3. ���� ���� ����
		hLine = CreatePolygonRgn(linePoint, 4, ALTERNATE);
		SetWindowRgn(hCanvas, hLine, TRUE); //�� �߱�
	}

	//2. �ű��
	void move(int xDist, int yDist) {
		upLeft.x += xDist;
		upLeft.y += yDist;
		downRight.x += xDist;
		downRight.y += yDist;
		MoveWindow(hCanvas,upLeft.x, upLeft.y, downRight.x, downRight.y, TRUE);
	}

	//3. ���̱�
	void show() {
		ShowWindow(hCanvas,SW_SHOW);
	}

};


//���� ������
HINSTANCE g_Inst;
mLine m1;

//�̵� ���� ���� ����
int newX, newY; //���콺 ��ǥ 
int oldX, oldY;
BOOL isGrapped = FALSE; //�������� ���������� üũ

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
		isGrapped = TRUE; //��� 
		printf("grapped!\n");
		oldX = LOWORD(lParam); //�ϴ� ������ư ���� ��ġ ����� ���� 
		oldY = HIWORD(lParam);
		break;
	}
	case WM_RBUTTONUP: {
		isGrapped = FALSE; //�����ֱ� 
		printf("not grapped!! \n");
		break;
	}
	case WM_MOUSEMOVE: { //���콺 �����̱� 
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


					//======================�������� �ܼ� �����ֱ�======================== 
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

	//child01 class //���� ���� �ƹ��͵� ���� child class
	wc.lpszClassName = TEXT("child01");
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	//proc�� winProc�� ����.
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