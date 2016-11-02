#ifndef _lineContainer_
#define _lineContainer_

typedef unsigned int mType;

#include <map>
#include <vector>
#include <iostream>
using namespace std;

class lineContainer {
private:
	map<int, map<int, map<int, int> > > container;

public:
	//1. 기본 생성자  생략
	lineContainer();
	//2. 데이터 setter // 0,1,7,8 //0노드의 1번 라인 인덱스는 7개 숫자를 가지고, 첫 문장의 인덱스는 8이다. 
	void setData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx);

	//3. 라인의 글자수를 구한다. 
	int getWordCnt(int nodeIdx, int lineIdx);

	//4. 라인의 첫번째 인덱스를 호출한다. 
	int getFirstIdx(int nodeIdx, int lineIdx);
};


#endif