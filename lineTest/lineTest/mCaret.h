#ifndef _mCaret_
#define _mCaret_
#include <iostream>
#include "lineContainer.h"
using namespace std;

class mCaret {
private:
	int curNodeIdx; //현재 캐럿이 소속된 문장의 노드 인덱스
	int curLineIdxInNode; //노드 안에서 몇번째 줄인가?
	int frontWordNumInLine; //현재 라인안에서 캐럿 앞에 몇 글자 있는가?
	int frontWordNumInNode; //노드 단위로 볼때 캐럿 앞에 몇 글자 있는가?
	int absCaretLine; // 캐럿 위로 전체 몇 문장이 남아있는가?

	
public:
	//1. 기본 생성자
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
	//3.1 왼쪽으로 한칸 옮긴다. 
	void moveLeft(lineContainer& lc) {
		int temp = frontWordNumInNode - 1; //임시로 하나 빼본다. //노드
		if (temp < 0) {
			if (curNodeIdx > 0) { //0밑으로 떨어졌는데, 노드가 첫 노드가 아니라면 이전노드의 마지막으로 가야겠지. 
				curNodeIdx--; //이전 노드로 옮기고,
				//curLineIdxInNode =  //이전 노드의 마지막으로 라인인덱스를 옮겨준다. 
			}
		}
			frontWordNumInNode = 0; //0 밑으로는 못내려가게 한다. 

	}

	
};
#endif // !_mCaret_
