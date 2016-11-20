//텍스트 에디터 클래스화 시키기
#include "mTextSource.h"
#include "mKeyboard.h"
#include "mCaretMaster.h"


#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;


//에디터 클래스만들기 테스트 01
class mTextEditor {
private:
	//멤버들
	size_m WORD_HEIGHT; //글자 높이
	mKeyboard keyboard; //키보드
	mTextSource textSource; //본문
	mScreenLineContainer lineContainer; //라인정보 컨테이너
	mCaretMaster caret; //캐럿
	size_m CARET_WIDTH; //캐럿의 너비
	

public:
	//1. 생성자
	mTextEditor(size_m caretWidth, size_m wordHeight):caret(caretWidth, wordHeight) {
		addText(keyboard.getMstr());
	}

	//메소드
	//1. 텍스트 하나 추가(문단하나 추가) => 노드 하나가 추가되는 것
	void addText(TCHAR* _inStr) {
		textSource.addText(_inStr);
	}
	void addText(const TCHAR* _inStr) {
		textSource.addText(_inStr);
	}
	void addText(mString& mStr) {
		textSource.addText(mStr);
	}
	//2. 키보드 proc
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		keyboard.mProc(hwnd, Message, wParam, lParam);
	}

	//3. 문단 내용 교체
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
	}

	//4. 보여주기 
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos) {
		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, 16, lineContainer);
		lineContainer.show();
	}
};

//전역 객체 
//테스트 엠텍스트에디터
mTextEditor textEditor(2,16);
mTextEditor textEditor2(2, 16);

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// 전역 변수들 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;

	switch (Message) {
	case WM_CREATE: {
		textEditor2.addText(TEXT("두번째 텍스트 에디터 입니다."));
		break;
	}
	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		textEditor.mProc(hwnd, Message, wParam, lParam);
		textEditor.replaceCurText(0);
		break;
	}


	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		
		textEditor2.showAllText(hdc, 100, 100, 100);
		textEditor.showAllText(hdc, rect.right, 0, 0); //맨 마지막에 호출된 녀석이 제일 TOP으로 올라가는 것을 볼 수 있다. 
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
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hbrBackground = (HBRUSH)NULL_BRUSH;
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