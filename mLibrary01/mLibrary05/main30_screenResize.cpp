#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

using namespace std;

//���� ������
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
	int ulx, uly, drx, dry; //�»�� ���ϴ� ��ǥ 
	double sizeX, sizeY; //� ������ �þ ���ΰ�
	int howMuch;
	
	void sizeUp() { //n ��ŭ �»�� ���ϴ� ���� ���� 
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
	int centerX, centerY; //����� �߽����� display �ȴ�. //�»��, ���ϴ��� �ܼ��� �ʺ�� ������ �� 
	int ulx, uly, drx, dry; //�»�� ���ϴ� ��ǥ 
	double sizeX, sizeY; //� ������ �þ ���ΰ�
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
		centerX = a + tX; //���������� ���Ŵϱ�
		centerY = d - tY; //���� �ö� �Ŵϱ� 

		sizeX = (double)tX / total;
		sizeY = (double)tY / total;
	}
	void centerUp(int mouseX, int mouseY) { //���콺 ��ǥ���� �Ÿ��� �ø���. 

		int tempXdist = abs(centerX - mouseX); //���콺���� �ﰢ�� �Ÿ�
		int tempYdist = abs(centerY - mouseY);
		int total = tempXdist + tempYdist;
		double sizeX = (double)tempXdist / total; //���� ���� 
		double sizeY = (double)tempYdist / total;
		int centerYHowMuch = howMuch * sizeY * 2; //Y�� ���ŭ �������� �ϴ°�
		int centerXHowMuch = howMuch * sizeX * 2; //X�� ���ŭ �������� �ϴ°�
											
		//1. ���콺��ǥ�� ���� 4��и� �� ��� �ִ��� ����
		if (mouseX - centerX < 0) { //���콺�� �� ���ʿ� �ִ�? 1 or 4�и�
			if (mouseY - centerY < 0) { //���콺�� ���� �ִ�. -> 4��и�
				centerX += centerXHowMuch;
				centerY += centerYHowMuch;
			}
			else { //���콺�� �Ʒ��� �ִ�. 1��и�
				centerX += centerXHowMuch;
				centerY -= centerYHowMuch;
			}
		}
		else { //���콺�� �����ʿ� �ִ�. 2or 3�и� 
			if (mouseY - centerY < 0) { //3��и�
				centerX -= centerXHowMuch;
				centerY += centerYHowMuch;
			}
			else { //2��и�
				centerX -= centerXHowMuch;
				centerY -= centerYHowMuch;
			}
		}
		
	


		
	}
	void centerDown(int mouseX, int mouseY) {

		int tempXdist = abs(centerX - mouseX); //���콺���� �ﰢ�� �Ÿ�
		int tempYdist = abs(centerY - mouseY);
		int total = tempXdist + tempYdist;
		double sizeX = (double)tempXdist / total; //���� ���� 
		double sizeY = (double)tempYdist / total;
		int centerYHowMuch = howMuch * sizeY * 2; //Y�� ���ŭ �������� �ϴ°�
		int centerXHowMuch = howMuch * sizeX * 2; //X�� ���ŭ �������� �ϴ°�

												  //1. ���콺��ǥ�� ���� 4��и� �� ��� �ִ��� ����
		if (mouseX - centerX < 0) { //���콺�� �� ���ʿ� �ִ�? 1 or 4�и�
			if (mouseY - centerY < 0) { //���콺�� ���� �ִ�. -> 4��и�
				centerX -= centerXHowMuch;
				centerY -= centerYHowMuch;
			}
			else { //���콺�� �Ʒ��� �ִ�. 1��и�
				centerX -= centerXHowMuch;
				centerY += centerYHowMuch;
			}
		}
		else { //���콺�� �����ʿ� �ִ�. 2or 3�и� 
			if (mouseY - centerY < 0) { //3��и�
				centerX += centerXHowMuch;
				centerY -= centerYHowMuch;
			}
			else { //2��и�
				centerX += centerXHowMuch;
				centerY += centerYHowMuch;
			}
		}
	}

	void sizeUp(int mouseX, int mouseY) { //n ��ŭ �»�� ���ϴ� ���� ���� 
		int tempXdiff = sizeX*howMuch;
		int tempYdiff = sizeY*howMuch; //x, y�� ��� �ȼ��� ������ ���̳�.

		//1. ���� ���� ����
		centerUp(mouseX, mouseY);
		//2. �»�� ���ϴ� ���� ���� 
		int xDist = abs(drx - ulx)/2; //���ͷ� ������ �Ÿ�
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

	void sizeDown(int mouseX, int mouseY) { //n ��ŭ �»�� ���ϴ� ���� ���� 
		int tempXdiff = sizeX*howMuch;
		int tempYdiff = sizeY*howMuch; //x, y�� ��� �ȼ��� ������ ���̳�.

									   //1. ���� ���� ����
		centerDown(mouseX, mouseY);
		//2. �»�� ���ϴ� ���� ���� 
		int xDist = abs(drx - ulx) / 2; //���ͷ� ������ �Ÿ�
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