#pragma once
#include "mHeaders.h"


class mScreenLineContainer {
	map<int, map<int, map<int, int> > > container;

public:
	//1. �⺻ ������  ����
	mScreenLineContainer() {}

	//2. ������ setter // 0,1,7,8 //0����� 1�� ���� �ε����� 7�� ���ڸ� ������, ù ������ ���ڿ��� �ε����� 8�̰� ������ �ε����� ~~��. 
	void setData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx, int lineLastIdx) {
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