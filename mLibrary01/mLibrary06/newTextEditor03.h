#pragma once

#include "mTextSource.h"
#include "mKeyboard.h"
#include "newCarot02.h"


class mTextEditor {
private:
	//멤버들
	size_m WORD_HEIGHT; //글자 높이
	mKeyboard keyboard; //키보드
	mTextSource textSource; //본문
	mScreenLineContainer lineContainer; //라인정보 컨테이너
	newCarot caret; //캐럿
	
	
	//시작점 : 기본은 0,0 이겠지. 
	int startX;
	int startY;

public:
	//1. 생성자
	mTextEditor(): startX(0), startY(0) {}

	mTextEditor(size_m caretWidth, size_m wordHeight) :caret(caretWidth, wordHeight), startX(0), startY(0) {
		addText(keyboard.getMstr());
	}

	void make(size_m caretWidth, size_m wordHeight) {
		startX = 0;
		startY = 0;
		caret.make(caretWidth, wordHeight);
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
	/*void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		keyboard.mProc(hwnd, Message, wParam, lParam);
	}*/
	int mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		return keyboard.mProc(hwnd, Message, wParam, lParam);
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

	void replaceCurText() {
		textSource.replaceTextAt(caret.getCnodeIdx(), keyboard.getMstr());
	}

	//4. 보여주기 //보여주면서 lc에 모든 정보들이 담겨진다. 
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos) {
		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, 16, lineContainer);
		lineContainer.show();
	}

	//5. 시작 좌상단 좌표 바꾸기 
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
	//입력관련
	//키보드 빈문자열로 리셋해주기 
	void keyboardClean() {
		keyboard.Clean();
	}

	//캐럿관련
	void caretInput() {
		caret.input(lineContainer);
	}

	void caretBackSpace() {
		caret.backSpace(lineContainer);
	}

	void caretEnter() {
		caret.enter();
	}

	size_m getCaretXpixel(HDC hdc) {
		return caret.getXpixel(hdc, textSource, lineContainer) + startX;
	}
	size_m getCaretYpixel() {
		return caret.getYpixel(lineContainer) + startY;
	}
	size_m getCaretNode() {
		return caret.getCnodeIdx();
	}

	void getCarotInfo() {
		caret.show(lineContainer);
	}

	mString& getKeyboardStr(){
		return keyboard.getMstr();
	}
};