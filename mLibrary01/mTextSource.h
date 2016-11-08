#pragma once
#include "mHeaders.h"
#include "mString.h"
typedef list<TCHAR*> text;

/*==========================inner method==================================*/
class mTextSource { //안에 리스트가 있어서 본문 자체를 싣는다. 
	text* textSource; //본문 자체를 싣는 리스트를 가리키는 포인터 
	size_m textNum; //1문단이 몇개 있는지, 즉 노드가 몇개 있는지 갯수다. 인덱스가 아니라.

public:
	//GETTERS
	size_m getTextNum() { //현재 문단이 몇 개 인지 리턴
		return textNum;
	}

	text* getTextSource() { //현재 문자열이 저장된 리스트의 주소값을 리턴
		return textSource;
	}

	//1. CONSTRUCTOR : 기본 : 텍스트 숫자(노드)가 0이 되게 하고. 리스트도 사이즈 0인걸로 초기화 
	mTextSource() : textNum(0) {
		textSource = new text(0); //사이즈가 0인 리스트를 만든다. 
	}

	//2. DESTRUCTOR
	~mTextSource() {
		for (text::iterator it = (*textSource).begin(); it != (*textSource).end(); it++) {
			delete (*it);
		}
		delete textSource;
	}

	//3. 문단 하나 추가
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
	//4. insertTextAt : 원하는 노드 인덱스에 문단을 추가한다. //나머지는 뒤로 밀려남. //주의사항: 처음이 0이 아닌 1이다. 
	void insertTextAt(size_m nodeIdx, TCHAR* _inStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}

		//1. 리스트를 하나 만들고 대상 문자열을 넣는다. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(_inStr);

		//2. iterator를 구한다. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr이 가리키는 곳에 splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum 갱신
		textNum++;
	}
	void insertTextAt(size_m nodeIdx, const TCHAR* _inStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		//0. 그냥 tchar 문자열로 복사
		TCHAR* tempStr = cloneStr(_inStr);

		//1. 리스트를 하나 만들고 대상 문자열을 넣는다. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(tempStr);

		//2. iterator를 구한다. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr이 가리키는 곳에 splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum 갱신
		textNum++;
	}
	void insertTextAt(size_m nodeIdx, mString& mStr) {
		if (nodeIdx < 0 || nodeIdx > (textNum-1)) {
			printf("============================>잘못된 범위 in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		//0. 그냥 tchar 문자열로 복사
		TCHAR* tempStr = mStr.cloneStr();

		//1. 리스트를 하나 만들고 대상 문자열을 넣는다. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(tempStr);

		//2. iterator를 구한다. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr이 가리키는 곳에 splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum 갱신
		textNum++;
	}
	
	//5. eraseText : 인덱스가 가리키는 곳의 데이터를 삭제한다. 
	void eraseTextAt(size_m nodeIdx) {
		if (nodeIdx < 0 || nodeIdx > (textNum-1)) {
			printf("============================>잘못된 범위 in eraseTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}

		//1. iterator를 구한다. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}

		//2. erase()
		(*textSource).erase(itr);
		
		//3. 문단 갯수 줄이기
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