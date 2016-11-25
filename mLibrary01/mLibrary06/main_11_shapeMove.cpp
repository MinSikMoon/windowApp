//텍스트 에디터 클래스화 시키기
#include "mTextEditor.h"
#include "resource3.h"
#include <vector>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

class mShape {
private:
	POINT upLeft, downRight; //좌상단 우하단 좌표
	mTextEditor textEditor;
	int editorWidth, editorX, editorY; //에디터의 제한 너비
	int diff = 2;

public:
	//1. 생성자 
	mShape() {
		textEditor.make(2, 16);
	}
	mShape(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		textEditor.make(2, 16);
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;
		editorWidth = _downRightX - _upLeftX;

		editorX = _upLeftX;
		editorY = _upLeftY;
	}
	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		textEditor.make(2, 16);
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;
		editorWidth = _downRightX - _upLeftX;

		editorX = _upLeftX;
		editorY = _upLeftY;
	}

	//2. 에디터 너비 조정 
	void setEditorWidth(int newEditorWidth) {
		editorWidth = newEditorWidth;
	}
	int getEditorWidth() {
		return editorWidth;
	}
	void setEditorX(int x) {
		editorX = x;
	}
	void setEditorY(int y) {
		editorY = y;
	}
	//3. set,get diff
	void setDiff(int n) {
		diff = n;
	}
	int getDiff() {
		return diff;
	}

	//get set x,y
	int getUpLeftX() {
		return upLeft.x;
	}
	int getUpLeftY() {
		return upLeft.y;
	}
	int getDownRightX() {
		return downRight.x;
	}
	int getDownRightY() {
		return downRight.y;
	}
	int getEditorX() {
		return editorX;
	}
	int getEditorY() {
		return editorY;
	}

	//4. mProc
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		textEditor.mProc(hwnd, Message, wParam, lParam);
		textEditor.replaceCurText(0);
	}

	//5. show
	virtual void show(HDC hdc) = 0; //왜 virtual 붙였는 지 알것
	//6. showText
	void showText(HDC hdc) {
		textEditor.showAllText(hdc, editorWidth, editorX, editorY);
		
	}

	//7. showProgress
	virtual void  showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) = 0;


};

class mRectangle : public mShape {
public:
	//1. 생성자
	mRectangle() {}

	mRectangle(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : mShape(_upLeftX, _upLeftY, _downRightX, _downRightY) {
		setEditorWidth(_downRightX - _upLeftX - getDiff() * 2);
		setEditorX(_upLeftX + getDiff());
		setEditorY(_upLeftY + getDiff());
	}

	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		mShape::make(_upLeftX, _upLeftY, _downRightX, _downRightY);
		setEditorWidth(_downRightX - _upLeftX - getDiff() * 2);
		setEditorX(_upLeftX + getDiff());
		setEditorY(_upLeftY + getDiff());
	}


	//3. show
	void show(HDC hdc) {
		Rectangle(hdc, getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
		//printf("%d %d %d %d \n", getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
		showText(hdc);

	}

	//4. showProgress
	void showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) {
		Rectangle(hdc, ulX, ulY, drX, drY);
	}


};

class mCircle : public mShape {
public:
	//1. 생성자
	mCircle() {}
	mCircle(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : mShape(_upLeftX, _upLeftY, _downRightX, _downRightY) {
		setEditorWidth((_downRightX - _upLeftX) / 10 * 8 - getDiff() * 2);
		setEditorX(_upLeftX + (getEditorWidth() / 10) + getDiff() + 5);
		setEditorY(_upLeftY + ((_downRightY - _upLeftY) / 4) + getDiff());
	}
	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		mShape::make(_upLeftX, _upLeftY, _downRightX, _downRightY);
		setEditorWidth((_downRightX - _upLeftX) / 10 * 8 - getDiff() * 2);
		setEditorX(_upLeftX + (getEditorWidth() / 10) + getDiff());
		setEditorY(_upLeftY + ((_downRightY - _upLeftY) / 4) + getDiff());
	}

