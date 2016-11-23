#pragma once
#include "mTextSource.h"
#include "mKeyboard.h"
#include "mCaretMaster.h"


class mTextEditor {
private:
	//������ : �⺻�� 0,0 �̰���. 
	int startX;
	int startY;

	//�����
	size_m WORD_HEIGHT; //���� ����
	mKeyboard keyboard; //Ű����
	mTextSource textSource; //����
	mScreenLineContainer lineContainer; //�������� �����̳�
	mCaretMaster caret; //ĳ��
	size_m CARET_WIDTH; //ĳ���� �ʺ�


public:
	//1. ������
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

	//6. ������ �ٲ��ֱ� 
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

	//�޼ҵ�
	//1. �ؽ�Ʈ �ϳ� �߰�(�����ϳ� �߰�) => ��� �ϳ��� �߰��Ǵ� ��
	void addText(TCHAR* _inStr) {
		textSource.addText(_inStr);
	}
	void addText(const TCHAR* _inStr) {
		textSource.addText(_inStr);
	}
	void addText(mString& mStr) {
		textSource.addText(mStr);
	}
	//2. Ű���� proc
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		keyboard.mProc(hwnd, Message, wParam, lParam);
	}

	//3. ���� ���� ��ü
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
		printf("replace �� \n");
		textSource.show();
	}

	//4. �����ֱ� 
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos) {
		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, 16, lineContainer);
		//lineContainer.show();
	}
	void showAllText(HDC hdc, size_m screenWidth) {
		textSource.showAllText(hdc, screenWidth, startX, startY, 16, lineContainer);
		lineContainer.show();
	}
	

	//5. ĳ�� ����
	int getCaretXpixel(HDC hdc) {
		return caret.getXpixel(hdc, textSource, lineContainer) + startX;
	}
	int getCaretYpixel() {
		return caret.getYpixel() + startY;
	}

	//6. �ؽ�Ʈ �ҽ��� ��� ����
	int getTextSourceNodeNum() {
		return textSource.getTextNum();
	}

	//7. ĳ�� ����
	void moveCaretRight() {
		
		caret.moveRight(textSource, lineContainer);
	}
	

	//�����
	int getFrontWordNum() {
		return caret.getFrontWordNum();
	}
};