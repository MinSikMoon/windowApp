#pragma once
#include "mHeaders.h"


class mScreenLineContainer {
	map<int, map<int, map<int, int> > > container;

public:
	//1. 기본 생성자  생략
	mScreenLineContainer() {}

	//2. 데이터 setter // 0,1,7,8 //0노드의 1번 라인 인덱스는 7개 숫자를 가지고, 첫 문장의 문자열의 인덱스는 8이고 마지막 인덱스는 ~~다. 
	void setData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx, int lineLastIdx) {
		container[nodeIdx][lineIdx][0] = lineWordCnt;
		container[nodeIdx][lineIdx][1] = lineFirstIdx;
		container[nodeIdx][lineIdx][2] = lineLastIdx;
	};

	//3. 몇 노드의 몇 인덱스 라인의 글자수를 구한다. 
	int getWordCnt(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][0];
	};

	//4. '' 첫 인덱스를 구한다. 
	int getFirstIdx(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][1];
	};

	//5. '' 끝 인덱스를 구한다. 
	int getLastIdx(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][2];
	};
};