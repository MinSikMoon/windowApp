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


class mResize {

public:
	mResize(int a, int b, int c, int d) {
		ulx = a;
		uly = b;
		drx = c;
		dry = d;
		howMuch = 3;
		int tX = abs(c - a); 
		int tY = abs(d - b);
		int total = tX + tY; 
		sizeX = (double)tX / total; 
		sizeY = (double)tY / total;
		
	}
	int ulx, uly, drx, dry; //좌상단 우하단 좌표 
	double sizeX, sizeY; //어떤 비율로 늘어날 것인가
	int howMuch;
	
	void sizeUp() { //n 만큼 좌상단 우하단 새로 지정 
		int templx = ulx - sizeX*howMuch;
		int temply = uly - sizeY*howMuch;

		int temprx = drx + sizeX*howMuch;
		int tempry = dry + sizeY*howMuch;

		ulx = templx;
		uly = temply;
		drx = temprx;
		dry = tempry;
	}

	void sizeDown() {
		int templx = ulx + sizeX*howMuch;
		int temply = uly + sizeY*howMuch;

		int temprx = drx - sizeX*howMuch;
		int tempry = dry - sizeY*howMuch;

		ulx = templx;
		uly = temply;
		drx = temprx;
		dry = tempry;
	}
	

};

class mCenterResize {
public:
	int centerX, centerY; //얘들을 중심으로 display 된다. //좌상단, 우하단은 단순한 너비와 높이일 뿐 
	int ulx, uly, drx, dry; //좌상단 우하단 좌표 
	double sizeX, sizeY; //어떤 비율로 늘어날 것인가
	int howMuch;
	
	mCenterResize(int a, int b, int c, int d) {
		ulx = a;
		uly = b;
		drx = c;
		dry = d;
		howMuch = 3;
		int tX = abs(c - a);
		int tY = abs(d - b);
		int total = tX + tY;
		centerX = a + tX; //오른쪽으로 간거니까
		centerY = d - tY; //위로 올라간 거니까 

		sizeX = (double)tX / total;
		sizeY = (double)tY / total;
	}
	void centerUp(int mouseX, int mouseY) { //마우스 좌표와의 거리를 늘린다. 

		int tempXdist = abs(centerX - mouseX); //마우스와의 삼각형 거리
		int tempYdist = abs(centerY - mouseY);
		int total = tempXdist + tempYdist;
		double sizeX = (double)tempXdist / total; //비율 조사 
		double sizeY = (double)tempYdist / total;
		int centerYHowMuch = howMuch * sizeY * 2; //Y는 몇개만큼 움직여야 하는가
		int centerXHowMuch = howMuch * sizeX * 2; //X는 몇개만큼 움직여야 하는가
											
		//1. 마우스좌표로 부터 4사분면 중 어디에 있는지 조사
		if (mouseX - centerX < 0) { //마우스가 더 왼쪽에 있다? 1 or 4분면
			if (mouseY - centerY < 0) { //마우스가 위에 있다. -> 4사분면
				centerX += centerXHowMuch;
				centerY += centerYHowMuch;
			}
			else { //마우스가 아래에 있다. 1사분면
				centerX += centerXHowMuch;
				centerY -= centerYHowMuch;
			}
		}
		else { //마우스가 오른쪽에 있다. 2or 3분면 
			if (mouseY - centerY < 0) { //3사분면
				centerX -= centerXHowMuch;
				centerY += centerYHowMuch;
			}
			else { //2사분면
				centerX -= centerXHowMuch;
				centerY -= centerYHowMuch;
			}
		}
		
	


		
	}
	void centerDown(int mouseX, int mouseY) {

		int tempXdist = abs(centerX - mouseX); //마우스와의 삼각형 거리
		int tempYdist = abs(centerY - mouseY);
		int total = tempXdist + tempYdist;
		double sizeX = (double)tempXdist / total; //비율 조사 
		double sizeY = (double)tempYdist / total;
		int centerYHowMuch = howMuch * sizeY * 2; //Y는 몇개만큼 움직여야 하는가
		int centerXHowMuch = howMuch * sizeX * 2; //X는 몇개만큼 움직여야 하는가

												  //1. 마우스좌표로 부터 4사분면 중 어디에 있는지 조사
		if (mouseX - centerX < 0) { //마우스가 더 왼쪽에 있다? 1 or 4분면
			if (mouseY - centerY < 0) { //마우스가 위에 있다. -> 4사분면
				centerX -= centerXHowMuch;
				centerY -= centerYHowMuch;
			}
			else { //마우스가 아래에 있다. 1사분면
				centerX -= centerXHowMuch;
				centerY += centerYHowMuch;
			}
		}
		else { //마우스가 오른쪽에 있다. 2or 3분면 
			if (mouseY - centerY < 0) { //3사분면
				centerX += centerXHowMuch;
				centerY -= centerYHowMuch;
			}
			else { //2사분면
				centerX += centerXHowMuch;
				centerY += centerYHowMuch;
			}
		}
	}

