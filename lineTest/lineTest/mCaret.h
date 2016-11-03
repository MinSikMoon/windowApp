#ifndef _mCaret_
#define _mCaret_
#include <iostream>
#include "lineContainer.h"
using namespace std;

class mCaret {
private:
	int curNodeIdx; //���� ĳ���� �Ҽӵ� ������ ��� �ε���
	int curLineIdxInNode; //��� �ȿ��� ���° ���ΰ�?
	int frontWordNumInLine; //���� ���ξȿ��� ĳ�� �տ� �� ���� �ִ°�?
	int frontWordNumInNode; //��� ������ ���� ĳ�� �տ� �� ���� �ִ°�?
	int absCaretLine; // ĳ�� ���� ��ü �� ������ �����ִ°�?

	
public:
	//1. �⺻ ������
	mCaret() : curNodeIdx(0), curLineIdxInNode(0), frontWordNumInLine(0), absCaretLine(0), frontWordNumInNode(0){}
	//2. setters
	void setCurNodeIdx(int n){
		curNodeIdx = n;
	}

	void setCurLineIdxInNode(int n) {
		curLineIdxInNode = n;
	}

	void setFrontWordNumInLine(int n) {
		frontWordNumInLine = n;
	}

	void setAbsCaretLine(int n) {
		absCaretLine = n;
	}

	//3. methods
	//3.1 �������� ��ĭ �ű��. 
	void moveLeft(lineContainer& lc) {
		int temp = frontWordNumInNode - 1; //�ӽ÷� �ϳ� ������. //���
		if (temp < 0) {
			if (curNodeIdx > 0) { //0������ �������µ�, ��尡 ù ��尡 �ƴ϶�� ��������� ���������� ���߰���. 
				curNodeIdx--; //���� ���� �ű��,
				//curLineIdxInNode =  //���� ����� ���������� �����ε����� �Ű��ش�. 
			}
		}
			frontWordNumInNode = 0; //0 �����δ� ���������� �Ѵ�. 

	}

	
};
#endif // !_mCaret_
