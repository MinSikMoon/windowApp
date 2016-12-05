#pragma once

#include "mScreenLineContainer.h"


class mCaretMaster {
private:
	size_m caretIdx; //캐럿의 인덱스 
	size_m caretNodeIdx;//2, 3 캐럿의 소속된 리스트의 노드 인덱스// 노드 안에서의 라인인덱스
	size_m caretLineIdxInNode;
	size_m frontWordNumInNode;//4. 캐럿 앞에 몇 글자 있는가? in node
	size_m upperLineNum;//5. 캐럿 위로 몇 줄 있는가?//===================================
	//---------------------------------------------
	size_m xPixel, yPixel;//6.,7 캐럿의 x,y 픽셀 좌표 //upperLineNum, textSource, lc 완성 // 위 5개가 중요한 변수들 
	size_m width, height;//8. 캐럿의 y사이즈 = 글자의 크기



	//private 함수
	//1. 캐럿 인덱스를 넣으면 현재 노드의 몇 번째 라인에 속해있는지 지정해주는 함수
	size_m getCaretLineIdxInNode(size_m caretIdx, size_m caretNodeIdx, mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(caretNodeIdx); //현재 캐럿소속 노드는 몇줄 차지하는가
		
		for (int i = 0; i < curNodeLineNum; i++) {
			int startCaretIdx = lc.getFirstIdx(caretNodeIdx, i) + i;
			int endCaretIdx = lc.getWordCnt(caretNodeIdx, i) + startCaretIdx;
		
			if (startCaretIdx <= caretIdx && caretIdx <= endCaretIdx) {
				return  i;
			}
		}
		//printf("getCaretLineIdxInNod = -1 \n");
		return 0; //뭔가 찝찝하긴 한데 일단 해보자. 
	}

	size_m getUpperLineNum(size_m caretNodeIdx, size_m LineIdxInNode, mScreenLineContainer& lc) {
		int sum = 0;
		for (int i = 0; i < caretNodeIdx; i++) {
			sum += lc.getNodeLineNumInfo(i); //구하고자 하는 캐럿노드 이전까지의 노드 라인을 모두 더해준다. 
		}
		sum += LineIdxInNode; //캐럿의 현재 라인인덱스를 더해주면 캐럿위로 몇개의 라인이 있는지 sum에 저장된다. 
		return sum;
	}
	
	size_m getFrontWordNumInNode(size_m caretIdx, size_m caretNodeIdx, size_m LineIdxInNode, mScreenLineContainer& lc) {
		if (caretIdx == 0) {
			return 0;
		}

		int startIdx = lc.getFirstIdx(caretNodeIdx, LineIdxInNode) + LineIdxInNode; //매개변수 캐럿이 속한 라인의 시작 캐럿 인덱스
		
		//printf("현재 캐럿인덱스 %d, 캐럿 노드 인덱스 %d, 캐럿 라인인덱스 %d, 이 라인의 글자수 %d \n", caretIdx, caretNodeIdx, LineIdxInNode, lc.getWordCnt(caretNodeIdx, LineIdxInNode));
		if (caretIdx == startIdx) { //
			//printf("현재 커서앞에 %d개 문자 있습니다. \n",caretIdx - LineIdxInNode - 1);
			return caretIdx - LineIdxInNode;
		}
		else {
			//printf("현재 커서앞에 %d개 문자 있습니다. \n", caretIdx - LineIdxInNode );
			return caretIdx - LineIdxInNode;
		}
	}

	size_m getLastCaretIdx(size_m nodeIdx, mScreenLineContainer& lc) { //노드의 마지막 캐럿 인덱스 구하기 
		int lineCnt = lc.getNodeLineNumInfo(nodeIdx);
		return lc.getLastIdx(nodeIdx, (lineCnt - 1)) + lineCnt;
	}

