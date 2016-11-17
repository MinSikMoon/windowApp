#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include <imm.h> //�̰� �� ���Խ�Ű��. 
#include "mstring.h"
#include "mTextPixel.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

class mKeyboard {
private:
	mString mStr; //���� ȭ�鿡 ������� ���ڿ�
	TCHAR tempStr[2]; //�������� �ѱ��� ���� ��
	TCHAR tempCompleteStr[2]; //�ϼ��� ���� ���� ��
	bool isCompleted; //�ϼ��� ���ڰ� �ִ���. //completeStr�� ���ڰ� ����ִ���. 
	bool curComposingStat;
	bool prevComposingStat; //���� ���� ime��� ����, 

	HIMC hImc;

	bool isProced;

public:
	//1. ������
	mKeyboard() {
		printf("mk1 �ʱ�ȭ \n");
		isCompleted = FALSE;
		curComposingStat = FALSE; //���� �ܰ��� IME ������� 
		prevComposingStat = FALSE; //���� �ܰ��� IME ������� 
	}

	//�޼ҵ� 1. ime endcomposition:
	void endComposition() {
		prevComposingStat = curComposingStat;
		curComposingStat = FALSE;
		memset(tempStr, 0, 2); //tempStr�� �ƹ��͵� ���� �ʴ´�. 
	}

	//�޼ҵ� 2. ime composition:
	void composition(HIMC hImc, HWND hwnd, LPARAM lParam) {
		hImc = ImmGetContext(hwnd);
		memset(tempCompleteStr, 0, 2);

		curComposingStat = TRUE; //���� IME�� ��������� �˸�.

		if (lParam & GCS_COMPSTR) { //�������� ���ڿ� �޾ƿ���.
			ImmGetCompositionString(hImc, GCS_COMPSTR, tempStr, 2);
			tempStr[1] = 0;
		}

		if (lParam & GCS_RESULTSTR) { //�ϼ��� ���ڿ��� �ִٸ� �޾ƿ��� 
			ImmGetCompositionString(hImc, GCS_RESULTSTR, tempCompleteStr, 2);
			tempStr[1] = 0;
			isCompleted = TRUE; //���� �ϼ��� ���ڰ� ����Ǿ������� �˷���.
		}

		ImmReleaseContext(hwnd, hImc);
	}

	//�޼ҵ� 3. char
	void makeChar(HWND hwnd, WPARAM wParam) {
		if (wParam == VK_BACK) {
			mStr.eraseLastChar();
			prevComposingStat = curComposingStat;
			curComposingStat = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
			return;
		}

		if (!prevComposingStat) { //���� ���� ime�� ������� �ʾҴµ� //�׳� �ڿ� ���ٿ��ִ� ��.

			if (curComposingStat) { //����� ime�� ������� ��, tempStr�� �ٿ��ش�. 
			
				mStr.add(tempStr); //tempStr�� �����ְ� invalidate
			}
			else { //���絵 ime�� ������� �ʾҴٸ� wParam�� �ٿ��ش�. 
			
				mStr.add((TCHAR)wParam);
			}
		}
		else { //���� ���� �ѱ� �Է� �̾��ٸ� 

			if (curComposingStat) { //���� ���� �ѱ��Է�, ���ݵ� �ѱ��Է� => �ϼ��� ���ڰ� �ִ��� �Ǵ� 
				if (isCompleted) { //�ϼ� ���ڰ� �ִٸ� 
				
					//�ѿ� ��ȯŰ�� ��ŵ�Ѵ�. 
					_tprintf(L"3���� �ѿ���ȯŰ : %ls \n", tempCompleteStr);
					mStr.eraseLastChar(); //���� ���� ����� 
					mStr.add(tempCompleteStr); //�ϼ��� ���� �ְ�
					isCompleted = FALSE; //�ϼ��Ȱ� ������.
					mStr.add(tempStr); //�������� ���ڵ� �ְ� 

				}
				else { //���� ���� �ѱ� �Է�, ���ݵ� �ѱ��Է� => �ϼ� ���ڴ� ���ٸ� 
				
					mStr.eraseLastChar();
					mStr.add(tempStr); //�������� ���ڸ� �ִ´�. 
				}
			}
			else { //���� ���� �ѱ� �Է��ε�, ������ �ƴҶ� 
				if (isCompleted) { //�ϼ� ���ڰ� �ִٸ� 
				
					mStr.eraseLastChar(); //���� ���� ����� 
					mStr.add(tempCompleteStr); //�ϼ��� ���� �ְ�
					isCompleted = FALSE; //�ϼ��Ȱ� ������.
					mStr.add((TCHAR)wParam);
				}
				else { //�ϼ� ���� ������ �������� �� �־������.
				
					mStr.eraseLastChar(); //�ѿ�Ű�� ������ �̰͵� ���� ������ 
										  //mStr.add(tempStr); //�������� ���� �ְ�
					mStr.add((TCHAR)wParam);
				}
			}
		}

		prevComposingStat = curComposingStat;
		curComposingStat = FALSE;
		//printf("char ��\n");
		InvalidateRect(hwnd, NULL, TRUE);
	}
	//testing
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		switch (Message) {
		case WM_IME_ENDCOMPOSITION: { //�ѱ� �Է��ϴٰ� �ѿ�Ű �������� tempStr�� ����ش�. 
			printf("endcomposition \n");
			endComposition();
			break;
		}
		case WM_IME_COMPOSITION: { //IME ��� 
			printf("compsitioning \n");
			hImc = ImmGetContext(hwnd);
			composition(hImc, hwnd, lParam);
			ImmReleaseContext(hwnd, hImc);
		}

		case WM_CHAR: { //mStr�� ���ڸ� �����ִ� ����
			printf("char \n");
			makeChar(hwnd, wParam);
			break;
		}
		}
	}

	//3. GETTER
	int getMstrLength() {
		return mStr.getLength();
	}

	mString& getMstr() {
		return mStr;
	}


};


mKeyboard mk1;
//bool isProced = FALSE;
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// ���� ������ 
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch (Message) {
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		mk1.mProc(hwnd, Message, wParam, lParam);
		break;
	}
	
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		if (mk1.getMstrLength() > 0) //���ڿ� ���̰� 0�̸� ��¾���.
			textOutCustom(hdc, mk1.getMstr(), 0, mk1.getMstrLength() - 1, 0, 0);
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