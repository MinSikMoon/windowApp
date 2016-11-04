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

//결론 : 결국 눈에 보이는 것은 캐럿인덱스가 움직이는것/ 문자열 수정은 캐럿인덱스에 따른 실제 인덱스에 따른다. 


class mCaret {
private:
	//입력 관련
	int curNodeIdx; //1. 캐럿이 어떤 노드에 속해있는가? (노드 인덱스)
	int frontWordIdxInNode; //캐럿 앞에 몇글자 있는가? (캐럿 인덱스와 헷갈리지 말자. 이거는 철저히 입력용으로 문자열 인덱스-1과 호환됨.
	//출력 관련 
	int upperLineCnt; //캐럿 위로 몇 줄이나 있나?
	int caretIdx; //캐럿은 strlen(노드의 문자열) + 줄수 의 범위를 가진다. 
	int caretLineIdxInNode; //캐럿이 현재 노드안에서 몇 번째 라인에 있느냐?
	
	//내부 함수
	//1. 캐럿 인덱스를 넣으면 현재 노드의 몇 번째 라인에 속해있는지 지정해주는 함수--------------------------- -1배출
	int getLineInNode(int caretIdx, map<int,int>& nodeLineNum, lineContainer& lc) {
		int lineCnt = nodeLineNum[curNodeIdx]; //현재 노드가 몇 줄 차지 하는지 나온다. 

		for(int i = 0; i < lineCnt; i++) { //라인 수만큼 돌면서 체크해준다. 
			int startIdx; //캐럿인덱스의 시작 인덱스를 임시저장.
			startIdx = lc.getFirstIdx(curNodeIdx, i) + i; //캐럿 인덱스이다! 헷갈리지 말자.
			int endIdx;
			endIdx = startIdx + lc.getWordCnt(curNodeIdx, i); //시작 캐럿 인덱스에서 글자수만큼 더해주면 끝 캐럿 인덱스.

			if (startIdx <= caretIdx && caretIdx <= endIdx) //범위 안에 들어가면 해당 라인 인덱스 리턴
				return i;
		}

		//오류나면 0리턴 해보자.// 원래 -1이었음. 이게 오류 나는 걸까봐
		return 0; 
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
	
	//3. 현재 캐럿인덱스는 어떤 실제 문자열 인덱스에 영향을 미치는가? ------------------------------------- -1 배출
	int getRealIdx(int caretIdx, lineContainer& lc) {
		if (caretIdx == 0)
			return -1; //0이면 0을 리턴해보자.

		int startIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode) + caretLineIdxInNode; //현재 캐럿이 속한 라인의 시작 캐럿인덱스
		if (caretIdx == startIdx) { //문장의 시작 캐럿 인덱스라면 .
			return caretIdx - caretLineIdxInNode - 2;
		}
		else {
			return caretIdx - caretLineIdxInNode - 1;
		}
	}

	//4. 노드의 마지막 캐럿 인덱스를 구해주는 함수
	int getLastCaretIdx(int nodeIdx, lineContainer& lc, map<int, int>& nodeLineNum) {
		//1. 노드가 몇 줄 차지하는지 구해본다.
		int lineCnt = nodeLineNum[nodeIdx];
		//2. lineCnt-1 의 인덱스의 마지막 인덱스에 lineCnt 만큼 더해주면 마지막 캐럿 인덱스가 된다. 
		return lc.getLastIdx(nodeIdx, lineCnt - 1) + lineCnt;
	}

	//5. 현재 캐럿인덱스가 문장의 첫줄인가?
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
	//1. 기본 생성자
	mCaret() : curNodeIdx(0), frontWordIdxInNode(0), upperLineCnt(0), caretIdx(0), caretLineIdxInNode(0){}
	
	//2. setters //결국 밖에서는 캐럿 인덱스와 노드만 지정해주면 안에서 자동으로 해주게끔 한다. 
	/*void setCurNodeIdx(int n){
		curNodeIdx = n;
	}*/ //굳이 필요없는듯
	void setCaretIdx(int n, map<int, int>& nodeLineNum,lineContainer& lc) {
		caretIdx = n; //캐럿이 지정되면 frontWordCnt도 되어야 겠지.
		setCaretLineIdxInNode(getLineInNode(caretIdx, nodeLineNum, lc)); //현재 노드 안에서 몇 줄안 인지 지정.
		setFrontWordIdxInNode(getRealIdx(caretIdx, lc)); //캐럿이 영향을 미치는 실제 인덱스 지정
		setUpperLineCnt(getUpperLineCnt(caretLineIdxInNode, nodeLineNum)); //위로 몇줄인지 지정
	}
	//3. 캐럿의 이동
	//3.1 왼쪽
	void moveLeft(lineContainer& lc, map<int, int>& nodeLineNum) {
		int tempCaretIdx = caretIdx - 1; //가상으로 한번 옮겨본다. 
		if (tempCaretIdx < 0) { // x==0 위치에서 왼쪽 화살표를 눌렀다. 
			if (curNodeIdx == 0) { //첫 노드라면
				tempCaretIdx = 0; //더 이상 안 움직이게.
			}
			else { //첫 노드가 아니라면 
				curNodeIdx--; //노드를 하나 이전으로 옮겨준다. 
				tempCaretIdx = getLastCaretIdx(curNodeIdx, lc, nodeLineNum); //이전 노드의 마지막 캐럿 인덱스로 옮겨준다. //노드의 마지막 캐럿 위치를 아는 함수를 만들자. 
			}
		}
		else { // x==0위치가 아닌 곳에서 눌렀다면 그대로 tempcaretidx가 caretidx가 되겠지.

		}
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc); //자동으로 나머지 변수들 계산해줌.
	}

	//3.2 오른쪽 //총 노드가 몇개인지도 알아야 하기에 v1이 들어가 줘야한다. 
	void moveRight(lineContainer& lc, map<int, int>& nodeLineNum, vector<TCHAR*> v1) {
		int tempCaretIdx;
		//1. 캐럿이 마지막 캐럿 인덱스라면 
		if (caretIdx == getLastCaretIdx(curNodeIdx, lc, nodeLineNum)) {
			if ( curNodeIdx == (v1.size()-1) ) { //1.1 근데 마지막 노드라면 
				tempCaretIdx = caretIdx; //현재 상태 유지
			}
			else { //마지막 노드가 아니면 다음 노드의 첫번째 캐럿 인덱스로 옮겨 줘야겠지.
				curNodeIdx++;
				tempCaretIdx = 0;
			}
		}
		else { //이도 저도 아니면 그냥 +1해주면 되겠지.
			tempCaretIdx = caretIdx + 1;
		}
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc);
	}

	//3.3 상 // 위로 이동 // 아직 테스트 말자. 
	void moveUp() {

	}

	//3.4 하
	void moveDown() {

	}

	//4. 캐럿의 출력
	//5. getXpixel //현재 캐럿의 x픽셀 값 추출, 
	int getXpixel(HDC hdc, vector<TCHAR*>& v1, lineContainer& lc, map<int,int>& nodeLineNum) {
		int result;
		//픽셀이 문장의 첫 줄이면 무조건 0
		if (isLineFirst(lc, nodeLineNum)) {
			result = 0;
		}
		else { //아니면 
			int tempStartIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode); //첫번째 인덱스를 알아낸다.
			result = getStrPixelWidth(hdc, v1[curNodeIdx], tempStartIdx, frontWordIdxInNode); //길이를 알아낸다. 
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