	size_m getLastCaretIdxInLine(int nodeIdx, int lineIdx, mScreenLineContainer& lc) {
		int tempFirstIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc); //문장의 첫 캐럿인덱스
		int tempWordCnt = lc.getLastIdx(nodeIdx, lineIdx) - lc.getFirstIdx(nodeIdx, lineIdx) + 1;
		return tempFirstIdx + tempWordCnt;
	}

	size_m getFirstCaretIdx(size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		if (lineIdx == 0) {
			return 0;
		}

		return lc.getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	}

	bool isLineFirst(size_m caretIdx, size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		int startIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc);
		if (caretIdx == startIdx) {
			return true;
		}

		return false;
	}
	size_m getLastLineIdxInNode(size_m nodeIdx, mScreenLineContainer& lc) {
		return lc.getNodeLineNumInfo(nodeIdx) - 1;
	}
	bool isLineLast(size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		size_m tempLineIdx = getLastLineIdxInNode(nodeIdx, lc);
		if (lineIdx == tempLineIdx) {
			return true;
		}
		else {
			return false;
		}
	}
	
	size_m getFitWordNum(HDC hdc, mString& mStr, size_m limitWidth, size_m startIdx, size_m avgCharWidth) {
		if (startIdx < 0) {
			printf("====================================> 잘못된 범위 int getFitWordNum() startIdx가 0보다 작아짐 : %d \n", startIdx);
			system("pause");
			exit(-1);
		}
		if (getMstrPixelWidth(hdc, mStr, startIdx, mStr.getLength() - 1) <= limitWidth) {
			return (mStr.getLength() - 1);
		}

		//1. 평균 글자 너비를 이용해서 limitWidth 안에 총 몇글자 정도 들어갈지 예상해본다.
		int estWordNum = limitWidth / avgCharWidth + 1;
		if (estWordNum > mStr.getLength()) {
			estWordNum = mStr.getLength();
		}

		//2. 그 정도 글자로 만든 새로운 문자열 만들기
		int tempEndIdx = startIdx + estWordNum - 1;
		//잘못된 인덱스 필터링
		if (tempEndIdx > (mStr.getLength() - 1))
			tempEndIdx = mStr.getLength() - 1;

		mString tempStr(mStr.subFromTo(startIdx, tempEndIdx)); //임시 mString 하나 만들기 이제 //3. 실제 limitWidth와 비교하면서 가감해본다.
															  

		if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth) { //길면 빼줘야지 한글자씩 
			while (1) {
				tempEndIdx--;
				tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //새걸로 교체 
				if (getMstrPixelWidthFull(hdc, tempStr) <= limitWidth)
					break;
			}
		}
		else {
			while (1) { //짧으면 한개씩 더해준다. 
				tempEndIdx++;
				tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //새걸로 교체 
				if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth)
					break;
			}
		}

		return tempEndIdx - startIdx + 1; //갯수를 리턴해줌.
	}



	////=> private 함수들 : 캐럿 인덱스를 넣으면 모두 정해진다. 캐럿 노드 인덱스는 0에서 시작해 캐럿이 움직일때마다 갱신되는 걸로 
	void setCaretNodeIdx(size_m n) {
		caretNodeIdx = n;
	}
	void setCaretLineIdxInNode(size_m n) {
		caretLineIdxInNode = n;
	}
	void setFrontWordNumInNode(size_m n) {
		frontWordNumInNode = n;
	}
	void setUpperLineNum(size_m n) {
		upperLineNum = n;
	}

	/////////////////////////////////////////////////////PUBLIC//////////////////////////////////////////
