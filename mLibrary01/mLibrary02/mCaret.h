#include "mHeaders.h"
#include "mString.h"
#include "mTextPixel.h"
#include "mScreenLineContainer.h"
#pragma once


class mCaret {
	//캐럿 인덱스가 짱이다. // 나머지는 함수로 다 정해짐.
	size_m caretIdx; //캐럿 인덱스, strlen(str)+라인수 의 범위를 가진다.

	size_m curNodeIdx; //캐럿이 소속된 노드 인덱스
	size_m curLineIdxInNode; //캐럿이 소속된 노드 안에서의 라인 인덱스
	size_m frontWordNumInNode; //캐럿앞에 글자 몇개 남았나?
	size_m upperLineNum; //캐럿 위로 몇 줄이 있나?


						 //private setters //캐럿 인덱스를 뺀 나머지들은 모두 이곳에서 세팅해줌.
	void setCurNodeIdx(size_m n) {
		curNodeIdx = n;
	}
	void setCurLineIdxInNode(size_m n) {
		curLineIdxInNode = n;
	}
	void setFrontWordNumInNode(size_m n) {
		frontWordNumInNode = n;
	}
	void setUpperLineNum(size_m n) {
		upperLineNum = n;
	}

	//캐럿 인덱스외 나머지 변수들을 자동으로 구해주는 프라이빗 함수들

	//만약 현재 textSource에 아무것도 안 들어가있는 상황 이라면 문제가 발생할 소지가 있다. 
	//1. 캐럿 소속 노드 인덱스와 캐럿 인덱스를 넣으면 현재 노드의 몇 번째 라인에 속해있는지 알아봐주는 함수--------------------------- -1배출
	size_m getLineInNode(size_m curNodeIdx, size_m caretIdx, map<size_m, size_m>& nodeLineNum, mScreenLineContainer& lc) {
		size_m lineCnt = nodeLineNum[curNodeIdx]; //현재 노드가 몇 줄 차지 하는지 나온다. //textSource의 0 노드의 문자열은 화면에 몇 줄로 출력되는가?

		for (size_m i = 0; i < lineCnt; i++) { //라인 수만큼 돌면서 체크해준다. 
			size_m startIdx; //캐럿인덱스의 시작 인덱스를 임시저장.
			startIdx = lc.getFirstIdx(curNodeIdx, i) + i; //캐럿 인덱스이다! 헷갈리지 말자.
			size_m endIdx;
			endIdx = startIdx + lc.getWordCnt(curNodeIdx, i); //시작 캐럿 인덱스에서 글자수만큼 더해주면 끝 캐럿 인덱스.

			if (startIdx <= caretIdx && caretIdx <= endIdx) //범위 안에 들어가면 해당 라인 인덱스 리턴
				return i;
		}

		//오류나면 0리턴 해보자.// 원래 -1이었음. 이게 오류 나는 걸까봐
		return 0;
	}
	//2. 캐럿이 속한 라인의 첫번째// 마지막 캐럿 인덱스 리턴 함수
	size_m getFirstCaretIdxInLine(size_m nodeIdx, size_m lineIdxInNode, mScreenLineContainer& lc) {
		if (lineIdxInNode == 0) {
			return 0;
		}
		else {
			size_m startCaretIdx = lc.getFirstIdx(nodeIdx, lineIdxInNode) + lineIdxInNode;
			return startCaretIdx;
		}
	}

