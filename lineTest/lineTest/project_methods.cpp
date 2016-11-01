#include "project_methods.h"

//1. 몇번째 노드에 저장된 데이터인가? : 스크롤바의 yPos가 주어질 경우
int getNodeIdx(map<int, int> v, int yPos, int wordHeight) {
	int sum = 0;
	int tempSum = 0;
	int nodeIdx = 0;
	int frontLine = yPos / wordHeight;

	for (int i = 0; i < v.size(); i++) {

		tempSum += v[i];
		if (frontLine >= sum &&frontLine < tempSum) {
			nodeIdx = i;
			break;
		}
		sum = tempSum;
	}

	return nodeIdx;
};

//2. 화면에 맞추어 개행시켜 보여주는 로직
int autoLineSwitch(HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int,int>& nodeLineNum) {
	int startIdx = 0;
	int lastIdx = 0;
	int sentenceNum;
	int i, j;
	int loopCnt = 0;
	int avgCharWidth2;
	TCHAR* testStr;

	
	for (i = 0; i < v1.size(); i++) { //노드 하나당 돈다. 
		testStr = NULL; //testStr 비워주기
		testStr = v1[i];
		avgCharWidth2 = strPxWidth(hdc, testStr) / _tcslen(testStr); //대강의 한글자 폭 구해보기
		sentenceNum = strPxWidth(hdc, testStr) / screenWidth + 1;
		startIdx = 0;
		//노드 당 몇 줄인가 세어주기.
		int nodeLineCnt = 0;

		if (screenWidth < strPxWidth(hdc, testStr)) {
			for (j = 0; j < sentenceNum; j++) {
				lastIdx = getEndIdx(hdc, testStr, screenWidth, startIdx, avgCharWidth2);
				textOutCustom(hdc, 0, loopCnt*wordHeight - yPos, testStr, startIdx, lastIdx);
				startIdx = lastIdx + 1;
				loopCnt++;
				nodeLineCnt++;
			}
		}
		else {
			TextOut(hdc, 0, loopCnt*wordHeight - yPos, testStr, getLen(testStr));
			loopCnt++;
			nodeLineCnt++;
		}
		nodeLineNum[i] = nodeLineCnt;	//노드의 한 문장당 몇 라인을 차지하는 가에 대한 정보 획득. 
	}

	return loopCnt; //총 몇줄 돌았는지 리턴
}




