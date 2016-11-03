#ifndef _mCaret_
#define _mCaret_
#include <iostream>
#include <map>
#include "lineContainer.h"
using namespace std;

class mCaret {
private:
	//�Է� ����
	int curNodeIdx; //1. ĳ���� � ��忡 �����ִ°�? (��� �ε���)
	int frontWordCntInNode; //ĳ�� �տ� ����� �ִ°�? (ĳ�� �ε����� �򰥸��� ����. �̰Ŵ� ö���� �Է¿����� ���ڿ� �ε���-1�� ȣȯ��.
	//��� ���� 
	int upperLineCnt; //ĳ�� ���� �� ���̳� �ֳ�?
	int caretIdx; //ĳ���� strlen(����� ���ڿ�) + �ټ� �� ������ ������. 
	int caretLineIdxInNode; //ĳ���� ���� ���ȿ��� �� ��° ���ο� �ִ���?
	
	//���� �Լ�
	//1. ĳ�� �ε����� ������ ���� ����� �� ��° ���ο� �����ִ��� �������ִ� �Լ�
	int getNodeAndLine(int caretIdx, map<int,int>& nodeLineNum, lineContainer& lc) {
		int lineCnt = nodeLineNum[curNodeIdx]; //���� ��尡 �� �� ���� �ϴ��� ���´�. 

		for(int i = 0; i < lineCnt; i++) { //���� ����ŭ ���鼭 üũ���ش�. 
			int startIdx; //ĳ���ε����� ���� �ε����� �ӽ�����.
			startIdx = lc.getFirstIdx(curNodeIdx, i) + i; //ĳ�� �ε����̴�! �򰥸��� ����.
			int endIdx;
			endIdx = startIdx + lc.getWordCnt(curNodeIdx, i); //���� ĳ�� �ε������� ���ڼ���ŭ �����ָ� �� ĳ�� �ε���.

			if (startIdx <= caretIdx && caretIdx <= endIdx) //���� �ȿ� ���� �ش� ���� �ε��� ����
				return i;
		}
	}
	//2. ĳ���� ���� ����� �� ��° ���ο� �ִ��� �˾����Ƿ�, ��ü �ؽ�Ʈ�������� ���� �� �� �ִ��� ���Ѵ�. 
	int getUpperLineCnt(int caretLineIdxInNode, map<int, int>& nodeLineNum) {
		//1. curNode������ ���Ѵ�. 
		int sum = 0;
		for (int i = 0; i < curNodeIdx; i++) {
			sum += nodeLineNum[i];
		}
		sum += caretLineIdxInNode; //�� ���� �� ���ϰ� �� �� �����ָ� ĳ������ �� �� �ִ��� ����.
		return sum;
	}
	
	
		
public:
	//1. �⺻ ������
	mCaret() : curNodeIdx(0), frontWordCntInNode(0){}
	//2. setters
	void setCurNodeIdx(int n){
		curNodeIdx = n;
	}
	
	void setFrontWordCntInNode(int n) {
		frontWordCntInNode = n;
	}

	void setUpperLineCnt(int n) {
		upperLineCnt = n;
	}

	
	void moveLeft(lineContainer& lc, map<int, int>& nodeLineNum) {
		

	}

	
};
#endif // !_mCaret_
