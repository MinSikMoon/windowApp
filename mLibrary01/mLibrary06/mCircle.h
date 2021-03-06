#pragma once
#include "mShape.h"
class mCircle : public mShape {
public:
	//1. 생성자
	mCircle() {}
	mCircle(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : mShape(_upLeftX, _upLeftY, _downRightX, _downRightY) {
		setEditorWidth((_downRightX - _upLeftX) / 10 * 8 - getDiff() * 2);
		setEditorHeight((_downRightY - _upLeftY) / 2 + getDiff());
		setEditorX(_upLeftX + (getEditorWidth() / 10) + getDiff() + 8);
		setEditorY(_upLeftY + ((_downRightY - _upLeftY) / 4) + getDiff());
	}
	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		mShape::make(_upLeftX, _upLeftY, _downRightX, _downRightY);
		setEditorWidth((_downRightX - _upLeftX) / 10 * 8 - getDiff() * 2);
		setEditorHeight((_downRightY - _upLeftY) / 2 + getDiff());
		setEditorX(_upLeftX + (getEditorWidth() / 10) + getDiff() + 8);
		setEditorY(_upLeftY + ((_downRightY - _upLeftY) / 4) + getDiff());
	}

	//3. show
	void show(HDC hdc) {
		resize(getUpLeft(), getDownRight());
		Ellipse(hdc, getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
		showText(hdc);
	}

	//4. showProgress
	void showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) {
		Ellipse(hdc, ulX, ulY, drX, drY);
	}


	//5. dot 보여주기
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
	//6. 들어왔는지 안들어왔는지 판별
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

	//7. circle resize
	void resize(POINT newUpLeft, POINT newDownRight) {
		changeXy(newUpLeft.x, newUpLeft.y, newDownRight.x, newDownRight.y);
		setEditorWidth((newDownRight.x - newUpLeft.x) / 10 * 8 - getDiff() * 2);
		setEditorHeight((newDownRight.y - newUpLeft.y) / 2 + getDiff());
		setEditorX(newUpLeft.x + (getEditorWidth() / 10) + getDiff() + 8);
		setEditorY(newUpLeft.y + ((newDownRight.y - newUpLeft.y) / 4) + getDiff());

		if (getTextHeight() > getEditorHeight()) { //글자 높이가 더 크면 조정해줘야지. 
			int midTextHeight = getTextHeight() / 2;
			int midYpos = (newDownRight.y - ((newDownRight.y - newUpLeft.y) / 2));
			int diffLineNum = ((getTextHeight() - getEditorHeight()) / getWordHeight() + 1); //몇 줄이나 튀어나갔나? 
			setEditorY(midYpos - midTextHeight + getDiff());
			setEditorHeight(getEditorHeight() + diffLineNum*getWordHeight() * 2);
		}

		printf("resize!!!!!!!!!!!!!!!!!!!!111\n");

	}
};