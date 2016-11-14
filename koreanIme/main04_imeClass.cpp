#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include <imm.h> //이걸 꼭 포함시키자. 
#include "mstring.h"
#include "mTextPixel.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;


//TCHAR* tempStr;
TCHAR tempStr[2]; //조립된 글자 들어가는 곳
TCHAR tempCompleteStr[2]; //완성된 글자 들어가는 곳
bool isCompleted; //완성된 글자가 있느냐. //completeStr에 문자가 들어있느냐. 
bool curComposingStat;
bool prevComposingStat; //이전 뤂의 ime통과 여부, 
mString mStr;

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// 전역 변수들 
	HIMC hImc; //입력 컨텍스트의 핸들을 받아오기. 
	HDC hdc;
	PAINTSTRUCT ps;


	switch (Message) {
	
	case WM_CREATE: {
		isCompleted = FALSE;
		curComposingStat = FALSE; //현재 단계의 IME 통과여부 
		prevComposingStat = FALSE; //이전 단계의 IME 통과여부 
		break;
	}
	case WM_IME_ENDCOMPOSITION: { //한글 입력하다가 한영키 눌렀을때 tempStr을 비워준다. 
		prevComposingStat = curComposingStat;
		curComposingStat = FALSE;
		memset(tempStr, 0, 2); //tempStr에 아무것도 넣지 않는다. 
		break;
	}
	case WM_IME_COMPOSITION: { //IME 통과 
		hImc = ImmGetContext(hwnd);
		memset(tempCompleteStr, 0, 2);

		curComposingStat = TRUE; //현재 IME를 통과했음을 알림.

		if (lParam & GCS_COMPSTR) { //조립중인 문자열 받아오기.
			ImmGetCompositionString(hImc, GCS_COMPSTR, tempStr, 2);
			tempStr[1] = 0;
		}

		if (lParam & GCS_RESULTSTR) { //완성된 문자열이 있다면 받아오기 
			ImmGetCompositionString(hImc, GCS_RESULTSTR, tempCompleteStr, 2);
			tempStr[1] = 0;
			isCompleted = TRUE; //현재 완성된 문자가 저장되어있음을 알려줌.
		}

		ImmReleaseContext(hwnd, hImc);
	}

	case WM_CHAR: { //mStr에 글자를 더해주는 로직
		if (wParam == VK_BACK) {
			mStr.eraseLastChar();
			prevComposingStat = curComposingStat;
			curComposingStat = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}

		if (!prevComposingStat) { //이전 뤂이 ime를 통과하지 않았는데 //그냥 뒤에 덧붙여주는 것.

			if (curComposingStat) { //현재는 ime를 통과했을 때, tempStr을 붙여준다. 
									//printf("=> 1 \n");
				mStr.add(tempStr); //tempStr을 더해주고 invalidate
			}
			else { //현재도 ime를 통과하지 않았다면 wParam을 붙여준다. 
				   //printf("=> 2 \n");
				mStr.add((TCHAR)wParam);
			}
		}
		else { //이전 뤂이 한글 입력 이었다면 
			if (curComposingStat) { //이전 뤂도 한글입력, 지금도 한글입력 => 완성된 글자가 있는지 판단 
				if (isCompleted) { //완성 글자가 있다면 
								   //printf("=> 3 \n");
								   //한영 변환키를 스킵한다. 
					_tprintf(L"3에서 한영변환키 : %ls \n", tempCompleteStr);
					mStr.eraseLastChar(); //지금 글자 지우고 
					mStr.add(tempCompleteStr); //완성된 글자 넣고
					isCompleted = FALSE; //완성된거 꺼내씀.
					mStr.add(tempStr); //조립중인 글자도 넣고 
				}
				else { //이전 뤂이 한글 입력, 지금도 한글입력 => 완성 글자는 없다면 
					   //printf("=> 4\n");
					mStr.eraseLastChar();
					mStr.add(tempStr); //조립중인 글자만 넣는다. 
				}
			}
			else { //이전 뤂이 한글 입력인데, 지금은 아닐때 
				if (isCompleted) { //완성 글자가 있다면 
								   //printf("=> 5 \n");
					mStr.eraseLastChar(); //지금 글자 지우고 
					mStr.add(tempCompleteStr); //완성된 글자 넣고
					isCompleted = FALSE; //완성된거 꺼내씀.
					mStr.add((TCHAR)wParam);
				}
				else { //완성 글자 없으면 조립중인 거 넣어줘야지.
					   //printf("=> 6 \n");
					mStr.eraseLastChar(); //한영키를 누르면 이것도 들어가기 때문에 
										  //mStr.add(tempStr); //조립중인 글자 넣고
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

		if (mStr.getLength() > 0) //문자열 길이가 0이면 출력안함.
			textOutCustom(hdc, mStr, 0, mStr.getLength() - 1, 0, 0);

		EndPaint(hwnd, &ps);
		printf("현재 mstr의 길이: %d \n", mStr.getLength());
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