	bool isLineFirstCaret(size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		int tempStartCaretIdx = getFirstCaretIdxInLine(nodeIdx, lineIdx, lc);
		if (lineIdx == tempStartCaretIdx || lineIdx == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	//라인과 노드의 마지막 캐럿 인덱스를 찾아주는 함수// 헷갈리지 마라.
	size_m getLastCaretIdxInLine(size_m nodeIdx, size_m lineIdxInNode, mScreenLineContainer& lc) {
		int tempLastRealIdxInNode = lc.getLastIdx(nodeIdx, lineIdxInNode);
		return tempLastRealIdxInNode + lineIdxInNode + 1;

	}

	size_m getLastCaretIdxInNode(int nodeIdx, map<size_m, size_m>& nodeLineNum, mScreenLineContainer& lc) {
		int tempLineNum = nodeLineNum[nodeIdx];
		int answer = lc.getLastIdx(nodeIdx, tempLineNum - 1) + tempLineNum;
	}

	//3. 캐럿 앞에 몇 글자 있는지 알아봐주는 함수 : 
	size_m getFrontWordNumInNode(size_m nodeIdx, size_m lineIdxInNode, size_m caretIdx, mScreenLineContainer& lc) {
		if (caretIdx == 0) //캐럿 인덱스가 0이면 앞에 아무 글자도 없겠지. 0개 
			return 0;

		int startCaretIdx = getFirstCaretIdxInLine(nodeIdx, lineIdxInNode, lc); //라인에서 첫번째 캐럿인덱스
		if (caretIdx == startCaretIdx) { //시작 캐럿 인덱스면 자신의 라인인덱스보다 2더큰 값을 빼주면 그게 앞글자 갯수
			return caretIdx - (lineIdxInNode + 2); //인덱스가 아니라 글자의 갯수가 배출됨!!
		}
		else { //나머지들은 자신의 라인인덱스보다 1더 큰 값을 빼주면 그게 앞글자 갯수
			return caretIdx - (lineIdxInNode + 1);
		}
	}

	//4. 캐럿 위로 몇 줄 있는 지 구해준다. (전체 텍스트 기준으로 ) (textController의 변수와 헷갈리지 마라. 이거는 캐럿위로 몇 줄이냐 이거다.)
	size_m getUpperLineNum(size_m nodeIdx, size_m lineIdx, map<size_m, size_m> nodeLineNum) {
		int sum = 0;
		for (int i = 0; i < nodeIdx; i++) { //이전 노드들 라인 갯수 다 더해줌.
			sum += nodeLineNum[i];
		}
		sum += lineIdx;
		return sum;
	}

	//5. 캐럿 앞에 몇 개의 단어가 들어가야 할지, 사이즈 비교. //mStr의 픽셀 길이를 
	int getFitWordNum(HDC hdc, mString& mStr, int limitWidth, int startIdx, int avgCharWidth) {
		if (startIdx < 0) {
			printf("====================================> 잘못된 범위 int getScreenEndIdx() startIdx가 0보다 작아짐 : %d \n", startIdx);
			system("pause");
			exit(-1);
		}


		if (getMstrPixelWidth(hdc, mStr, startIdx, mStr.getLength() - 1) <= limitWidth) { //제한 픽셀 사이즈가 문장보다 더 길면 그냥 문장의 마지막 인덱스 뽑아주면 되겠지. 
			return (mStr.getLength() - startIdx); //startIdx부터의 글자 갯수
		}

		//1. 평균 글자 너비를 이용해서 limitWidth 안에 총 몇글자 정도 들어갈지 예상해본다.
		int estWordNum = limitWidth / avgCharWidth + 1;

		//2. 그 정도 글자로 만든 새로운 문자열 만들기
		int tempEndIdx = startIdx + estWordNum - 1;
		mString tempStr(mStr.subFromTo(startIdx, tempEndIdx));

		//3. 실제 limitWidth와 비교하면서 가감해본다.
		if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth) { //길면 빼줘야지 한글자씩 
			while (1) {
				tempEndIdx--;
				tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //새걸로 교체 
				if (getMstrPixelWidthFull(hdc, tempStr) <= limitWidth)
					break;
			}
		}
		else {
			while (1) {
				tempEndIdx++;
				tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //새걸로 교체 
				if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth)
					break;
			}
		}
		return tempEndIdx - startIdx + 1; //갯수를 리턴해줌.
	}

public:
	//1. CONSTRUCTOR // 다 0으로 초기화 
	mCaret() : curNodeIdx(0), curLineIdxInNode(0), frontWordNumInNode(0), upperLineNum(0), caretIdx(0) {}

	//2. setCaretIdx() : 캐럿 인덱스를 지정하면 curNodeIdx를 뺀 나머지 변수들이 따라서 결정된다. 
	void setCaretIdx(int nodeIdx, int lineIdx, int _caretIdx, map<size_m, size_m>& nodeLineNum, mScreenLineContainer& lc) {
		caretIdx = _caretIdx;
		setCurLineIdxInNode(getLineInNode(nodeIdx, caretIdx, nodeLineNum, lc));
		setFrontWordNumInNode(getFrontWordNumInNode(nodeIdx, lineIdx, caretIdx, lc));
		setUpperLineNum(getUpperLineNum(nodeIdx, lineIdx, nodeLineNum));
	}

	//degugging : show()
	void show() {
		printf("\n ** < 캐럿 정보 > ** \n");
		printf("현재 캐럿 소속 노드 인덱스 : %d \n", curNodeIdx);
		printf("현재 캐럿 소속 노드안 라인 인덱스 : %d \n", curLineIdxInNode);
		printf("현재 캐럿 앞에 몇 글자가 있나? (노드 문자열당) : %d \n", frontWordNumInNode);
		printf("현재 캐럿 위로 몇 줄 있나? : %d \n", upperLineNum);
		printf("현재 캐럿의 캐럿 인덱스 : %d \n", caretIdx);
	}
};


