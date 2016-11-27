#pragma once
#include "mShape.h"

class mRectangle : public mShape {
public:
	//1. ������
	mRectangle() {}

	mRectangle(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : mShape(_upLeftX, _upLeftY, _downRightX, _downRightY) {
		setEditorWidth(_downRightX - _upLeftX - getDiff() * 2);
		setEditorHeight(_downRightY - _upLeftY - getDiff() * 2);
		setEditorX(_upLeftX + getDiff());
		setEditorY(_upLeftY + getDiff());
	}

	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		mShape::make(_upLeftX, _upLeftY, _downRightX, _downRightY);
		setEditorWidth(_downRightX - _upLeftX - getDiff() * 2);
		setEditorHeight(_downRightY - _upLeftY - getDiff() * 2);
		setEditorX(_upLeftX + getDiff());
		setEditorY(_upLeftY + getDiff());
	}


	//3. show
	void show(HDC hdc) {
		resize(getUpLeft(), getDownRight());
		Rectangle(hdc, getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
		showText(hdc);
		//printf("%d %d %d %d \n", getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
	}

	//4. showProgress
	void showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) {
		Rectangle(hdc, ulX, ulY, drX, drY);
	}

	//5. showDot
	void  showDot(HDC hdc) {
		//setpixel ���� lineto�� ������. 
		HPEN tempPen = CreatePen(PS_SOLID, 5, blueColor); //�����
		HPEN oldPen = (HPEN)SelectObject(hdc, tempPen);
		mSetPixel(hdc, getUpLeftX(), getUpLeftY()); //�»��
		mSetPixel(hdc, getUpLeftX(), getDownRightY()); //���ϴ�
		mSetPixel(hdc, getDownRightX(), getUpLeftY()); //����
		mSetPixel(hdc, getDownRightX(), getDownRightY()); // ���ϴ�
		SelectObject(hdc, oldPen);
	};

	//6. isIn : �����ȿ� ���콺�� ���Դ��� �Ǻ����ִ� ��
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

	//7. rectangle resize
	//resize
	void resize(POINT newUpLeft, POINT newDownRight) {
		changeXy(newUpLeft.x, newUpLeft.y, newDownRight.x, newDownRight.y);

		setEditorWidth(newDownRight.x - newUpLeft.x - getDiff() * 2);
		setEditorHeight(newDownRight.y - newUpLeft.y - getDiff() * 2);
		setEditorX(newUpLeft.x + getDiff());
		setEditorY(newUpLeft.y + getDiff());

		if (getTextHeight() > getEditorHeight()) { //���� ���̰� �� ũ�� �����������. 
			int midTextHeight = getTextHeight() / 2;
			int midYpos = (newDownRight.y - ((newDownRight.y - newUpLeft.y) / 2));
			int diffLineNum = ((getTextHeight() - getEditorHeight()) / getWordHeight() + 1); //�� ���̳� Ƣ�����? 
			setEditorY(midYpos - midTextHeight + getDiff());
			setEditorHeight(getEditorHeight() + diffLineNum*getWordHeight() * 2);
		}

	}



};