	//3. show
	void show(HDC hdc) {
		//SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Ellipse(hdc, getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
		showText(hdc);
	}

	//4. showProgress
	void showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) {
		Ellipse(hdc, ulX, ulY, drX, drY);
	}

};

class mLine : public mShape {
public:
	//1. 생성자
	mLine() {}
	mLine(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : mShape(_upLeftX, _upLeftY, _downRightX, _downRightY) {}
	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		mShape::make(_upLeftX, _upLeftY, _downRightX, _downRightY);
	}

	//2. show
	void show(HDC hdc) {
		MoveToEx(hdc, getUpLeftX(), getUpLeftY(), NULL);
		LineTo(hdc, getDownRightX(), getDownRightY());
	}

	//4. mProc
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		return;
	}

	//4. showProgress
	void showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) {
		MoveToEx(hdc, ulX, ulY, NULL);
		LineTo(hdc, drX, drY);
	}

};

class mShapeContainer {
private:
	vector<mShape*> shapeVector;
	int shapeNum;
	int focusedIdx;

public:
	//1. 생성자
	mShapeContainer() : shapeNum(0) {}
	~mShapeContainer() {
		for (vector<mShape*>::iterator itr = shapeVector.begin(); itr != shapeVector.end(); itr++) {
			delete *itr;
		}
	}

	//2. add
	void add(mShape* _newShape) {
		shapeVector.push_back(_newShape);
		shapeNum++;
	}

	//3. showAll
	void showAll(HDC hdc) {
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				mShape* temp = shapeVector[i];
				temp->show(hdc);
			}
		}
	}

	//4. procAt
	void procAt(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam, int _idx) {
		mShape* temp = shapeVector[_idx];
		temp->mProc(hwnd, Message, wParam, lParam);
	}

	//5. getShapeNum
	int getShapeNum() {
		return shapeNum;
	}

	//6. isEmpty(){
	bool isEmpty() {
		return shapeNum == 0 ? true : false;
	}

	////7. showProgressAt
	//void showProgressAt(HDC hdc, int _idx, int ulX, int ulY, int drX, int drY) {
	//	mShape* temp = shapeVector[_idx];
	//	temp->showProgress(hdc, ulX, ulY, drX, drY);
	//}
};

class mMouse {
private:
	POINT oldPos;
	POINT newPos;
	bool grapped;

public:
	mMouse(){
		oldPos.x = 0;
		oldPos.y = 0;
		newPos.x = 0;
		newPos.y = 0;
		grapped = false;
	}

	//1. 좌상단, 우하단 알아서 뱉어주는 함수
	POINT getUpLeft() {
		int tempLeftX;
		int tempUpY;
		POINT tempPoint;

		//1. x정해주기
		if (oldPos.x < newPos.x) {
			tempLeftX = oldPos.x;
		}
		else if (oldPos.x == newPos.x) {
			tempLeftX = oldPos.x;
		}
		else {
			tempLeftX = newPos.x;
		}

		//2. y정해주기
		if (oldPos.y < newPos.y) {
			tempUpY = oldPos.y;
		}
		else if (oldPos.y == newPos.y) {
			tempUpY = oldPos.x;
		}
		else {
			tempUpY = newPos.y;
		}

		tempPoint.x = tempLeftX;
		tempPoint.y = tempUpY;

		return tempPoint;
	}
	POINT getRightDown() {
		int tempRightX;
		int tempDownY;
		POINT tempPoint;

		//1. x정해주기
		if (oldPos.x < newPos.x) {
			tempRightX = newPos.x;
		}
		else if (oldPos.x == newPos.x) {
			tempRightX = oldPos.x;
		}
		else {
			tempRightX = oldPos.x;
		}

		//2. y정해주기
		if (oldPos.y < newPos.y) {
			tempDownY = newPos.y;
		}
		else if (oldPos.y == newPos.y) {
			tempDownY = oldPos.x;
		}
		else {
			tempDownY = oldPos.y;
		}

		tempPoint.x = tempRightX;
		tempPoint.y = tempDownY;
		return tempPoint;
	}