	void sizeUp(int mouseX, int mouseY) { //n 만큼 좌상단 우하단 새로 지정 
		int tempXdiff = sizeX*howMuch;
		int tempYdiff = sizeY*howMuch; //x, y는 몇개의 픽셀을 움직일 것이냐.

		//1. 센터 새로 지정
		centerUp(mouseX, mouseY);
		//2. 좌상단 우하단 새로 지정 
		int xDist = abs(drx - ulx)/2; //센터로 부터의 거리
		int yDist = abs(dry - uly)/2;

		ulx = centerX - xDist;
		uly = centerY - yDist;
		drx = centerX + xDist;
		dry = centerY + yDist;

		int templx = ulx - sizeX*howMuch;
		int temply = uly - sizeY*howMuch;

		int temprx = drx + sizeX*howMuch;
		int tempry = dry + sizeY*howMuch;

		ulx = templx;
		uly = temply;
		drx = temprx;
		dry = tempry;
		

	}

	void sizeDown(int mouseX, int mouseY) { //n 만큼 좌상단 우하단 새로 지정 
		int tempXdiff = sizeX*howMuch;
		int tempYdiff = sizeY*howMuch; //x, y는 몇개의 픽셀을 움직일 것이냐.

									   //1. 센터 새로 지정
		centerDown(mouseX, mouseY);
		//2. 좌상단 우하단 새로 지정 
		int xDist = abs(drx - ulx) / 2; //센터로 부터의 거리
		int yDist = abs(dry - uly) / 2;

		ulx = centerX - xDist;
		uly = centerY - yDist;
		drx = centerX + xDist;
		dry = centerY + yDist;

		int templx = ulx +sizeX*howMuch;
		int temply = uly + sizeY*howMuch;

		int temprx = drx - sizeX*howMuch;
		int tempry = dry - sizeY*howMuch;

		ulx = templx;
		uly = temply;
		drx = temprx;
		dry = tempry;


	}
	

};
mResize m1(100, 100, 150, 150);
mCenterResize m2(300, 300, 350, 350);
mCenterResize m3(100, 100, 150, 150);

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (Message) {
	case WM_CREATE: {
		

		break;
	}
	
	case WM_LBUTTONDOWN: {
	
		
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);

		m3.sizeUp(tempX, tempY);
		m2.sizeUp(tempX, tempY);
		InvalidateRect(hwnd, NULL, TRUE);
		
		break;
	}

	case WM_RBUTTONDOWN: {
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);
		m3.sizeDown(tempX, tempY);
		m2.sizeDown(tempX, tempY);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}


	case WM_PAINT : {
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, m3.ulx, m3.uly, m3.drx, m3.dry);
		Ellipse(hdc, m2.ulx, m2.uly, m2.drx, m2.dry);
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