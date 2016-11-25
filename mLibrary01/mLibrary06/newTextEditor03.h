#pragma once

#include "mTextSource.h"
#include "mKeyboard.h"
#include "newCarot02.h"


class mTextEditor {
private:
	//�����
	size_m WORD_HEIGHT; //���� ����
	mKeyboard keyboard; //Ű����
	mTextSource textSource; //����
	mScreenLineContainer lineContainer; //�������� �����̳�
	newCarot caret; //ĳ��
	
	
	//������ : �⺻�� 0,0 �̰���. 
	int startX;
	int startY;

public:
	//1. ������
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
	/*void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		keyboard.mProc(hwnd, Message, wParam, lParam);
	}*/
	int mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		return keyboard.mProc(hwnd, Message, wParam, lParam);
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
		textSource.replaceTextAt(nodeIdx, keyboard.getMstr());
	}

	void replaceCurText() {
		textSource.replaceTextAt(caret.getCnodeIdx(), keyboard.getMstr());
	}

	//4. �����ֱ� //�����ָ鼭 lc�� ��� �������� �������. 
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos) {
		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, 16, lineContainer);
		lineContainer.show();
	}

	//5. ���� �»�� ��ǥ �ٲٱ� 
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
	//�Է°���
	//Ű���� ���ڿ��� �������ֱ� 
	void keyboardClean() {
		keyboard.Clean();
	}

	//ĳ������
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