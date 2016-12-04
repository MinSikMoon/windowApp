#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>

#include "mKeyboard.h"
#include "mTextSource.h"
#include "mScreenLineContainer.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

//전역 변수들
HINSTANCE g_Inst;
class mCursor {
	size_m frontWordNum;
	size_m cNodeIdx;
	size_m cLineIdx;
	size_m cIdx; //cIdx는 방향키이동때 중요, 입력할 때는 cIdx를 믿으면 안된다. 

	int width;
	int height;
	size_m upperLineNum;

public:
	//1. constructor
	mCursor() {
		width = 2;
		height = 6;
	}
	mCursor(size_m caretWidth, size_m wordHeight) {
		width = caretWidth;
		height = wordHeight;
	}
	void make(size_m caretWidth, size_m wordHeight) {
		width = caretWidth;
		height = wordHeight;
	}

	//2. getCurNodeIdx()
	size_m getCurNodeIdx() {
		return cNodeIdx;
	}
};

class mTextEditor { //1. 키보드 //2. 텍스트 소스 //3. 라인컨테이너
private:
	mKeyboard keyboard;
	mTextSource textSource;
	mScreenLineContainer lc;
	mCursor cursor;
	POINT startUpLeftPos;

public:
	//1. constructor
	mTextEditor() {
		startUpLeftPos.x = 0;
		startUpLeftPos.y = 0;
		addText(keyboard.getMstr()); //키보드로부터 빈문자열을 하나 붙인다. 
	}
	mTextEditor(size_m caretWidth, size_m wordHeight):cursor(caretWidth, wordHeight) {
		startUpLeftPos.x = 0;
		startUpLeftPos.y = 0;
		addText(keyboard.getMstr());
	}
	
	//2. addText : 텍스트 소스에 문자열을 붙인다. : 노드가 하나 추가된다. 
	void addText(TCHAR* _inStr) {
		textSource.addText(_inStr);
	}
	void addText(const TCHAR* _inStr) {
		textSource.addText(_inStr);
	}
	void addText(mString& mStr) {
		textSource.addText(mStr);
	}

	//3. 키보드 proc : 키보드의 mstr에 문자열이 쌓이는 구조. 
	int mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		return keyboard.mProc(hwnd, Message, wParam, lParam);
	}
	
	//4. 문단 내용 교체
	void replaceTextAt(size_m nodeIdx, TCHAR* _newStr) {
		textSource.replaceTextAt(nodeIdx, _newStr);
	}
	void replaceTextAt(size_m nodeIdx, const TCHAR* _newStr) {
		textSource.replaceTextAt(nodeIdx, _newStr);
	}
	void replaceTextAt(size_m nodeIdx, mString& _newStr) {
		textSource.replaceTextAt(nodeIdx, _newStr);
	}
	void replaceCurText(size_m nodeIdx) {
		textSource.replaceTextAt(nodeIdx, keyboard.getMstr());
	} //: nodeIdx를 키보드str로 교체
	void replaceCurTextByCnode() {  //: 현재 캐럿이 소속된 노드의 문장을 키보드 str로 교체한다. 
		textSource.replaceTextAt(cursor.getCurNodeIdx(), keyboard.getMstr());
	}
	
	//5. lineContainer 정보 채워넣기
	void setLineContainerInfo(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos, double wordWidth) {
		if (screenWidth < wordWidth)
			return; //오작동 방지

		textSource.setAllLineContainerInfo(hdc, screenWidth, firstLineXpos, firstLineYpos, wordWidth, lc);
	}
	void setLineContainerInfo(HDC hdc, size_m screenWidth, double wordWidth) {
		if (screenWidth < wordWidth)
			return; //오작동 방지
		textSource.setAllLineContainerInfo(hdc, screenWidth, startUpLeftPos.x, startUpLeftPos.y, wordWidth, lc);
		lc.show();
	}
	
	//6. showAllText() : lc에 기록된 정보를 바탕으로 textOut한다. 
	/*void showAllText(HDC hdc, int firstLineXpos, int firstLineYpos) {

		for (int i = 0; i < lc.getTextSourceNodeNum(); i++) {
			textoutcu
		}
	}*/


};


mTextEditor testEditor(2,16);
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	
	
	switch (Message) {
	case WM_CREATE: {
		testEditor.addText(TEXT("첫번째 테스트"));
		testEditor.addText(TEXT(""));
		break;
	}

	
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		testEditor.setLineContainerInfo(hdc, 500, 16);

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