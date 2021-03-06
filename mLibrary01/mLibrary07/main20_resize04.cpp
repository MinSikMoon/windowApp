//텍스트 에디터 클래스화 시키기
#include "mTextEditor.h"
#include "resource3.h"
#include <vector>

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;
DWORD blueColor = RGB(0, 0, 255);
DWORD yellowColor = RGB(255, 255, 224);

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
	void changeXy(int ulx, int uly, int drx, int dry) {
		upLeft.x = ulx;
		upLeft.y = uly;
		downRight.x = drx;
		downRight.y = dry;
	}
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

	//8. showDot //선택같은거 할때 점찍어서 보여주는것
	virtual void  showDot(HDC hdc) = 0;

	//9. dot쉽게 찍기
	void mSetPixel(HDC hdc, int x, int y) {
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x, y);
	}

	//10. isIn : 도형안에 마우스가 들어왔는지 판별해주는 것
	virtual bool isIn(POINT mousePosition) { return false; }

	//11. move //도형을 움직여 주자.
	void move(int xDist, int yDist) {
		upLeft.x += xDist;
		upLeft.y += yDist;
		downRight.x += xDist;
		downRight.y += yDist;

		editorX += xDist;
		editorY += yDist;
	};

	//12. 두 점간의 거리 계산해주는 함수
	static double distanceChecker(POINT a, POINT b) {
		double xDist = abs(a.x - b.x);
		double yDist = abs(a.y - b.y);
		double temp = pow(xDist, 2) + pow(yDist, 2);
		return sqrt(temp);
	}
	//13. 
	int isClosed(POINT mousePoint) {
		POINT p1, p2, p3, p4; //좌상단부터 시계방향으로 // 좌상, 우상, 우하, 좌하
		double dist = 7.0;
		p1 = { upLeft.x, upLeft.y };
		p2 = { downRight.x, upLeft.y };
		p3 = { downRight.x, downRight.y };
		p4 = { upLeft.x, downRight.y };

		if (distanceChecker(mousePoint, p1) < dist) {
			return 1;
		}
		else if (distanceChecker(mousePoint, p2) < dist) {
			return 2;
		}
		else if (distanceChecker(mousePoint, p3) < dist) {
			return 3;
		}
		else if (distanceChecker(mousePoint, p4) < dist) {
			return 4;
		}
		else {
			return -1; //근접한게 없다. 
		}
	}



	//14. resize 도형별로 edit의 너비가 달라서 다시 해줘야함. 
	virtual void resize(POINT newUpLeft, POINT newDownRight) = 0;


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

	//5. showDot
	void  showDot(HDC hdc) {
		//setpixel 말고 lineto로 만들어보자. 
		HPEN tempPen = CreatePen(PS_SOLID, 5, blueColor); //노란색
		HPEN oldPen = (HPEN)SelectObject(hdc, tempPen);
		mSetPixel(hdc, getUpLeftX(), getUpLeftY()); //좌상단
		mSetPixel(hdc, getUpLeftX(), getDownRightY()); //좌하단
		mSetPixel(hdc, getDownRightX(), getUpLeftY()); //우상단
		mSetPixel(hdc, getDownRightX(), getDownRightY()); // 우하단
		SelectObject(hdc, oldPen);
	};

	//6. isIn : 도형안에 마우스가 들어왔는지 판별해주는 것
	bool isIn(POINT mousePosition) {
		POINT p1 = { getUpLeftX(), getUpLeftY() };
		POINT p2 = { getDownRightX(), getDownRightY() };

		bool isXSatisfied = false;
		bool isYSatisfied = false;

		if (mousePosition.x >= p1.x && mousePosition.x <= p2.x) {
			isXSatisfied = true;
		}

		if (mousePosition.y >= p1.y && mousePosition.y <= p2.y) {
			isYSatisfied = true;
		}

		if (isXSatisfied && isYSatisfied) {
			return true;
		}
		else {
			return false;
		}
	}

	//resize
	void resize(POINT newUpLeft, POINT newDownRight) {
		changeXy(newUpLeft.x, newUpLeft.y, newDownRight.x, newDownRight.y);

		setEditorWidth(newDownRight.x - newUpLeft.x - getDiff() * 2);
		setEditorX(newUpLeft.x + getDiff());
		setEditorY(newUpLeft.y + getDiff());
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


	//dot 보여주기
	void  showDot(HDC hdc) {
		//setpixel 말고 lineto로 만들어보자. 
		HPEN tempPen = CreatePen(PS_SOLID, 5, blueColor); //노란색
		HPEN oldPen = (HPEN)SelectObject(hdc, tempPen);
		mSetPixel(hdc, getUpLeftX(), getUpLeftY()); //좌상단
		mSetPixel(hdc, getUpLeftX(), getDownRightY()); //좌하단
		mSetPixel(hdc, getDownRightX(), getUpLeftY()); //우상단
		mSetPixel(hdc, getDownRightX(), getDownRightY()); // 우하단
		SelectObject(hdc, oldPen);
	};

	bool isIn(POINT mousePosition) {
		POINT p1 = { getUpLeftX(), getUpLeftY() };
		POINT p2 = { getDownRightX(), getDownRightY() };

		double a = (p2.x - p1.x) / 2.0;
		double b = (p2.y - p1.y) / 2;
		double alpha = p1.x + a;
		double beta = p1.y + b;
		double lValue = (mousePosition.x - alpha)*(mousePosition.x - alpha) / (a*a)
			+
			(mousePosition.y - beta)*(mousePosition.y - beta) / (b*b);

		if (lValue <= 1) {
			return true;
		}
		else {
			return false;
		}
	}

	//resize
	void resize(POINT newUpLeft, POINT newDownRight) {
		changeXy(newUpLeft.x, newUpLeft.y, newDownRight.x, newDownRight.y);
		setEditorWidth((newDownRight.x - newUpLeft.x) / 10 * 8 - getDiff() * 2);
		setEditorX(newUpLeft.x + (getEditorWidth() / 10) + getDiff());
		setEditorY(newUpLeft.y + ((newDownRight.y - newUpLeft.y) / 4) + getDiff());

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
	void  showDot(HDC hdc) {
		//setpixel 말고 lineto로 만들어보자. 
		HPEN tempPen = CreatePen(PS_SOLID, 5, blueColor); //노란색
		HPEN oldPen = (HPEN)SelectObject(hdc, tempPen);
		mSetPixel(hdc, getUpLeftX(), getUpLeftY()); //좌상단
		mSetPixel(hdc, getDownRightX(), getDownRightY()); // 우하단
		SelectObject(hdc, oldPen);
	};

	bool isIn(POINT mousePosition) {
		POINT p1 = { getUpLeftX(), getUpLeftY() };
		POINT p2 = { getDownRightX(), getDownRightY() };
		if (p1.x == p2.x) { // l 같은 선분
			if (p1.y <= p2.y) {
				if (mousePosition.x <= p1.x + 5 &&
					mousePosition.x >= p1.x - 5 &&
					mousePosition.y >= p1.y - 5 &&
					mousePosition.y <= p2.y + 5) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				if (mousePosition.x <= p1.x + 5 &&
					mousePosition.x >= p1.x - 5 &&
					mousePosition.y <= p1.y + 5 &&
					mousePosition.y >= p2.y - 5) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else if (p1.y == p2.y) { // ㅡ 같은 선분
			if (p1.x <= p2.x) {
				if (mousePosition.y <= p1.y + 5 &&
					mousePosition.y >= p1.y - 5 &&
					mousePosition.x >= p1.x - 5 &&
					mousePosition.x <= p2.x + 5) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				if (mousePosition.y <= p1.y + 5 &&
					mousePosition.y >= p1.y - 5 &&
					mousePosition.x <= p1.x + 5 &&
					mousePosition.x >= p2.x - 5) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else { // 나머지 선분
			POINT vSP = { mousePosition.x - p1.x, mousePosition.y - p1.y };
			POINT vSE = { p2.x - p1.x,p2.y - p1.y };
			POINT vEP = { mousePosition.x - p2.x, mousePosition.y - p2.y };

			if ((vSP.x*vSE.x + vSP.y*vSE.y) * (vEP.x*vSE.x + vEP.y*vSE.y) <= 0) {
				int dSE = vSE.x*vSE.x + vSE.y*vSE.y;
				int cp = vSP.x*vSE.y - vSP.y*vSE.x;
				int fcp = (cp > 0) ? cp : cp*-1;

				if (fcp*fcp / dSE <= 5 * 5) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				int dSP = vSP.x*vSP.x + vSP.y*vSP.y;
				int dEP = vEP.x*vEP.x + vEP.y*vEP.y;

				int min_dist = (dSP > dEP) ? dEP : dSP;
				if (min_dist <= 5 * 5) {
					return true;
				}
				else {
					return false;
				}
			}

			return false;
		}


	}

	//resize
	void resize(POINT newUpLeft, POINT newDownRight) {
		changeXy(newUpLeft.x, newUpLeft.y, newDownRight.x, newDownRight.y);
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
		if (_idx == -1) {
			return;
		}
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

	//7. whoIsIn //마우스 포인터 찍은 곳에 누가 들어와있는지 idx로 가르쳐 주는 함수
	int whoIsIn(POINT mousePoint) {
		if (shapeNum == 0) {
			return -1; //아무것도 없다는 뜻
		}

		int tempIdx = -1;

		for (int i = 0; i < shapeNum; i++) {
			mShape* temp = shapeVector[i];

			if (temp->isIn(mousePoint))
				tempIdx = i;

		}
		return tempIdx; //아무것도 선택되지 않았다.
	}

	//8. showDotAt
	void showDotAt(HDC hdc, int idx) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];
		temp->showDot(hdc);
	}

	//9. showAt
	void showAt(HDC hdc, int idx) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];
		temp->show(hdc);
	}

	//10. moveAt
	void moveAt(int idx, int xDist, int yDist) {
		mShape* temp = shapeVector[idx];
		temp->move(xDist, yDist);
	}

	//11. showProgressAt
	void showProgressAt(HDC hdc, int idx, int luX, int luY, int rdX, int rdY) {
		mShape* temp = shapeVector[idx];
		temp->showProgress(hdc, luX, luY, rdX, rdY);
	}

	//12. getUpLeftPosAt
	POINT getUpLeftPosAt(int idx) {
		mShape* temp = shapeVector[idx];
		POINT tempPoint{ temp->getUpLeftX(), temp->getUpLeftY() };
		return tempPoint;
	}
	POINT getDownRightPosAt(int idx) {
		mShape* temp = shapeVector[idx];
		POINT tempPoint{ temp->getDownRightX(), temp->getDownRightY() };
		return tempPoint;
	}

	//13. isClosedAt
	int isClosedAt(int idx, POINT point) {
		mShape* temp = shapeVector[idx];
		return temp->isClosed(point);
	}

	//14. resizeAt
	void resizeAt(int idx, POINT ul, POINT rd) {
		mShape* temp = shapeVector[idx];
		temp->resize(ul, rd);
	}
};

class mMouse {
private:
	POINT oldPos;
	POINT newPos;
	bool grapped;

public:
	mMouse() {
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
int focusedIdx = -1;
int g_resizePoint;

int g_oldX, g_oldY;
int g_newX, g_newY;

POINT g_ul, g_rd;

namespace Flag {
	enum Type {
		CIRCLE,
		RECTANGLE,
		LINE,
		MOVE,
		RESIZE,
		NOTHING,
		RESIZE_READY
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

		focusedIdx = -1;

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
		POINT temp = { tempX, tempY };

		if (orderFlag == Flag::NOTHING) { //클릭했는데, 아무것도 안하는 거면 도형을 선택하는 거다. 
			focusedIdx = msc.whoIsIn(temp);  //1. 컨테이너를 돌면서 선택된 얘를 찾아준다. 
			printf("selected: %d \n", focusedIdx); //어느 도형을 찍었는지 판별해준다 
		}

		if (focusedIdx != -1) { //어떤 도형이 선택되어 있고 
			if (g_resizePoint != -1) { //리사이즈가 아니면 움직이는것 MOVING
				if (focusedIdx == msc.whoIsIn(temp)) { //현재 마우스가 그 도형위에 있는데, 거기다가 grap된 상태이면 움직인다는 뜻이다. 
					orderFlag = Flag::MOVE; //움직이는 상태로 바꿔준다. 
					g_oldX = tempX;
					g_oldY = tempY;

					g_ul = msc.getUpLeftPosAt(focusedIdx);
					g_rd = msc.getDownRightPosAt(focusedIdx);

					printf("%d를 움직이겠다. \n", focusedIdx);
					
				}
			}
			else { //리사이즈하는것
				orderFlag = Flag::RESIZE; //이제 리사이즈 한다. //마우스 움직임으로 넘어가라. 
				g_ul = msc.getUpLeftPosAt(focusedIdx);
				g_rd = msc.getDownRightPosAt(focusedIdx);
			}
		}
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
		case Flag::CIRCLE: {
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
		case Flag::MOVE: { //움직여 준다. 
			int tempXdist = mouse.getNewPos().x - mouse.getOldPos().x;
			int tempYdist = mouse.getNewPos().y - mouse.getOldPos().y;
			msc.moveAt(focusedIdx, tempXdist, tempYdist);
			break;
		}
		case Flag::RESIZE: { //리사이즈 해준다. 
			POINT tempUl, tempRd;

			if (g_resizePoint == 1) { //좌상단만 변해준다. 
				tempUl = { tempX, tempY };
				tempRd = g_rd;
			}
			else if (g_resizePoint == 2) {
				tempUl = { g_ul.x, tempY };
				tempRd = { tempX, g_rd.y };
			}
			else if (g_resizePoint == 3) { //우하단만 변경
				tempUl = g_ul;
				tempRd = { tempX, tempY };
			}
			else {
				tempUl = { tempX, g_ul.y };
				tempRd = { g_rd.x, tempY };
			}
			msc.resizeAt(focusedIdx, tempUl, tempRd); //사이즈 변경 
			break;
		}


		}

		orderFlag = Flag::NOTHING;

		InvalidateRect(hwnd, NULL, TRUE);


		break;
	}

	case WM_MOUSEMOVE: {
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);

		if (mouse.getGrapped()) { //그리는 중이라면 //
			mouse.setNewX(tempX);
			mouse.setNewY(tempY);

			g_newX = tempX;
			g_newY = tempY;

			InvalidateRect(hwnd, NULL, TRUE);
		}
		else { //마우스가 클릭된 상태는 아니다.  //테스트 //선택된 도형의 꼭짓점 근처에 갔는지 확인하는 것.
			if (focusedIdx != -1) { //어떤 도형이 선택되어 있고 
				POINT tempPoint = { tempX, tempY };
				//현재 포커스된 도형이 있는데, 지금 그 도형의 꼭짓점 근처에 마우스가 도달 했는지 체크해야 한다. 
				g_resizePoint = msc.isClosedAt(focusedIdx, tempPoint); //-1이 아니면 움직이는게 아니라 리사이즈 되어야 된다는 뜻.

			}
		}





		break;
	}

	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		if (!msc.isEmpty())
			msc.procAt(hwnd, Message, wParam, lParam, focusedIdx);
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

		case Flag::MOVE: {
			int tempXdist, tempYdist, tempUlX, tempUlY, tempDrX, tempDrY;

			tempXdist = g_newX - g_oldX;
			tempYdist = g_newY - g_oldY;

			tempUlX = g_ul.x + tempXdist;
			tempUlY = g_ul.y + tempYdist;
			tempDrX = g_rd.x + tempXdist;
			tempDrY = g_rd.y + tempYdist;

			msc.showProgressAt(hdc, focusedIdx, tempUlX, tempUlY, tempDrX, tempDrY);


			break;
		}

		}


		msc.showAll(hdc);
		msc.showAt(hdc, focusedIdx);
		msc.showDotAt(hdc, focusedIdx);
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