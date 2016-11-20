#pragma once
#include <map>
#include "mString.h"


class mScreenLineContainer {
private:
	//1. ��� �����̳�/ ���� ȭ�� ���δ� ���ڰ���, ù �ε���, ���ε��� ��� ���� 
	map<int, map<int, map<int, int> > > container; //���
	//2. ���� ���� ȭ�鿡�� ���ٷ� ǥ�õǴ��� ���
	map<int, int> nodeLineNumInfo; //���� �� ������ ������ �ִ���. 
	//3. ��ü �ؽ�Ʈ�� �� ���ΰ�
	size_m totalLineNum;
	//4. ȭ�� ���� �߸� ���� �� ���ΰ�
	size_m upperLineNum;
	//5. ���� ȭ�鿡 ���̴� �� + ���� �� ����
	size_m restLineNum; //total-upper
	//6. ȭ��� ���̴� ù ������ � ��� �Ҽ��ΰ�
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	//7. ȭ�� ������ ����
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;

public:
	//1. �⺻ ������  ����
	mScreenLineContainer() {}

	//2. ������ setter // 0,1,7,8 //0����� 1�� ���� �ε����� 7�� ���ڸ� ������, ù ������ ���ڿ��� �ε����� 8�̰� ������ �ε����� ~~��. 
	void setNodeLineData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx, int lineLastIdx) {
		container[nodeIdx][lineIdx][0] = lineWordCnt;
		container[nodeIdx][lineIdx][1] = lineFirstIdx;
		container[nodeIdx][lineIdx][2] = lineLastIdx;
	};

	//3. �� ����� �� �ε��� ������ ���ڼ��� ���Ѵ�. 
	int getWordCnt(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][0];
	};

	//4. '' ù �ε����� ���Ѵ�. 
	int getFirstIdx(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][1];
	};

	//5. '' �� �ε����� ���Ѵ�. 
	int getLastIdx(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][2];
	};
};