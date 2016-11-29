#pragma once
#include "mShape.h"
#include "mMouse.h"
#include <vector>
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

	void showAll_relative(HDC hdc, POINT originPoint) {
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				mShape* temp = shapeVector[i];
				temp->showRelative(hdc, originPoint);
			}
		}
	}

	void showAllExcept(HDC hdc, int exIdx) {
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				if (i == exIdx)
					continue;

				mShape* temp = shapeVector[i];
				temp->show(hdc);
			}
		}
	}
	void showAllExcept_relative(HDC hdc, int exIdx, POINT ORIGIN_POINT) {
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				if (i == exIdx)
					continue;

				mShape* temp = shapeVector[i];
				temp->showRelative(hdc, ORIGIN_POINT);
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


	//7. WhoisIn겹쳐 있는 도형에서 가장 최근에 넣은 도형이 나오게 되는 상황 방지 
	int whoIsIn(POINT mousePoint, int curFocusedIdx) {
		if (shapeNum == 0) {
			return -1; //아무것도 없다는 뜻
		}

		int tempIdx = -1;
		int inCnt = 0; //몇 개의 도형이 겹치는지 체크 

		for (int i = 0; i < shapeNum; i++) {
			mShape* temp = shapeVector[i];

			if (temp->isIn(mousePoint)) {
				inCnt++;
				tempIdx = i; //이렇게 되면 가장 최근에 찍은게 나오게 된다. 
			}


		}
		if (inCnt > 1) {
			if (curFocusedIdx < tempIdx) {
				return curFocusedIdx;
			}
			else {
				return tempIdx; //아무것도 선택되지 않았다.
			}
		}
		else {
			return tempIdx; //아무것도 선택되지 않았다.
		}


	}

	//8. showDotAt
	void showDotAt(HDC hdc, int idx) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];
		temp->showDot(hdc);
	}
	void showDotAt_relative(HDC hdc, int idx, POINT originPoint) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];
		temp->showDot_relative(hdc,originPoint);
	}
	//9. showAt
	void showAt(HDC hdc, int idx) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];


		temp->show(hdc);

	}
	void showAt_relative(HDC hdc, int idx, POINT ORIGIN_POINT) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];


		temp->showRelative(hdc, ORIGIN_POINT);
	}
	void showZoomedAt(HDC hdc, int idx, double zoomLevel) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];

		//각 도형마다 showZoomed를 만들어야 한다. 
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

	//12. getUpLeft DownRight PosAt
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

	//15. getUpleftAt, DownRight
	POINT getUpLeftAt(int idx) {
		mShape* temp = shapeVector[idx];
		return temp->getUpLeft();
	}
	POINT getDownRightAt(int idx) {
		mShape* temp = shapeVector[idx];
		return temp->getDownRight();
	}

	//16. increase/ decrease
	void increaseAt(int idx, int xInc, int yInc) {
		mShape* temp = shapeVector[idx];
		temp->increase(xInc, yInc);
	}
	void decreaseAt(int idx, int xDec, int yDec) {
		mShape* temp = shapeVector[idx];
		temp->decrease(xDec, yDec);
	}

	//17. multiple
	void multipleAt(int idx, double multiple) {
		mShape* temp = shapeVector[idx];
		temp->multiply(multiple);
	}
	
	//proc관련


	//18. shapeMaking
	int buMakeShapeAction(int orderFlag, int focusedIdx, mMouse& g_mouse) {
		int g_focusedIdx = focusedIdx;
		switch (orderFlag) {
		case Flag::CIRCLE: {
			add(new mCircle(g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y));
			printf("%d,%d 에서 %d, %d로 그었다. \n", g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		case Flag::RECTANGLE: {
			add(new mRectangle(g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y));
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		case Flag::LINE: { //라인이 거꾸로 나오는 문제 
			add(new mLine(g_mouse.getRelativeOldX(), g_mouse.getRelativeOldY(), g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY()));
			printf("%d,%d 에서 %d, %d로 그었다. \n", g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		}

		return g_focusedIdx;
	}

	//19. showProgress
	void paintShowProgressAction(HDC hdc, int g_orderFlag, mMouse g_mouse, POINT ORIGIN_POINT) {
		switch (g_orderFlag) {

		case Flag::CIRCLE: {
			mCircle().showProgress_relative(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y, ORIGIN_POINT);
			break;
		}
		case Flag::RECTANGLE: {
			mRectangle().showProgress_relative(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y, ORIGIN_POINT);
			break;
		}
		case Flag::LINE: {
			mLine().showProgress_relative(hdc, g_mouse.getRelativeOldX(), g_mouse.getRelativeOldY(), g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY(), ORIGIN_POINT);
			break;
		}
		}
	}





};