#pragma once
#include "mHeaders.h"
#include "mString.h"
typedef list<TCHAR*> text;

/*==========================inner method==================================*/
class mTextSource { //�ȿ� ����Ʈ�� �־ ���� ��ü�� �ƴ´�. 
	text* textSource; //���� ��ü�� �ƴ� ����Ʈ�� ����Ű�� ������ 
	size_m textNum; //1������ � �ִ���, �� ��尡 � �ִ��� ������. �ε����� �ƴ϶�.

public:
	//GETTERS
	size_m getTextNum() { //���� ������ �� �� ���� ����
		return textNum;
	}

	text* getTextSource() { //���� ���ڿ��� ����� ����Ʈ�� �ּҰ��� ����
		return textSource;
	}

	//1. CONSTRUCTOR : �⺻ : �ؽ�Ʈ ����(���)�� 0�� �ǰ� �ϰ�. ����Ʈ�� ������ 0�ΰɷ� �ʱ�ȭ 
	mTextSource() : textNum(0) {
		textSource = new text(0); //����� 0�� ����Ʈ�� �����. 
	}

	//2. DESTRUCTOR
	~mTextSource() {
		for (text::iterator it = (*textSource).begin(); it != (*textSource).end(); it++) {
			delete (*it);
		}
		delete textSource;
	}

	//3. ���� �ϳ� �߰�
	void addText(TCHAR* _inStr) {
		(*textSource).push_back(_inStr);
		textNum++;
	}
	void addText(const TCHAR* _inStr) {
		TCHAR* tempStr = cloneStr(_inStr);
		(*textSource).push_back(tempStr);
		textNum++;
	}
	void addText(mString& mStr) {
		TCHAR* tempStr = mStr.cloneStr();
		(*textSource).push_back(tempStr);
		textNum++;
	}
	//4. insertTextAt : ���ϴ� ��� �ε����� ������ �߰��Ѵ�. //�������� �ڷ� �з���. //���ǻ���: ó���� 0�� �ƴ� 1�̴�. 
	void insertTextAt(size_m nodeIdx, TCHAR* _inStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}

		//1. ����Ʈ�� �ϳ� ����� ��� ���ڿ��� �ִ´�. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(_inStr);

		//2. iterator�� ���Ѵ�. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr�� ����Ű�� ���� splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum ����
		textNum++;
	}
	void insertTextAt(size_m nodeIdx, const TCHAR* _inStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		//0. �׳� tchar ���ڿ��� ����
		TCHAR* tempStr = cloneStr(_inStr);

		//1. ����Ʈ�� �ϳ� ����� ��� ���ڿ��� �ִ´�. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(tempStr);

		//2. iterator�� ���Ѵ�. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr�� ����Ű�� ���� splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum ����
		textNum++;
	}
	void insertTextAt(size_m nodeIdx, mString& mStr) {
		if (nodeIdx < 0 || nodeIdx > (textNum-1)) {
			printf("============================>�߸��� ���� in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		//0. �׳� tchar ���ڿ��� ����
		TCHAR* tempStr = mStr.cloneStr();

		//1. ����Ʈ�� �ϳ� ����� ��� ���ڿ��� �ִ´�. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(tempStr);

		//2. iterator�� ���Ѵ�. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr�� ����Ű�� ���� splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum ����
		textNum++;
	}
	
	//5. eraseText : �ε����� ����Ű�� ���� �����͸� �����Ѵ�. 
	void eraseTextAt(size_m nodeIdx) {
		if (nodeIdx < 0 || nodeIdx > (textNum-1)) {
			printf("============================>�߸��� ���� in eraseTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}

		//1. iterator�� ���Ѵ�. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}

		//2. erase()
		(*textSource).erase(itr);
		
		//3. ���� ���� ���̱�
		textNum--;
	}



	//debugging : show
	void show() {
		printf("cur list size : %d \n", (*textSource).size());
		printf("cur textNum : %d \n", textNum);
		for (text::iterator it = (*textSource).begin(); it != (*textSource).end(); it++) {
			_tprintf(TEXT("%ls \n"), (*it));
		}

	}
};