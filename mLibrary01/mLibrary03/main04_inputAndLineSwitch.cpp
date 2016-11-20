
#include <cstdio>
#include <iostream>
#include <locale>
#include <windows.h>
#include <tchar.h>
#include <imm.h> //이걸 꼭 포함시키자. 

#include "mTextSource.h"
#include "mstring.h"
#include "mKeyboard.h"
#include "mCaretMaster.h"


#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

//전역 변수들
int caretWidth = 2;
int wordHeight = 16; //전역적으로 쓰일 글자 높이

//전역 객체들
mKeyboard mk1; //1. 키보드
mTextSource textSource; //2. 본문 문단들이 달려있는 리스트 (이게 출력됨)
mScreenLineContainer screenLineContainer; //3. 화면상 보이는 줄 정보 관리 : 노드당 몇줄?
mCaretMaster caret(caretWidth, wordHeight); //4. 캐럿

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// 전역 변수들 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;

	switch (Message) {
	case WM_CREATE: {
		textSource.addText(mk1.getMstr());	//mk1의 mstr을 기본적으로 textSource에 넣어둔다.
		textSource.addText(TEXT("이것은 덧붙인 텍스트입니다. 노드1이 되겟네"));
		caret.show();
		break;
	}
	//---------------------------캐럿관련
	//1. 포커스 잃을 때
	case WM_KILLFOCUS: {
		DestroyCaret(); //포커스를 잃으면 현재 윈도우에 장착된 캐럿을 없앤다. 
		break;
	}

	//2. 포커스 받을 때 //캐럿은 이 때 만들어야 한다. 
	case WM_SETFOCUS: {
		CreateCaret(hwnd, NULL, caret.getWidth(), caret.getHeight()); //캐럿 만들기// 2번째 비트맵이 NULL이니까, 캐럿은 2, 16 폭과 높이(문자 높이)로 만들어진다. 
		ShowCaret(hwnd); //앞에서 hwnd에 캐럿을 만들었으므로, ShowCaret()하면 캐럿이 보여진다.(깜빡깜빡)
		break;
	}

	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		mk1.mProc(hwnd, Message, wParam, lParam);
		textSource.replaceTextAt(0, mk1.getMstr()); //mk1에 들어있는 mstr로 textsource를 교체한다. 
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
		SetCaretPos(caret.getXpixel(), caret.getYpixel()); //캐럿 위치 조정 후 출력
		
		//디버깅
		screenLineContainer.show();
		
		
		
		
		
		
		EndPaint(hwnd, &ps);
		break;
	}


				   //======================마지막엔 콘솔 없애주기======================== 
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