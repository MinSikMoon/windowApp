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
	//1. �⺻ ������  ����
	lineContainer();
	//2. ������ setter // 0,1,7,8 //0����� 1�� ���� �ε����� 7�� ���ڸ� ������, ù ������ �ε����� 8�̴�. 
	void setData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx);

	//3. ������ ���ڼ��� ���Ѵ�. 
	int getWordCnt(int nodeIdx, int lineIdx);

	//4. ������ ù��° �ε����� ȣ���Ѵ�. 
	int getFirstIdx(int nodeIdx, int lineIdx);
};


#endif