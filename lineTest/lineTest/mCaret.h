#ifndef _mCaret_
#define _mCaret_
#include <iostream>
#include <map>
#include <vector>
#include <tchar.h>
#include <Windows.h>
#include "lineContainer.h"
#include "mWordPixel.h"
using namespace std;

//��� : �ᱹ ���� ���̴� ���� ĳ���ε����� �����̴°�/ ���ڿ� ������ ĳ���ε����� ���� ���� �ε����� ������. 


class mCaret {
private:
	//�Է� ����
	int curNodeIdx; //1. ĳ���� � ��忡 �����ִ°�? (��� �ε���)
	int frontWordIdxInNode; //ĳ�� �տ� ����� �ִ°�? (ĳ�� �ε����� �򰥸��� ����. �̰Ŵ� ö���� �Է¿����� ���ڿ� �ε���-1�� ȣȯ��.
	//��� ���� 
	int upperLineCnt; //ĳ�� ���� �� ���̳� �ֳ�?
	int caretIdx; //ĳ���� strlen(����� ���ڿ�) + �ټ� �� ������ ������. 
	int caretLineIdxInNode; //ĳ���� ���� ���ȿ��� �� ��° ���ο� �ִ���?
	
	//���� �Լ�
	//1. ĳ�� �ε����� ������ ���� ����� �� ��° ���ο� �����ִ��� �������ִ� �Լ�--------------------------- -1����
	int getLineInNode(int caretIdx, map<int,int>& nodeLineNum, lineContainer& lc) {
		int lineCnt = nodeLineNum[curNodeIdx]; //���� ��尡 �� �� ���� �ϴ��� ���´�. 

		for(int i = 0; i < lineCnt; i++) { //���� ����ŭ ���鼭 üũ���ش�. 
			int startIdx; //ĳ���ε����� ���� �ε����� �ӽ�����.
			startIdx = lc.getFirstIdx(curNodeIdx, i) + i; //ĳ�� �ε����̴�! �򰥸��� ����.
			int endIdx;
			endIdx = startIdx + lc.getWordCnt(curNodeIdx, i); //���� ĳ�� �ε������� ���ڼ���ŭ �����ָ� �� ĳ�� �ε���.

			if (startIdx <= caretIdx && caretIdx <= endIdx) //���� �ȿ� ���� �ش� ���� �ε��� ����
				return i;
		}

		//�������� 0���� �غ���.// ���� -1�̾���. �̰� ���� ���� �ɱ��
		return 0; 
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
	
	//3. ���� ĳ���ε����� � ���� ���ڿ� �ε����� ������ ��ġ�°�? ------------------------------------- -1 ����
	int getRealIdx(int caretIdx, lineContainer& lc) {
		if (caretIdx == 0)
			return -1; //0�̸� 0�� �����غ���.

		int startIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode) + caretLineIdxInNode; //���� ĳ���� ���� ������ ���� ĳ���ε���
		if (caretIdx == startIdx) { //������ ���� ĳ�� �ε������ .
			return caretIdx - caretLineIdxInNode - 2;
		}
		else {
			return caretIdx - caretLineIdxInNode - 1;
		}
	}

	//4. ����� ������ ĳ�� �ε����� �����ִ� �Լ�
	int getLastCaretIdx(int nodeIdx, lineContainer& lc, map<int, int>& nodeLineNum) {
		//1. ��尡 �� �� �����ϴ��� ���غ���.
		int lineCnt = nodeLineNum[nodeIdx];
		//2. lineCnt-1 �� �ε����� ������ �ε����� lineCnt ��ŭ �����ָ� ������ ĳ�� �ε����� �ȴ�. 
		return lc.getLastIdx(nodeIdx, lineCnt - 1) + lineCnt;
	}