public:
	//1. 생성자
	mCaretMaster() {}
	mCaretMaster(size_m width, size_m height) {
		caretIdx = 0;
		caretNodeIdx = 0;
		caretLineIdxInNode = 0;
		frontWordNumInNode = 0;
		upperLineNum = 0;
		xPixel = 0;
		yPixel = 0;
		this->width = width;
		this->height = height;
	}
	void make(size_m width, size_m height) {
		caretIdx = 0;
		caretNodeIdx = 0;
		caretLineIdxInNode = 0;
		frontWordNumInNode = 0;
		upperLineNum = 0;
		xPixel = 0;
		yPixel = 0;
		this->width = width;
		this->height;
	}
	
	//2. set캐럿인덱스
	void setCaretIdx(int newCaretIdx, int caretNodeIdx, mScreenLineContainer& lc) {
		caretIdx = newCaretIdx;
		caretLineIdxInNode = getCaretLineIdxInNode(newCaretIdx, caretNodeIdx, lc);
		frontWordNumInNode = getFrontWordNumInNode(newCaretIdx, caretNodeIdx, caretLineIdxInNode, lc);
		upperLineNum = getUpperLineNum(caretNodeIdx, caretNodeIdx, lc);
	}

	size_m getWidth() {
		return width;
	}
	size_m getHeight() {
		return height;
	}

	////set, get xPixel
	size_m getXpixel(HDC hdc, mTextSource& textSource, mScreenLineContainer& lc) {
		int result;
		//픽셀이 문장의 첫 줄이면 무조건 0
		if (isLineFirst(caretIdx, caretNodeIdx, caretLineIdxInNode, lc)) {
			result = 0;
		}
		else { //아니면 
			int tempStartIdx = lc.getFirstIdx(caretNodeIdx, caretLineIdxInNode); //첫번째 인덱스를 알아낸다.
			result = getMstrPixelWidth(hdc, mString(textSource.getTextAt(caretNodeIdx)), tempStartIdx, frontWordNumInNode); //길이를 알아낸다. 
		}
		return result;
	}

	
	
	size_m getYpixel() {
		return height*upperLineNum;
	}


	//3. 캐럿의 노드 인덱스는 이동하면서 결정된다. 따라서 세터가 따로 존재하지 않는다. 
	//1. 상
	void moveUp(HDC hdc, int avgCharWidth, mTextSource& textSource, mScreenLineContainer& lc) {
		int tempCaretIdx;
		int tempNodeIdx;
		int tempLineIdx;
		int tempCurXwidth;
		int tempPrevXwidth;

		//1. 전체 텍스트의 첫번째 줄이면 아무일도 일어나지 않는다.
		if (caretNodeIdx == 0 && caretLineIdxInNode == 0) {
			return;
		}
		else { //첫째줄이 아니면 노드가 이동할 것이냐 아니냐로 또 나뉜다. ----------------- 노드가 하나 전으로 간다. 
			if (caretLineIdxInNode == 0) { //첫째줄이라 노드가 이동하는 경우 //이안에서도 첫째 캐럿이냐 아니냐로 나뉘겠지.
				tempNodeIdx = caretNodeIdx - 1; //노드가 이전 노드로 이동 한다. 

				if (isLineFirst(caretIdx, caretNodeIdx, caretLineIdxInNode, lc)) { //첫째캐럿? //전노드 마지막줄 첫째 캐럿으로 ㄱ ㄱ
					tempLineIdx = getLastLineIdxInNode(tempNodeIdx, lc); //이전 노드의 마지막 라인
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc); //이전 노드 마지막 라인의 첫째 캐럿
				}
				else { //첫째 캐럿이 아니면 이제 사이즈 비교해가면서 옮겨보자. // getfitwordnum 테스트
					   //1. 현재 캐럿이 위치하는 곳까지의 xpixel을 구한다. 이게 limit width가 될 것이다. 
					tempLineIdx = getLastLineIdxInNode(tempNodeIdx, lc); //이전 노드의 마지막 라인

					int tempPrevStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx); //이전문장의 첫째글자 인덱스
					int tempPrevEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //이전 문장의 마지막 글자 인덱스
					tempCurXwidth =getXpixel(hdc, textSource, lc); //현재 캐럿이 있는 곳의 xpixel 구해짐. //limit width가 된다.
					tempPrevXwidth = getMstrPixelWidth(hdc, mString(textSource.getTextAt(tempNodeIdx)), tempPrevStartIdx, tempPrevEndIdx); //이전 문장의 길이를 가상으로 구함.
					

					if (tempPrevXwidth < tempCurXwidth) { //이전 문장의 길이가  현재 캐럿이 있는 문장보다 짧다면 이전 문장의 마지막 캐럿으로 
						tempCaretIdx = getLastCaretIdxInLine(tempNodeIdx, tempLineIdx, lc);
					
					}
					else { //아니고 이전 문장이 현재 문장보다 길이가 같거다 길다면 
						int tempCnt = getFitWordNum(hdc, mString(textSource.getTextAt(tempNodeIdx)), tempCurXwidth, tempPrevStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //이전 문장 첫째 인덱스에서 갯수 더해준만큼
					}

				}
			}
			else { //--------------------------------------------------노드는 바뀌지 않는다. //라인도 정해짐
				tempNodeIdx = caretNodeIdx; //노드는 바뀌지 않겠지.
				tempLineIdx = caretLineIdxInNode - 1; //현재 노드안에서 이전 문장 // 여기서 라인 정해줌.

				if (isLineFirst(caretIdx, caretNodeIdx, caretLineIdxInNode, lc)) { //첫째캐럿?
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc); //이전 노드 마지막 라인의 첫째 캐럿
				}
				else {
					int tempPrevStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx); //이전문장의 첫째글자 인덱스
					int tempPrevEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //이전 문장의 마지막 글자 인덱스
					tempCurXwidth = getXpixel(hdc, textSource, lc); //현재 캐럿이 있는 곳의 xpixel 구해짐. //limit width가 된다.
					tempPrevXwidth = getMstrPixelWidth(hdc, mString(textSource.getTextAt(tempNodeIdx)), tempPrevStartIdx, tempPrevEndIdx); //이전 문장의 길이를 가상으로 구함.

					if (tempPrevXwidth < tempCurXwidth) { //이전 문장의 길이가  현재 캐럿이 있는 문장보다 짧다면 이전 문장의 마지막 캐럿으로 
						tempCaretIdx = getLastCaretIdxInLine(tempNodeIdx, tempLineIdx, lc);
					}
					else { //아니고 이전 문장이 현재 문장보다 길이가 같거다 길다면 
						int tempCnt = getFitWordNum(hdc, mString(textSource.getTextAt(tempNodeIdx)), tempCurXwidth, tempPrevStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //이전 문장 첫째 인덱스에서 갯수 더해준만큼
					}
				}
			}

		}
		caretNodeIdx = tempNodeIdx;
		caretIdx = tempCaretIdx;
		setCaretIdx(tempCaretIdx, tempNodeIdx, lc);
	}

	//2. 하
	//3. 좌
	//4. 우
	void moveRight(mTextSource& textSource,  mScreenLineContainer& lc) { //현재 textSOURCE와 LC의 자료시점 차이가 있다. TEXTSOURCE가 먼저 입력되는 상황 
		int tempCaretIdx;
	
		//1. 캐럿이 마지막 캐럿 인덱스라면 
		if (caretIdx == getLastCaretIdxInLine(caretNodeIdx, caretLineIdxInNode, lc)) {
			printf("캐럿이 마지막이다. ");
			if (caretNodeIdx == lc.getTextSourceNodeNum()-1) { //1.1 근데 마지막 노드라면 //텍스트 소스의 사이즈 
				tempCaretIdx = caretIdx; //현재 상태 유지
			}
			else { //마지막 노드가 아니면 다음 노드의 첫번째 캐럿 인덱스로 옮겨 줘야겠지.
				caretNodeIdx++;
				tempCaretIdx = 0;
			}
		}
		else { //이도 저도 아니면 그냥 +1해주면 되겠지.
			tempCaretIdx = caretIdx + 1;
		}
		caretIdx = tempCaretIdx;
		setCaretIdx(tempCaretIdx, caretNodeIdx, lc);
	}

	//노드의 마지막 라인이 무엇인지 가르쳐 주는 것
	int getLastLineIdxInNode(int nodeIdx, map<int, int>& nodeLineNum) {
		return nodeLineNum[nodeIdx] - 1;
	}

	
	//get caret width, height
	
	


	

	

	


	//debugging
	void show() {
		printf("cur caret x,y pixel = %d , %d \n", xPixel, yPixel);
	}
	int getFrontWordNum() {
		return frontWordNumInNode;
	}



};