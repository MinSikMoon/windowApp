#pragma once
#include <map>
#include "mString.h"


class mScreenLineContainer {
private:
	//1. 노드 컨테이너/ 노드당 화면 라인당 글자갯수, 첫 인덱스, 끝인덱스 기록 가능 
	map<int, map<int, map<int, int> > > container; //노드
	//2. 노드당 현재 화면에서 몇줄로 표시되는지 기록
	map<int, int> nodeLineNumInfo; //노드당 몇 라인을 가지고 있는지. 
	//3. 전체 텍스트는 몇 줄인가
	size_m totalLineNum;
	//4. 화면 위로 잘린 줄은 몇 줄인가
	size_m upperLineNum;
	//5. 현재 화면에 보이는 줄 + 남은 줄 갯수
	size_m restLineNum; //total-upper
	//6. 화면상 보이는 첫 문장은 어떤 노드 소속인가
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	//7. 화면 마지막 문장
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;

public:
	//1. 기본 생성자  생략
	mScreenLineContainer() {}

	//2. 데이터 setter // 0,1,7,8 //0노드의 1번 라인 인덱스는 7개 숫자를 가지고, 첫 문장의 문자열의 인덱스는 8이고 마지막 인덱스는 ~~다. 
	void setNodeLineData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx, int lineLastIdx) {
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