	//5. ���� ĳ���ε����� ������ ù���ΰ�?
	bool isLineFirst(lineContainer& lc, map<int, int>& nodeLineNum) {
		int startIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode) + caretLineIdxInNode;
		if (caretIdx == startIdx || caretIdx == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	
	
	void setCaretLineIdxInNode(int n) {
		caretLineIdxInNode = n;
	}

	void setUpperLineCnt(int n) {
		upperLineCnt = n;
	}

	void setFrontWordIdxInNode(int n) {
		frontWordIdxInNode = n;
	}

public:
	//1. �⺻ ������
	mCaret() : curNodeIdx(0), frontWordIdxInNode(0), upperLineCnt(0), caretIdx(0), caretLineIdxInNode(0){}
	
	//2. setters //�ᱹ �ۿ����� ĳ�� �ε����� ��常 �������ָ� �ȿ��� �ڵ����� ���ְԲ� �Ѵ�. 
	/*void setCurNodeIdx(int n){
		curNodeIdx = n;
	}*/ //���� �ʿ���µ�
	void setCaretIdx(int n, map<int, int>& nodeLineNum,lineContainer& lc) {
		caretIdx = n; //ĳ���� �����Ǹ� frontWordCnt�� �Ǿ�� ����.
		setCaretLineIdxInNode(getLineInNode(caretIdx, nodeLineNum, lc)); //���� ��� �ȿ��� �� �پ� ���� ����.
		setFrontWordIdxInNode(getRealIdx(caretIdx, lc)); //ĳ���� ������ ��ġ�� ���� �ε��� ����
		setUpperLineCnt(getUpperLineCnt(caretLineIdxInNode, nodeLineNum)); //���� �������� ����
	}
	//3. ĳ���� �̵�
	//3.1 ����
	void moveLeft(lineContainer& lc, map<int, int>& nodeLineNum) {
		int tempCaretIdx = caretIdx - 1; //�������� �ѹ� �Űܺ���. 
		if (tempCaretIdx < 0) { // x==0 ��ġ���� ���� ȭ��ǥ�� ������. 
			if (curNodeIdx == 0) { //ù �����
				tempCaretIdx = 0; //�� �̻� �� �����̰�.
			}
			else { //ù ��尡 �ƴ϶�� 
				curNodeIdx--; //��带 �ϳ� �������� �Ű��ش�. 
				tempCaretIdx = getLastCaretIdx(curNodeIdx, lc, nodeLineNum); //���� ����� ������ ĳ�� �ε����� �Ű��ش�. //����� ������ ĳ�� ��ġ�� �ƴ� �Լ��� ������. 
			}
		}
		else { // x==0��ġ�� �ƴ� ������ �����ٸ� �״�� tempcaretidx�� caretidx�� �ǰ���.

		}
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc); //�ڵ����� ������ ������ �������.
	}

	//3.2 ������ //�� ��尡 ������� �˾ƾ� �ϱ⿡ v1�� �� ����Ѵ�. 
	void moveRight(lineContainer& lc, map<int, int>& nodeLineNum, vector<TCHAR*> v1) {
		int tempCaretIdx;
		//1. ĳ���� ������ ĳ�� �ε������ 
		if (caretIdx == getLastCaretIdx(curNodeIdx, lc, nodeLineNum)) {
			if ( curNodeIdx == (v1.size()-1) ) { //1.1 �ٵ� ������ ����� 
				tempCaretIdx = caretIdx; //���� ���� ����
			}
			else { //������ ��尡 �ƴϸ� ���� ����� ù��° ĳ�� �ε����� �Ű� ��߰���.
				curNodeIdx++;
				tempCaretIdx = 0;
			}
		}
		else { //�̵� ���� �ƴϸ� �׳� +1���ָ� �ǰ���.
			tempCaretIdx = caretIdx + 1;
		}
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc);
	}

	//3.3 �� // ���� �̵� // ���� �׽�Ʈ ����. 
	void moveUp() {

	}

	//3.4 ��
	void moveDown() {

	}

	//4. ĳ���� ���
	//5. getXpixel //���� ĳ���� x�ȼ� �� ����, 
	int getXpixel(HDC hdc, vector<TCHAR*>& v1, lineContainer& lc, map<int,int>& nodeLineNum) {
		int result;
		//�ȼ��� ������ ù ���̸� ������ 0
		if (isLineFirst(lc, nodeLineNum)) {
			result = 0;
		}
		else { //�ƴϸ� 
			int tempStartIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode); //ù��° �ε����� �˾Ƴ���.
			result = getStrPixelWidth(hdc, v1[curNodeIdx], tempStartIdx, frontWordIdxInNode); //���̸� �˾Ƴ���. 
		}
		return result;
	}

	//6. getYpixel
	int getYpixel(HDC hdc, int wordHeight) {
		return wordHeight*upperLineCnt;
	}


	//7. getters
	int getCurNodeIdx() {
		return curNodeIdx;
	}
	
	int getRealIdx() {
		return frontWordIdxInNode;
	}
	
};
#endif // !_mCaret_
