#include "mHeaders.h"

#include "mString.h"
#include "mTextPixel.h"
#include "mTextSource.h"
#include "mScreenLineContainer.h"
#include "mScreenTextController.h"
#include "mCaret.h"
#include "mKeyboard.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif



mKeyboard mk1;
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static mCaret caret1;

	switch (Message) {
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		mk1.mProc(hwnd, Message, wParam, lParam);
		break;
	}

	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		//mString m1(TEXT("abcdefghijklmnopABCDEFGH가나다IJKLMNOPqrstuv1234567890ABCZQLEMONHIHIHI"));

		mScreenTextController::autoLineSwitch(hdc, mk1.getMstr(), rect.right, 0, 16);

		/*int strWidth = getMstrPixelWidthFull(hdc, m1);
		int tempCharWidth = strWidth / m1.getLength();
		int t = getScreenEndIdx(hdc, m1, rect.right, 0, tempCharWidth, 1);
		textOutCustom(hdc, m1, 0, t, 0, 0);*/

		//mScreenTextController::test();
		//caret1.show();

		//mTextSource mts1;

		//mts1.addText(TEXT("HI"));
		//mts1.addText(m1);
		//mts1.show();
		//mts1.insertTextAt(0, L"77777"); //인덱스상의 위치에 들어간다고 보면 된다. 첫문단을 0으로 봤을때 1은 두번째 문단
		//mts1.show();
		//mts1.eraseTextAt(2);
		//mts1.show();





		/*printf("====>마지막 글자 인덱스: %d \n", t);
		printf("화면의 가로 사이즈: %d \n", rect.right);
		printf("평균 문자하나 가로 길이 : %d \n", tempCharWidth);
		printf("대충 짐작한 estWordNum: %d \n", rect.right / tempCharWidth);
		printf("문자열의 가로 길이: %d \n", strWidth);*/
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