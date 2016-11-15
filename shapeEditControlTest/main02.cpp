#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include <vector>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif




using namespace std;


//���� ������
HINSTANCE g_Inst;
HWND hEdit;
//int nTop = 10;
BOOL bShow = TRUE;



class mCircle {
private:
	int upLeftX, upLeftY;
	int downRightX, downRightY; //������ ����Ʈ��Ʈ���� ���δϱ� ������ �»��, ���ϴ� ��ǥ
	int editX, editY, editWidth, editHeight; //����Ʋ ��Ʈ�� �»�� ��ǥ, �ʺ�, ����
	
	HWND hEdit; // ����Ʈ ��Ʈ�� 
	int ID_EDIT;
	
public:
	//1. ������
	mCircle() {}

	mCircle(HWND hwnd, HINSTANCE hInstance,int _ID_EDIT, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY)
		: upLeftX(_upLeftX), upLeftY(_upLeftY), downRightX(_downRightX), downRightY(_downRightY)
	{
		editWidth = (_downRightX - _upLeftX)/10 * 8;
		editHeight = (_downRightY - _upLeftY)/2;
		editX = _upLeftX + (editWidth / 10);
		editY = _upLeftY + (editHeight / 2);
		ID_EDIT = _ID_EDIT;
		//Ellipse(hdc, _upLeftX, _upLeftY, _downRightX, _downRightY);

		hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_CENTER | ES_MULTILINE | DT_WORDBREAK,
			editX, editY, editWidth, editHeight, hwnd, (HMENU)ID_EDIT, hInstance, NULL);

		SetWindowText(hEdit, TEXT("�ȳ��ϼ���.")); //�⺻ 
		ShowWindow(hEdit, SW_HIDE);
	}
	//getter setter
	int getUpLeftX() {
		return upLeftX;
	}
	int getUpLeftY() {
		return upLeftY;
	}
	int getDownRightX() {
		return downRightX;
	}
	int getDownRightY() {
		return downRightY;
	}
	int getId() {
		return ID_EDIT;
	}

	//2. �޼ҵ�
	void make(HWND hwnd, HINSTANCE hInstance, int _ID_EDIT, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY){
		upLeftX = _upLeftX;
		upLeftY = _upLeftY;
		downRightX = _downRightX;
		downRightY = _downRightY;
		ID_EDIT = _ID_EDIT;
		editWidth = (downRightX - upLeftX) / 10 * 8;
		editHeight = (downRightY - upLeftY) / 2;
		editX = upLeftX + (editWidth / 10);
		editY = upLeftY + (editHeight / 2);

		//Ellipse(hdc, upLeftX, upLeftY, downRightX, downRightY); //ellipse ������ hdc�� �ʿ��ϴ�. //show ���� ellipse�ϴ°ɷ� �ұ�?

		hEdit = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_CENTER | ES_MULTILINE | DT_WORDBREAK,
			editX, editY, editWidth, editHeight, hwnd, (HMENU)ID_EDIT, hInstance, NULL);

		SetWindowText(hEdit, TEXT("�ȳ��ϼ���.")); //�⺻ 

		ShowWindow(hEdit, SW_HIDE);
	}

	void show(HDC hdc) { //������ �� ������ ������ش�. 
		Ellipse(hdc, upLeftX, upLeftY, downRightX, downRightY); //������ �� ���� ������ش�. 
		ShowWindow(hEdit, SW_SHOW);
	}

	void makeRectangle(HDC hdc) {
		Rectangle(hdc, upLeftX, upLeftY, downRightX, downRightY); //�簢���� ������ش�. 
		Ellipse(hdc, upLeftX, upLeftY, downRightX, downRightY); //������ �� ���� ������ش�. 
	}

	void move(HWND hwnd, HINSTANCE hInstance,  int xDist, int yDist) {
		upLeftX += xDist;
		upLeftY += yDist;
		downRightX += xDist;
		downRightY += yDist;

		editWidth = (downRightX - upLeftX) / 10 * 8;
		editHeight = (downRightY - upLeftY) / 2;
		editX = upLeftX + (editWidth / 10);
		editY = upLeftY + (editHeight / 2);

		
		MoveWindow(hEdit, editX, editY, editWidth, editHeight, TRUE); //����Ʈ �����δ�. 
	}


	
};


class mCircleContainer {
private:
	vector<mCircle*> circleVector;
	int startId;
	int circleNum;

public:
	mCircleContainer(int _startId) : startId(_startId), circleNum(0){

	}

	~mCircleContainer() {
		for (vector<mCircle*>::iterator itr = circleVector.begin(); itr != circleVector.end(); itr++) {
			delete *itr;
		}
	}

	void add(HWND hwnd, HINSTANCE hInstance, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		int tempId = startId + circleVector.size();
		circleVector.push_back(new mCircle(hwnd, hInstance, tempId, _upLeftX, _upLeftY, _downRightX, _downRightY));
		circleNum++;
		printf("id: %d , circleNum: %d \n", tempId, circleNum);
	}

	void showAll(HDC hdc) {
		for (int i = 0; i < circleNum; i++) {
			mCircle* temp = circleVector[i];
			temp->show(hdc);
			
		}
	}

	void showRectangle(HDC hdc, int ID) { //�ش� ID�� CIRCLE���� �簢���� �׷��ش�. 
		int tempIdx = ID - startId;
		circleVector[tempIdx]->makeRectangle(hdc); //����Ű�� ���� �簢�� �׷��ֱ� 
	}

	void moveFocusedCircle(HWND hwnd, HINSTANCE hInst, int ID, int xDist, int yDist) {
		int tempIdx = ID - startId;
		circleVector[tempIdx]->move(hwnd, hInst, xDist, yDist);
	}
	
};
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */

mCircleContainer mc1(100);
int focusedId = -1;
BOOL isGrapped = FALSE; //�������� ���������� üũ

int newX, newY; //���콺 ��ǥ 
int oldX, oldY;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;


	switch (Message) {
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

			mc1.moveFocusedCircle(hwnd, g_Inst, focusedId, tempXdist, tempYdist);
			InvalidateRect(hwnd, NULL, TRUE);
			oldX = newX;
			oldY = newY;

			ReleaseDC(hwnd, hdc);
		}
		break;
	}
	case WM_COMMAND: {
		focusedId = LOWORD(wParam);
		printf("focusedId: %d \n", focusedId);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	
	case WM_CREATE: {
		mc1.add(hwnd, g_Inst,0, 0, 300, 300);
		mc1.add(hwnd, g_Inst, 0, 310, 300, 450);
		mc1.add(hwnd, g_Inst, 310, 0, 600, 100);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		mc1.showAll(hdc);
		if (focusedId != -1) {
			printf("focusedId: %d \n", focusedId);
			mc1.showRectangle(hdc, focusedId);
		}
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