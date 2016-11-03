#ifndef _mCaret_
#define _mCaret_
#include <iostream>
#include <map>
#include "lineContainer.h"
using namespace std;

class mCaret {
private:
	//입력 관련
	int curNodeIdx; //1. 캐럿이 어떤 노드에 속해있는가? (노드 인덱스)
	int frontWordCntInNode; //캐럿 앞에 몇글자 있는가? (캐럿 인덱스와 헷갈리지 말자. 이거는 철저히 입력용으로 문자열 인덱스-1과 호환됨.
	//출력 관련 
	int upperLineCnt; //캐럿 위로 몇 줄이나 있나?
	int caretIdx; //캐럿은 strlen(노드의 문자열) + 줄수 의 범위를 가진다. 
	int caretLineIdxInNode; //캐럿이 현재 노드안에서 몇 번째 라인에 있느냐?
	
	//내부 함수
	//1. 캐럿 인덱스를 넣으면 현재 노드의 몇 번째 라인에 속해있는지 지정해주는 함수
	int getNodeAndLine(int caretIdx, map<int,int>& nodeLineNum, lineContainer& lc) {
		int lineCnt = nodeLineNum[curNodeIdx]; //현재 노드가 몇 줄 차지 하는지 나온다. 

		for(int i = 0; i < lineCnt; i++) { //라인 수만큼 돌면서 체크해준다. 
			int startIdx; //캐럿인덱스의 시작 인덱스를 임시저장.
			startIdx = lc.getFirstIdx(curNodeIdx, i) + i; //캐럿 인덱스이다! 헷갈리지 말자.
			int endIdx;
			endIdx = startIdx + lc.getWordCnt(curNodeIdx, i); //시작 캐럿 인덱스에서 글자수만큼 더해주면 끝 캐럿 인덱스.

			if (startIdx <= caretIdx && caretIdx <= endIdx) //범위 안에 들어가면 해당 라인 인덱스 리턴
				return i;
		}
	}
	//2. 캐럿이 무슨 노드의 몇 번째 라인에 있는지 알았으므로, 전체 텍스트기준으로 위로 몇 줄 있는지 구한다. 
	int getUpperLineCnt(int caretLineIdxInNode, map<int, int>& nodeLineNum) {
		//1. curNode전까지 더한다. 
		int sum = 0;
		for (int i = 0; i < curNodeIdx; i++) {
			sum += nodeLineNum[i];
		}
		sum += caretLineIdxInNode; //앞 노드들 다 더하고 내 줄 더해주면 캐럿위로 몇 줄 있는지 나옴.
		return sum;
	}
	
	
		
public:
	//1. 기본 생성자
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