	void setOldX(int _x) {
		oldPos.x = _x;
	}
	void setOldY(int _y) {
		oldPos.y = _y;
	}
	void setNewX(int _x) {
		newPos.x = _x;
	}
	void setNewY(int _y) {
		newPos.y = _y;
	}

	POINT getOldPos() {
		return oldPos;
	}
	POINT getNewPos() {
		return newPos;
	}
	
	void setGrap(bool grap) {
		grapped = grap;
	}
	
	bool getGrapped() {
		return grapped;
	}
	
};


//global_variables
mShapeContainer msc;
mMouse mouse;
int orderFlag = -1;

int curX, curY;

namespace Flag {
	enum Type {
		CIRCLE,
		RECTANGLE,
		LINE,
		MOVE,
		RESIZE,
		NOTHING
	};
}



//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// 전역 변수들 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;


	switch (Message) {
	
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_40001: { //원
			orderFlag = Flag::CIRCLE;
			break;
		}
		case ID_40002: { //사각
			orderFlag = Flag::RECTANGLE;
			break;
		}
		case ID_40003: { //선분
			orderFlag = Flag::LINE;
			break;
		}

		}


		break;
	}
	
	case WM_LBUTTONDOWN: {
		mouse.setGrap(true);
				
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);

		mouse.setOldX(tempX);
		mouse.setOldY(tempY);
		break;
	}

	case WM_LBUTTONUP: {
		mouse.setGrap(false);
		
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);
		
		mouse.setNewX(tempX);
		mouse.setNewY(tempY);

		switch (orderFlag) {
		case Flag::CIRCLE : {
			msc.add(new mCircle(mouse.getUpLeft().x, mouse.getUpLeft().y, mouse.getRightDown().x, mouse.getRightDown().y));
			break;
		}
		case Flag::RECTANGLE: {
			msc.add(new mRectangle(mouse.getUpLeft().x, mouse.getUpLeft().y, mouse.getRightDown().x, mouse.getRightDown().y));
			break;
		}
		case Flag::LINE: {
			msc.add(new mLine(mouse.getOldPos().x, mouse.getOldPos().y, mouse.getNewPos().x, mouse.getNewPos().y));
			break;
		}
		
		}
		
		orderFlag = Flag::NOTHING;
		
		InvalidateRect(hwnd, NULL, TRUE);


		break;
	}

	case WM_MOUSEMOVE: {
		if (mouse.getGrapped()) { //그리는 중이라면
			int tempX = LOWORD(lParam);
			int tempY = HIWORD(lParam);

			mouse.setNewX(tempX);
			mouse.setNewY(tempY);

			InvalidateRect(hwnd, NULL, TRUE);
		}

		break;
	}
	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		if(!msc.isEmpty())
			msc.procAt(hwnd, Message, wParam, lParam, msc.getShapeNum()-1);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		switch (orderFlag) {
		case Flag::CIRCLE: {
			mCircle().showProgress(hdc, mouse.getUpLeft().x, mouse.getUpLeft().y, mouse.getRightDown().x, mouse.getRightDown().y);
			break;
		}
		case Flag::RECTANGLE: {
			mRectangle().showProgress(hdc, mouse.getUpLeft().x, mouse.getUpLeft().y, mouse.getRightDown().x, mouse.getRightDown().y);
			break;
		}
		case Flag::LINE: {
			mLine().showProgress(hdc, mouse.getOldPos().x, mouse.getOldPos().y, mouse.getNewPos().x, mouse.getNewPos().y);
			break;
		}

		}

		SetBkMode(hdc, TRANSPARENT);
		msc.showAll(hdc);
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
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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