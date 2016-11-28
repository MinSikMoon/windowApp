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
	//1. ������
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


	//7. WhoisIn���� �ִ� �������� ���� �ֱٿ� ���� ������ ������ �Ǵ� ��Ȳ ���� 
	int whoIsIn(POINT mousePoint, int curFocusedIdx) {
		if (shapeNum == 0) {
			return -1; //�ƹ��͵� ���ٴ� ��
		}

		int tempIdx = -1;
		int inCnt = 0; //�� ���� ������ ��ġ���� üũ 

		for (int i = 0; i < shapeNum; i++) {
			mShape* temp = shapeVector[i];

			if (temp->isIn(mousePoint)) {
				inCnt++;
				tempIdx = i; //�̷��� �Ǹ� ���� �ֱٿ� ������ ������ �ȴ�. 
			}


		}
		if (inCnt > 1) {
			if (curFocusedIdx < tempIdx) {
				return curFocusedIdx;
			}
			else {
				return tempIdx; //�ƹ��͵� ���õ��� �ʾҴ�.
			}
		}
		else {
			return tempIdx; //�ƹ��͵� ���õ��� �ʾҴ�.
		}


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
	void showZoomedAt(HDC hdc, int idx, double zoomLevel) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];

		//�� �������� showZoomed�� ������ �Ѵ�. 
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
	//proc����
	//18. shapeMaking

};