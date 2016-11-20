
#include <cstdio>
#include <iostream>
#include <locale>
#include <windows.h>
#include <tchar.h>
#include <imm.h> //�̰� �� ���Խ�Ű��. 

#include "mTextSource.h"
#include "mstring.h"
#include "mKeyboard.h"
#include "mCaretMaster.h"


#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

//���� ������
int caretWidth = 2;
int wordHeight = 16; //���������� ���� ���� ����

//���� ��ü��
mKeyboard mk1; //1. Ű����
mTextSource textSource; //2. ���� ���ܵ��� �޷��ִ� ����Ʈ (�̰� ��µ�)
mScreenLineContainer screenLineContainer; //3. ȭ��� ���̴� �� ���� ���� : ���� ����?
mCaretMaster caret(caretWidth, wordHeight); //4. ĳ��

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// ���� ������ 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;

	switch (Message) {
	case WM_CREATE: {
		textSource.addText(mk1.getMstr());	//mk1�� mstr�� �⺻������ textSource�� �־�д�.
		textSource.addText(TEXT("�̰��� ������ �ؽ�Ʈ�Դϴ�. ���1�� �ǰٳ�"));
		caret.show();
		break;
	}
	//---------------------------ĳ������
	//1. ��Ŀ�� ���� ��
	case WM_KILLFOCUS: {
		DestroyCaret(); //��Ŀ���� ������ ���� �����쿡 ������ ĳ���� ���ش�. 
		break;
	}

	//2. ��Ŀ�� ���� �� //ĳ���� �� �� ������ �Ѵ�. 
	case WM_SETFOCUS: {
		CreateCaret(hwnd, NULL, caret.getWidth(), caret.getHeight()); //ĳ�� �����// 2��° ��Ʈ���� NULL�̴ϱ�, ĳ���� 2, 16 ���� ����(���� ����)�� ���������. 
		ShowCaret(hwnd); //�տ��� hwnd�� ĳ���� ��������Ƿ�, ShowCaret()�ϸ� ĳ���� ��������.(��������)
		break;
	}

	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		mk1.mProc(hwnd, Message, wParam, lParam);
		textSource.replaceTextAt(0, mk1.getMstr()); //mk1�� ����ִ� mstr�� textsource�� ��ü�Ѵ�. 
		break;
	}


	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		textSource.showAllText(hdc, rect.right, 50, 100, wordHeight, screenLineContainer);
		SetCaretPos(caret.getXpixel(), caret.getYpixel()); //ĳ�� ��ġ ���� �� ���
		
		//�����
		screenLineContainer.show();
		
		
		
		
		
		
		EndPaint(hwnd, &ps);
		break;
	}


				   //======================�������� �ܼ� �����ֱ�======================== 
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}




















//-------------------------------------------------WIN MAIN-------------------------------------------------------------------
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	_wsetlocale(LC_ALL, _T("korean"));

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