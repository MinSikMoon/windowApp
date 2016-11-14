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


//TCHAR* tempStr;
TCHAR tempStr[2]; //������ ���� ���� ��
TCHAR tempCompleteStr[2]; //�ϼ��� ���� ���� ��
bool isCompleted; //�ϼ��� ���ڰ� �ִ���. //completeStr�� ���ڰ� ����ִ���. 
bool curComposingStat;
bool prevComposingStat; //���� ���� ime��� ����, 
mString mStr;

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// ���� ������ 
	HIMC hImc; //�Է� ���ؽ�Ʈ�� �ڵ��� �޾ƿ���. 
	HDC hdc;
	PAINTSTRUCT ps;


	switch (Message) {
	
	case WM_CREATE: {
		isCompleted = FALSE;
		curComposingStat = FALSE; //���� �ܰ��� IME ������� 
		prevComposingStat = FALSE; //���� �ܰ��� IME ������� 
		break;
	}
	case WM_IME_ENDCOMPOSITION: { //�ѱ� �Է��ϴٰ� �ѿ�Ű �������� tempStr�� ����ش�. 
		prevComposingStat = curComposingStat;
		curComposingStat = FALSE;
		memset(tempStr, 0, 2); //tempStr�� �ƹ��͵� ���� �ʴ´�. 
		break;
	}
	case WM_IME_COMPOSITION: { //IME ��� 
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

	case WM_CHAR: { //mStr�� ���ڸ� �����ִ� ����
		if (wParam == VK_BACK) {
			mStr.eraseLastChar();
			prevComposingStat = curComposingStat;
			curComposingStat = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}

		if (!prevComposingStat) { //���� ���� ime�� ������� �ʾҴµ� //�׳� �ڿ� ���ٿ��ִ� ��.

			if (curComposingStat) { //����� ime�� ������� ��, tempStr�� �ٿ��ش�. 
									//printf("=> 1 \n");
				mStr.add(tempStr); //tempStr�� �����ְ� invalidate
			}
			else { //���絵 ime�� ������� �ʾҴٸ� wParam�� �ٿ��ش�. 
				   //printf("=> 2 \n");
				mStr.add((TCHAR)wParam);
			}
		}
		else { //���� ���� �ѱ� �Է� �̾��ٸ� 
			if (curComposingStat) { //���� ���� �ѱ��Է�, ���ݵ� �ѱ��Է� => �ϼ��� ���ڰ� �ִ��� �Ǵ� 
				if (isCompleted) { //�ϼ� ���ڰ� �ִٸ� 
								   //printf("=> 3 \n");
								   //�ѿ� ��ȯŰ�� ��ŵ�Ѵ�. 
					_tprintf(L"3���� �ѿ���ȯŰ : %ls \n", tempCompleteStr);
					mStr.eraseLastChar(); //���� ���� ����� 
					mStr.add(tempCompleteStr); //�ϼ��� ���� �ְ�
					isCompleted = FALSE; //�ϼ��Ȱ� ������.
					mStr.add(tempStr); //�������� ���ڵ� �ְ� 
				}
				else { //���� ���� �ѱ� �Է�, ���ݵ� �ѱ��Է� => �ϼ� ���ڴ� ���ٸ� 
					   //printf("=> 4\n");
					mStr.eraseLastChar();
					mStr.add(tempStr); //�������� ���ڸ� �ִ´�. 
				}
			}
			else { //���� ���� �ѱ� �Է��ε�, ������ �ƴҶ� 
				if (isCompleted) { //�ϼ� ���ڰ� �ִٸ� 
								   //printf("=> 5 \n");
					mStr.eraseLastChar(); //���� ���� ����� 
					mStr.add(tempCompleteStr); //�ϼ��� ���� �ְ�
					isCompleted = FALSE; //�ϼ��Ȱ� ������.
					mStr.add((TCHAR)wParam);
				}
				else { //�ϼ� ���� ������ �������� �� �־������.
					   //printf("=> 6 \n");
					mStr.eraseLastChar(); //�ѿ�Ű�� ������ �̰͵� ���� ������ 
										  //mStr.add(tempStr); //�������� ���� �ְ�
					mStr.add((TCHAR)wParam);
				}
			}
		}

		prevComposingStat = curComposingStat;
		curComposingStat = FALSE;
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		if (mStr.getLength() > 0) //���ڿ� ���̰� 0�̸� ��¾���.
			textOutCustom(hdc, mStr, 0, mStr.getLength() - 1, 0, 0);

		EndPaint(hwnd, &ps);
		printf("���� mstr�� ����: %d \n", mStr.getLength());
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