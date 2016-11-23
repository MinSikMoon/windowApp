#pragma once
#include "mTextSource.h"
#include "mKeyboard.h"
#include "mCaretMaster.h"


class mTextEditor {
private:
	//시작점 : 기본은 0,0 이겠지. 
	int startX;
	int startY;

	//멤버들
	size_m WORD_HEIGHT; //글자 높이
	mKeyboard keyboard; //키보드
	mTextSource textSource; //본문
	mScreenLineContainer lineContainer; //라인정보 컨테이너
	mCaretMaster caret; //캐럿
	size_m CARET_WIDTH; //캐럿의 너비


public:
	//1. 생성자
	mTextEditor() : startX(0), startY(0) {}

	mTextEditor(size_m caretWidth, size_m wordHeight) :caret(caretWidth, wordHeight), startX(0), startY(0) {
		addText(keyboard.getMstr());
	}

	void make(size_m caretWidth, size_m wordHeight) {
		startX = 0;
		startY = 0;
		caret.make(caretWidth, wordHeight);
		addText(keyboard.getMstr());
	}

	//6. 시작점 바꿔주기 
	void changeStartPoint(int x, int y) {
		startX = x;
		startY = y;
	}
	int getStartX() {
		return startX;
	}
	int getStartY() {
		return startY;
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
		lineContainer.show();
		textSource.replaceTextAt(nodeIdx, keyboard.getMstr());
		printf("replace 중 \n");
		textSource.show();
	}

	//4. 보여주기 
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos) {
		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, 16, lineContainer);
		//lineContainer.show();
	}
	void showAllText(HDC hdc, size_m screenWidth) {
		textSource.showAllText(hdc, screenWidth, startX, startY, 16, lineContainer);
		lineContainer.show();
	}
	

	//5. 캐럿 관련
	int getCaretXpixel(HDC hdc) {
		return caret.getXpixel(hdc, textSource, lineContainer) + startX;
	}
	int getCaretYpixel() {
		return caret.getYpixel() + startY;
	}

	//6. 텍스트 소스의 노드 정보
	int getTextSourceNodeNum() {
		return textSource.getTextNum();
	}

	//7. 캐럿 무빙
	void moveCaretRight() {
		
		caret.moveRight(textSource, lineContainer);
	}
	

	//디버깅
	int getFrontWordNum() {
		return caret.getFrontWordNum();
	}
};