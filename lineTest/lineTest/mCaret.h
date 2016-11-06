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
			return -2; //0이면 -2를 리턴해보자. **********************************오류 발생 목적

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

	//5. 현재 캐럿인덱스가 문장의 첫줄인가? 오버로딩 버젼
	bool isLineFirst(lineContainer& lc, map<int, int>& nodeLineNum) {
		int startIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode) + caretLineIdxInNode;
		if (caretIdx == startIdx || caretIdx == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	//6. 현재 노드,라인의 첫번째 캐럿인덱스는 무엇인가
	int getFirstCaretIdx(int nodeIdx, int lineIdx, lineContainer& lc) {
		return lc.getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	}
	//7. 현재 노드, 라인의 마지막 캐럿인덱스는 무엇인가
	int getLastCaretIdx(int nodeIdx, int lineIdx, lineContainer& lc) {
		int tempFirstIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc); //문장의 첫 캐럿인덱스
		int tempWordCnt = lc.getLastIdx(nodeIdx, lineIdx) - lc.getFirstIdx(nodeIdx, lineIdx) + 1;
		return tempFirstIdx + tempWordCnt;
	}


	// 오버로딩 //캐럿 인덱스와 노드 인덱스, 노드라인 인덱스가 주어질때 인자로 넘긴 캐럿인덱스가 라인의 첫번째 인지 판단
	bool isLineFirst(int caretIdx,int nodeIdx, int lineIdx, lineContainer& lc, map<int, int>& nodeLineNum) {
		int startIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc);
		if (caretIdx == startIdx || caretIdx == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	

	//6. 현재 문장에서 다른 한문장으로 넘어갈때 어떤 캐럿인덱스에 들어가야 되는지 리턴해주자.
	//몇 노드, 몇 라인 을 알면 -> 첫 인덱스와 끝인덱스를 알고 -> 길이를 안다. -> 비교하며 가감해서 캐럿인덱스를 구하자.
	//6.1몇 노드, 몇 라인의 문장은 어떤 캐럿인덱스를 시작으로 끝 캐럿인덱스는 얼마인가?
	//6.2 어떤 노드의 어떤 라인의 어떤 캐럿인덱스에 있으면 x픽셀 값은 얼마인가

	
	//////////////////////////////////////////////////////////////////////////////////////////실험
	////6. 현재 노드,라인의 첫번째 캐럿인덱스는 무엇인가
	//int getFirstCaretIdx(int nodeIdx, int lineIdx, lineContainer& lc) {
	//	return lc.getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	//}

	//// 오버로딩 //캐럿 인덱스와 노드 인덱스, 노드라인 인덱스가 주어질때 인자로 넘긴 캐럿인덱스가 라인의 첫번째 인지 판단
	//bool isLineFirst(int caretIdx, int nodeIdx, int lineIdx, lineContainer& lc, map<int, int>& nodeLineNum) {
	//	int startIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc);
	//	if (caretIdx == startIdx || caretIdx == 0) {
	//		return true;
	//	}
	//	else {
	//		return false;
	//	}
	//}
	////5. getXpixel //캐럿인덱스, 노드 인덱스, 노드라인 인덱스가 주어질때 xpixel값을 구한다. 
	//int getXpixel(HDC hdc, int caretIdx, int nodeIdx, int lineIdx, vector<TCHAR*>& v1, lineContainer& lc, map<int, int>& nodeLineNum) {
	//	int result;
	//	//문장의 첫 캐럿인덱스이면 무조건 0픽셀 리턴,
	//	if (isLineFirst(caretIdx, nodeIdx, lineIdx, lc, nodeLineNum)) {
	//		result = 0;
	//	}
	//	else { //아니면 
	//		int tempStartIdx = lc.getFirstIdx(nodeIdx, lineIdx); //첫번째 실제 인덱스를 알아낸다.
	//		int tempRealIdx = getRealIdx(caretIdx, lc); //현재 캐럿이 실제로 영향을 미치는 문자 인덱스 
	//		result = getStrPixelWidth(hdc, v1[nodeIdx], tempStartIdx, tempRealIdx); //길이를 알아낸다. 
	//	}
	//	return result;
	//}
	//
	
	

	//final test: 1. 타겟 문장/ 2. 시작 인덱스/ 3. 글자 너비/ 4. 한계 pixel 너비
	//4개를 넣으면 한계 픽셀사이즈에 들어가는 갯수를 뽑아준다. 
	//getendidx와 차이점은 화면 사이즈가 아닌 문자열의 픽셀 길이가 제한이 된다는 것 뿐
	//target에는 v1[i] 처럼 통째로의 문자열이 들어간다. 
	//startidx에는 문장의 첫 인덱스가 들어가겠지.
	int getFitWordNum(HDC hdc, TCHAR* targetStr, int limitPixelWidth, int startIdx, int avgCharWidth) {
		
		if (getStrPixelWidth(hdc, targetStr, startIdx, _tcslen(targetStr) - 1) <= limitPixelWidth) {
			int tempIdx;
			tempIdx = (_tcslen(targetStr) - 1);
			return tempIdx-startIdx + 1; //문장의 처음부터 마지막 까지의 갯수를 배출 
		}
		//1. 평균 글자 너비를 이용해서 limitWidth 안에 총 몇글자 정도 들어갈지 예상해본다.
		int estWordNum = limitPixelWidth / avgCharWidth + 1;
		//2. 그 정도 글자로 만든 새로운 문자열 만들기
		int tempEndIdx = startIdx + estWordNum - 1;
		TCHAR* tempStr = subTchar(targetStr, startIdx, tempEndIdx);
		//3. 실제 limitWidth와 비교하면서 가감해본다.
		if (strPxWidth(hdc, tempStr) > limitPixelWidth) { //길면 빼줘야지 한글자씩 
			while (1) {
				tempEndIdx--;
				TCHAR* cleaner = tempStr;
				tempStr = subTchar(targetStr, startIdx, tempEndIdx);
				delete cleaner;
				if (strPxWidth(hdc, tempStr) <= limitPixelWidth)
					break;
			}
		}
		else {
			while (1) {
				tempEndIdx++;
				TCHAR* cleaner = tempStr;
				tempStr = subTchar(targetStr, startIdx, tempEndIdx);
				delete cleaner;
				if (strPxWidth(hdc, tempStr) >= limitPixelWidth)
					break;
			}
		}
		delete tempStr; //힙영역 해제
		return tempEndIdx - startIdx + 1; //갯수를 리턴해줌.
	}


	/////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////



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
	
	//노드의 마지막 라인이 무엇인지 가르쳐 주는 것
	int getLastLineIdxInNode(int nodeIdx, map<int, int>& nodeLineNum) {
		return nodeLineNum[nodeIdx] - 1;
	}
	
	//3.4 상
	void moveUp(HDC hdc, map<int,int>& nodeLineNum, lineContainer& lc, vector<TCHAR*>& v1, int avgCharWidth) {
		int tempCaretIdx;
		int tempNodeIdx;
		int tempLineIdx;
		int tempCurXwidth;
		int tempPrevXwidth;

		//1. 전체 텍스트의 첫번째 줄이면 아무일도 일어나지 않는다.
		if (curNodeIdx == 0 && caretLineIdxInNode == 0) {
			return;
		}
		else { //첫째줄이 아니면 노드가 이동할 것이냐 아니냐로 또 나뉜다. ----------------- 노드가 하나 전으로 간다. 
			if (caretLineIdxInNode == 0) { //첫째줄이라 노드가 이동하는 경우 //이안에서도 첫째 캐럿이냐 아니냐로 나뉘겠지.
				tempNodeIdx = curNodeIdx - 1; //노드가 이전 노드로 이동 한다. 

				if (isLineFirst(lc, nodeLineNum)) { //첫째캐럿? //전노드 마지막줄 첫째 캐럿으로 ㄱ ㄱ
					tempLineIdx = getLastLineIdxInNode(tempNodeIdx, nodeLineNum); //이전 노드의 마지막 라인
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc); //이전 노드 마지막 라인의 첫째 캐럿
				}
				else { //첫째 캐럿이 아니면 이제 사이즈 비교해가면서 옮겨보자. // getfitwordnum 테스트
					//1. 현재 캐럿이 위치하는 곳까지의 xpixel을 구한다. 이게 limit width가 될 것이다. 
					tempLineIdx = getLastLineIdxInNode(tempNodeIdx, nodeLineNum); //이전 노드의 마지막 라인
					
					int tempPrevStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx); //이전문장의 첫째글자 인덱스
					int tempPrevEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //이전 문장의 마지막 글자 인덱스
					tempCurXwidth = getXpixel(hdc, v1, lc, nodeLineNum); //현재 캐럿이 있는 곳의 xpixel 구해짐. //limit width가 된다.
					tempPrevXwidth = getStrPixelWidth(hdc, v1.at(tempNodeIdx), tempPrevStartIdx, tempPrevEndIdx); //이전 문장의 길이를 가상으로 구함.

					if (tempPrevXwidth < tempCurXwidth) { //이전 문장의 길이가  현재 캐럿이 있는 문장보다 짧다면 이전 문장의 마지막 캐럿으로 
						tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
					}
					else { //아니고 이전 문장이 현재 문장보다 길이가 같거다 길다면 
						int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempPrevStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //이전 문장 첫째 인덱스에서 갯수 더해준만큼
					}

				}
			}
			else { //--------------------------------------------------노드는 바뀌지 않는다. //라인도 정해짐
				tempNodeIdx = curNodeIdx; //노드는 바뀌지 않겠지.
				tempLineIdx = caretLineIdxInNode - 1; //현재 노드안에서 이전 문장 // 여기서 라인 정해줌.

				if (isLineFirst(lc, nodeLineNum)) { //첫째캐럿?
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc); //이전 노드 마지막 라인의 첫째 캐럿
				}
				else {
					int tempPrevStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx); //이전문장의 첫째글자 인덱스
					int tempPrevEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //이전 문장의 마지막 글자 인덱스
					tempCurXwidth = getXpixel(hdc, v1, lc, nodeLineNum); //현재 캐럿이 있는 곳의 xpixel 구해짐. //limit width가 된다.
					tempPrevXwidth = getStrPixelWidth(hdc, v1.at(tempNodeIdx), tempPrevStartIdx, tempPrevEndIdx); //이전 문장의 길이를 가상으로 구함.

					if (tempPrevXwidth < tempCurXwidth) { //이전 문장의 길이가  현재 캐럿이 있는 문장보다 짧다면 이전 문장의 마지막 캐럿으로 
						tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
					}
					else { //아니고 이전 문장이 현재 문장보다 길이가 같거다 길다면 
						int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempPrevStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //이전 문장 첫째 인덱스에서 갯수 더해준만큼
					}
				}
			}
			
		}
		curNodeIdx = tempNodeIdx;
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc);
	}
	//===========================================================================================================
	//노드와 라인을 넣으면 그게 해당 노드의 마지막 라인인지 검출해주는 함수
	bool isLastLine(map<int, int>& nodeLineNum, int NodeIdx, int lineIdx) {
		int temp = nodeLineNum[NodeIdx] - 1;

		if (lineIdx == temp) {
			return true;
		}
		else {
			return false;
		}
	}

	//3.5 하 // 아래로 움직이기
	void moveDown(HDC hdc, map<int, int>& nodeLineNum, lineContainer& lc, vector<TCHAR*>& v1, int avgCharWidth) {
		int tempCaretIdx;
		int tempNodeIdx;
		int tempLineIdx;
		int tempCurXwidth;
		int tempNextXwidth;
		
		int tempLastNodeIdx = v1.size() - 1; //마지막 노드 검출하는 변수 //왜 5, -1이 나올까. //하나 빼줘야함. 
		int tempLastLineIdx = getLastLineIdxInNode(tempLastNodeIdx, nodeLineNum); //마지막 라인
		
		printf("마지막 노드인덱스는 ==> %d , 라인은 %d \n", tempLastNodeIdx, tempLastLineIdx);
		//1. 전체 텍스트의 마지막 노드, 마지막 라인이면 아무일도 일어나지 않는다. 
		if (curNodeIdx == tempLastNodeIdx && caretLineIdxInNode == tempLastLineIdx) {
			return;
		}
		else { //1.1 노드가 이동한다.
			if (isLastLine(nodeLineNum, curNodeIdx, caretLineIdxInNode)) {//캐럿이 해당 노드의 마지막 줄에 있는가도 검출이 가능해야 되네. // lineNodeNum[curNodeIdx] - 1 하면 나옴.
				tempNodeIdx = curNodeIdx + 1; //다음 노드로 ㄱ ㄱ
				tempLineIdx = 0; //노드가 올라가면 다음 줄은 무조건 인덱스가 0이니까. 

				//1.1.1 해당 라인의 첫째 캐럿이면 다음 노드 첫째줄의 첫째 캐럿으로(0) ㄱ ㄱ 
				if (isLineFirst(lc, nodeLineNum)) {
					tempCaretIdx = 0;
				}
				else { //1.1.2 문장의 첫째 캐럿이 아니라면 사이즈 비교하면서 옮긴다. 
								
					int tempNextStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx) ;
					int tempNextEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //tempLineIdx는 0 이겠지.

					tempCurXwidth = getXpixel(hdc, v1, lc, nodeLineNum);// 현재 캐럿이 있는 곳의 xpixel // limit width가 된다. 
					tempNextXwidth = getStrPixelWidth(hdc, v1.at(tempNodeIdx), tempNextStartIdx, tempNextEndIdx); //다음 문장 길이를 임시로 구함.
					//1.1.2.1 다음 문장 길이가 현재 캐럿있는 문장보다 짧으면 그냥 마지막 캐럿으로 ㄱ ㄱ
					if (tempNextXwidth < tempCurXwidth) {
						tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
					}
					else { //1.1.2.2 다음 문장이 현재 문장보다 길이가 같거나 길다면 
						int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempNextStartIdx, avgCharWidth);
						tempCaretIdx = tempCnt; //0 + tempCnt // 다음 노드라면 무조건 캐럿인덱스도 0에서 시작하잖아. 
					}
				}
			}
			else { //1.2 노드가 이동하지 않는다. 
				tempNodeIdx = curNodeIdx; //노드가 이동하지 않는다. 
				tempLineIdx = caretLineIdxInNode + 1;

				int tempNextStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx);
				int tempNextEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx);
				
				//1.1.1 해당 라인의 첫째 캐럿이면 다음 라인의 첫째 캐럿으로 ㄱ ㄱ 
				if (isLineFirst(lc, nodeLineNum)) {
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc);
				}
				else { //1.1.2 문장의 첫째 캐럿이 아니라면 사이즈 비교하면서 옮긴다. 
					int tempNextStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx);
					int tempNextEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //tempLineIdx는 0 이겠지.

					tempCurXwidth = getXpixel(hdc, v1, lc, nodeLineNum);// 현재 캐럿이 있는 곳의 xpixel // limit width가 된다. 
					tempNextXwidth = getStrPixelWidth(hdc, v1.at(tempNodeIdx), tempNextStartIdx, tempNextEndIdx); //다음 문장 길이를 임시로 구함.
					//1.1.2.1 다음 문장 길이가 현재 캐럿있는 문장보다 짧으면 그냥 마지막 캐럿으로 ㄱ ㄱ
					if (tempNextXwidth < tempCurXwidth) {
						tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
					}
					else { //1.1.2.2 다음 문장이 현재 문장보다 길이가 같거나 길다면 
						int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempNextStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //0 + tempCnt // 다음 노드라면 무조건 캐럿인덱스도 0에서 시작하잖아. 
					}
				}
				//if (tempNextXwidth < tempCurXwidth) {
				//	tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
				//}
				//else { //1.2.2.2 다음 문장이 현재 문장보다 길이가 같거나 길다면 
				//	int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempNextStartIdx, avgCharWidth);
				//	tempCaretIdx = tempCnt; //0 + tempCnt // 다음 노드라면 무조건 캐럿인덱스도 0에서 시작하잖아. 
				//}


			}
		}
		curNodeIdx = tempNodeIdx;
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc);
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
