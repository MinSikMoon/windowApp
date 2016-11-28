//텍스트 에디터 클래스화 시키기
#include "mTextEditor.h"


#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;


class mShape {
private:
	POINT upLeft, downRight; //좌상단 우하단 좌표
	POINT editUpLeft; //에디터의 좌상단 좌표
	int editWidth, editHeight; //에디터의 너비, 높이 
	mTextEditor textEditor; //도형 안에 들어갈 텍스트 에디터 

	//WNDCLASSEX canvasWC; //캔버스 등록할 용도,
	mString wndClassName;
	HWND hCanvas; //배경이 되어줄 윈도우 // 부모는 hwnd가 되겠지.
	int ID_CANVAS; //캔버스 윈도우의 ID

public:
	mShape() : textEditor(2,16) {}
	mShape(const TCHAR* _wndClassName, HWND hwnd, HINSTANCE hInstance, int _ID_CANVAS, 
		int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : textEditor(2, 16) {
		ID_CANVAS = _ID_CANVAS;
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;
		
		//editWidth, Height는 상속받은 자식들 별로 다르게 만들어 줘야겠지.  
		//사이즈 종류 초기화 다 끝나면 윈도우 만들어 주자. 
		hCanvas = CreateWindow(_wndClassName, NULL, WS_CHILD | WS_VISIBLE, 
			upLeft.x, upLeft.y, downRight.x, downRight.y, hwnd, (HMENU)ID_CANVAS, hInstance, NULL);
	}
	void make(const TCHAR* _wndClassName, HWND hwnd, HINSTANCE hInstance, int _ID_CANVAS, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		textEditor.make(2, 16);
		
		ID_CANVAS = _ID_CANVAS;
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;

		//editWidth, Height는 상속받은 자식들 별로 다르게 만들어 줘야겠지.  
		//사이즈 종류 초기화 다 끝나면 윈도우 만들어 주자. 
		hCanvas = CreateWindow(_wndClassName, NULL, WS_CHILD | WS_VISIBLE,
			upLeft.x, upLeft.y, downRight.x, downRight.y, hwnd, (HMENU)ID_CANVAS, hInstance, NULL);
	}
	~mShape() {
		DestroyWindow(hCanvas);
	}

	//SETTERS AND GETTERS
	POINT getUpLeft() {
		return upLeft;
	}
	POINT getDownRight() {
		return downRight;
	}
	void setUpLeft(POINT _upLeft) {
		upLeft = _upLeft;
	}
	void setDownRight(POINT _downRight) {
		downRight = _downRight;
	}
	
	 //에디터의 사이즈 관련 
	void setEditUpLeft(POINT _upLeft) {
		editUpLeft = _upLeft;
	}
	void setEditWidthHeight(int width, int height) {
		editWidth = width;
		editHeight = height;
	}
	POINT getEditUpLeft() {
		return editUpLeft;
	}
	int getEditWidth() {
		return editWidth;
	}
	int getEditHeight() {
		return editHeight;
	}

	//크기 및 이동
	void move(int xDist, int yDist) {
		upLeft.x += xDist;
		upLeft.y += yDist;
		downRight.x += xDist;
		downRight.y += yDist;
	};
	void resize(POINT newUpLeft, POINT newDownRight) {
		upLeft = newUpLeft;
		downRight = newDownRight;
	};

	//텍스트 관련
	void addText(TCHAR* _inStr) {
		textEditor.addText(_inStr);
	}
	void addText(const TCHAR* _inStr) {
		textEditor.addText(_inStr);
	}
	void addText(mString& mStr) {
		textEditor.addText(mStr);
	}

	void replaceTextAt(size_m nodeIdx, TCHAR* _newStr) {
		textEditor.replaceTextAt(nodeIdx, _newStr);
	}
	void replaceTextAt(size_m nodeIdx, const TCHAR* _newStr) {
		textEditor.replaceTextAt(nodeIdx, _newStr);
	}
	void replaceTextAt(size_m nodeIdx, mString& _newStr) {
		textEditor.replaceTextAt(nodeIdx, _newStr);
	}

	void replaceCurText(size_m nodeIdx) {
		textEditor.replaceCurText(nodeIdx);
	}

	/*void showAllText(HDC hdc) {
		textEditor.showAllText(hdc, editWidth, editUpLeft.x, editUpLeft.y);
	}*/
	void showAllText(HDC hdc, int a, int b, int c) {
		textEditor.showAllText(hdc, a, b, c);
	}

	//키보드 관련
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		textEditor.mProc(hwnd, Message, wParam, lParam);
	}
	//오버라이딩
	void show(HDC hdc) {}


};

//mShape를 상속받은 mRectangle를 만들어보자.
class mRectangle : public mShape {
public:
	mRectangle() {}
	mRectangle(const TCHAR* _wndClassName, HWND hwnd, HINSTANCE hInstance, int _ID_CANVAS, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY)
		: mShape(_wndClassName, hwnd, hInstance, _ID_CANVAS,
		_upLeftX, _upLeftY, _downRightX, _downRightY){
		POINT temp = {_upLeftX, _upLeftY};
		int tempWidth = _downRightX - _upLeftX;
		int tempHeight = _downRightY - _upLeftY;
		
		setEditUpLeft(temp);
		setEditWidthHeight(tempWidth, tempHeight);
	}
	void make(const TCHAR* _wndClassName, HWND hwnd, HINSTANCE hInstance, int _ID_CANVAS, int _upLeftX, int _upLeftY, int _downRightX, int _downRightY)
		 {
		mShape::make(_wndClassName, hwnd, hInstance, _ID_CANVAS,
			_upLeftX, _upLeftY, _downRightX, _downRightY);
		POINT temp = { _upLeftX, _upLeftY };
		int tempWidth = _downRightX - _upLeftX;
		int tempHeight = _downRightY - _upLeftY;

		setEditUpLeft(temp);
		setEditWidthHeight(tempWidth, tempHeight);
	}

	void show(HDC hdc) {
		//1. upleft좌표와 editwidth,height 좌표를 이용해 사각형을 그려준다. 
		Rectangle(hdc, getUpLeft().x, getUpLeft().y, getDownRight().x, getDownRight().y);
		//printf("%d %d %d %d \n", getUpLeft().x, getUpLeft().y, getEditWidth(), getEditHeight());
		//2. textEditor를 보여준다. 
		//showAllText(hdc);
		showAllText(hdc, getEditWidth(), getUpLeft().x, getUpLeft().y);
	}
};




//전역 객체 
//테스트 엠텍스트에디터
HINSTANCE g_Inst;


mRectangle mr1;  

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// 전역 변수들 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;

	switch (Message) {
	case WM_CREATE: {
		mr1.make(TEXT("child"), hwnd, g_Inst, 100, 10, 10, 200, 200);
	
		break;
	}

	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		(mr1).mProc(hwnd, Message, wParam, lParam);
		(mr1).replaceCurText(0);
		break;
	}
	
	case WM_COMMAND: {

		break;
	}

	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		(mr1).show(hdc);
	
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
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hbrBackground = (HBRUSH)LTGRAY_BRUSH;
	wc.lpszClassName = _T("WindowClass");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	//childCanvas만들기
	wc.lpszClassName = TEXT("child");
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)NULL_BRUSH;
	RegisterClassEx(&wc);

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
	wc.lpszClassName = TEXT("child");
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	RegisterClassEx(&wc);
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