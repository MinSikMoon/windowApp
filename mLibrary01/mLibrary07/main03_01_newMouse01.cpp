#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>

#include "mMouse_new01.h"
#include "mOriginPoint.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;


class mPixel {
private:
	double onePixelCnt; //��� �ȼ��� 1�ȼ��� ĥ ���̳�?

public:
	//1. constructor
	mPixel() {
		onePixelCnt = 1.0;
	}
	
	void setPixelCnt(double n) {
		if (n < 0) {
			onePixelCnt = 1.0;
		}
		else {
			onePixelCnt = n;
		}
	}
	double getPixelCnt() {
		return onePixelCnt;
	}
	
	//1. ���� ��ǥ�� �־��� �� ȭ�鿡 ǥ���� ���� 
	//ȭ��� ����� �� 200,200������ �������δ� 100,100�϶�, cnt�� 2�� 
	int physicalToLogical(int physicalPixel) {
		return physicalPixel / onePixelCnt;
	}

	//ȭ�鿡 ������ �׷��� �Ǵµ�, �������δ� 100,100������ ȭ�鿡 �Ѹ���, �� ���������δ� 200,200�� �Ǿ�� �Ҷ�, cnt�� 2��
	int logicalToPhysical(int logicalPixel) {
		return logicalPixel*onePixelCnt;
	}


};

class mZoomMouse {
	mMouse mouse; //������ǥ�� �����ǥ�� �ٲ��� ���콺
	mPixel stdPixel; //������ �� ���� �ȼ��μ�, �ٲ��� �����ǥ�� �ٽ� �� �ȼ� ���� ����� pixel�� ��ȯ ������
public:
	//1. constructor
	mZoomMouse() {}

	//2. pixel ��������
	void setPixelCnt(double n) {
		stdPixel.setPixelCnt(n);
	}

	//3. mouse����
	//.
	void setPull(bool pull) {
		mouse.setPulled(pull);
	}
	bool isPulled() {
		return mouse.getPulled();
	}
	void setGrab(bool grab) {
		mouse.setGrap(grab);
	}
	bool isGrabbed() {
		return mouse.getGrapped();
	}

	//1. ��·�� ó�� ���콺�� ������ ���� ��������ǥ �� �� �ۿ� �����Ƿ�
	void setOldPhysicalPos(POINT oldPos) {
		mouse.setOldPos(oldPos);
	}
	void setOldPhysicalPos(int x, int y) {
		mouse.setOldPos(x, y);
	}
	void setNewPhysicalPos(POINT newPos) {
		mouse.setNewPos(newPos);
	}
	void setNewPhysicalPos(int x, int y) {
		mouse.setNewPos(x,y);
	}
	POINT getNewPhysicalPos() {
		return mouse.getNewPos();
	}
	POINT getOldPhysicalPos() {
		return mouse.getOldPos();
	}


	void setLogicalOriginPos(POINT newOriginPos) { //������ �̹� ���� �����, �ܷ����� ������ ���� ����ǥ�� ���� �Ѵ�. 
		mouse.setOriginPos(newOriginPos);
	}

	POINT getOriginPos() {
		return mouse.getOriginPos();
	}
	void setOriginPos(int x, int y) {
		mouse.setOriginPos(x, y);
	}
	//2. getRealLogical : ����� ��ǥ�� ������ zoom���͸��� ���ش�. 
	POINT getLogicalNewPos() {
		int tempRelativeX = mouse.getRelativeNewX();
		int tempRelativeY = mouse.getRelativeNewY();
		int finalX = stdPixel.physicalToLogical(tempRelativeX);
		int finalY = stdPixel.physicalToLogical(tempRelativeY);
		return{ finalX, finalY };
	}
	POINT getLogicalOldPos() {
		int tempRelativeX = mouse.getRelativeOldX();
		int tempRelativeY = mouse.getRelativeOldY();
		int finalX = stdPixel.physicalToLogical(tempRelativeX);
		int finalY = stdPixel.physicalToLogical(tempRelativeY);
		return{ finalX, finalY };
	}

	//.�׽��� : pullingAction
	void pullingAction() {
		if (isPulled()) {
			int xDist = getLogicalNewPos().x - getLogicalOldPos().x;
			int yDist = getLogicalNewPos().y - getLogicalOldPos().y;
			int newOriginX = getOriginPos().x - xDist; //������ ������ ������ ��ǥ�� ��. 
			int newOriginY = getOriginPos().y - yDist;
			setOriginPos(newOriginX, newOriginY); //���ο� �������� ���Ƴ����. 
			setOldPhysicalPos(getNewPhysicalPos());
			printf("����: %d, %d \n",getOriginPos().x, getOriginPos().y); //������ logical ��ǥ �����̴�. 
		}
	}

	//.�׽��� : logicalToPhysical
	int getPhysicalX(int physicalX) {
		//1. ������ ������ȭ ��Ų��. 
		return stdPixel.logicalToPhysical(-getOriginPos().x + physicalX);
		
	}
	int getPhysicalY(int physicalY) {
		//1. ������ ������ȭ ��Ų��. 
		return stdPixel.logicalToPhysical(-getOriginPos().y + physicalY);
	}
	int getLogicalX() {
		return stdPixel.physicalToLogical(getOriginPos().x) + getLogicalNewPos().x;
	}
	int getLogicalY() {
		return stdPixel.physicalToLogical(getOriginPos().y) + getLogicalNewPos().y;
	}
};

//���� ������
HINSTANCE g_Inst;
mMouse g_mouse;
mOriginPoint ORIGIN_POINT;
mPixel g_pixel;
mZoomMouse mouse;
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (Message) {
	case WM_CREATE: {
		mouse.setPixelCnt(3.0);

		break;
	}
	
	case WM_RBUTTONDOWN: {
		mouse.setPull(true);
		mouse.setOldPhysicalPos(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	case WM_MOUSEMOVE: {
		mouse.setNewPhysicalPos(LOWORD(lParam), HIWORD(lParam)); //���� ������ ��ǥ�� ���� �� �ִ�. 
		
		
		mouse.pullingAction();
		
		
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_RBUTTONUP: {
		mouse.setPull(false);
		
		break;
	}


	case WM_LBUTTONDOWN: {
		mouse.setNewPhysicalPos(LOWORD(lParam), HIWORD(lParam));
		printf("%d, %d \n",mouse.getLogicalX(), mouse.getLogicalY());
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		//Rectangle(hdc, -g_mouse.getOriginPos().x + 0, -g_mouse.getOriginPos().y + 0, -g_mouse.getOriginPos().x + 100, -g_mouse.getOriginPos().y + 100);
		Rectangle(hdc, mouse.getPhysicalX(0), mouse.getPhysicalY(0), mouse.getPhysicalX(100), mouse.getPhysicalY(100));
		EndPaint(hwnd, &ps);
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