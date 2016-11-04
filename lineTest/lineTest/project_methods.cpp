#include "project_methods.h"
#include "lineContainer.h"

//1. 몇번째 노드에 저장된 데이터인가? : 스크롤바의 yPos가 주어질 경우
int getNodeIdx(map<int, int> v, int yPos, int wordHeight, int& out_lineNum) {
	int sum = 0;
	int tempSum = 0;
	int nodeIdx = 0;
	int frontLine = yPos / wordHeight; //앞에 몇글자 있는지 본다. 

	for (unsigned int i = 0; i < v.size(); i++) {

		tempSum += v[i];
		if (frontLine >= sum &&frontLine < tempSum) {
			nodeIdx = i;
			out_lineNum = frontLine - sum;
			break;
		}
		sum = tempSum;
	}

	return nodeIdx;

};

//1.5 getNodeIdx의 두번째 버젼 오버로딩 //몇 번째 노드의 몇번째 라인(out_lineNum)인지 뽑아준다. 
int getNodeIdx(map<int, int> nodeLineNum, int upperLineCnt, int& out_lineNum) {
	int sum = 0;
	int tempSum = 0;
	int nodeIdx = 0;
	
	for (unsigned int i = 0; i < nodeLineNum.size(); i++) {

		tempSum += nodeLineNum[i];
		if (upperLineCnt >= sum &&upperLineCnt < tempSum) {
			nodeIdx = i;
			out_lineNum = upperLineCnt - sum;
			break;
		}
		sum = tempSum;
	}

	return nodeIdx;
}

//2. 마지막 노드인덱스와 라인 알아내보기. // 
int getLastScreenNodeIdx(map<int, int> nodeLineNum, int totalLineCnt, int upperLineCnt, int restLineCnt, int screenAvailableCnt, int& out_lineNum) {
	//1. 필터 : 마지막 라인까지 출력되는데 화면 크기가 더 클때 //무조건 마지막 노드의 마지막 라인 배출
	int tempUpperCnt;
	if (upperLineCnt + screenAvailableCnt > totalLineCnt) {
		tempUpperCnt = totalLineCnt - 1;
		return getNodeIdx(nodeLineNum, tempUpperCnt, out_lineNum);
	}
	else { //화면 안에서 텍스트 여분이 없고 화면 밑으로 텍스트가 더 남아있을 때
		tempUpperCnt = upperLineCnt + screenAvailableCnt - 1;
		return getNodeIdx(nodeLineNum, tempUpperCnt, out_lineNum);
	}
}


//2. 화면에 맞추어 개행시켜 보여주는 로직
int autoLineSwitch( HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int,int>& nodeLineNum, lineContainer& lc1) {
	int startIdx = 0;
	int lastIdx = 0;
	int sentenceNum;
	unsigned int i;
	int j;
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
				//i: 몇번째 노드인가// 여기서 nodeLineCnt-1하면 노드안에서 몇번째 줄인지// lastIdx-startIdx+1하면 한 줄에 글자 몇개인지
				//startIdx: 문장의 첫글자의 인덱스는 몇인가?// 맵에 1차원 배열을 key 값으로 받을 수 있는지 조사.
				//printf("%d 번 노드의 %d 번째 라인인데 길이는 %d 입니다. \n", i, j, lastIdx - startIdx + 1); //여기는 잘 나옴.
				lc1.setData(i, j, lastIdx - startIdx + 1, startIdx, lastIdx);
			
				startIdx = lastIdx + 1;
				loopCnt++;
				nodeLineCnt++;
				
				
			}
		}
		else {
			TextOut(hdc, 0, loopCnt*wordHeight - yPos, testStr, getLen(testStr));
		
			lc1.setData(i, 0, getLen(testStr), 0, getLen(testStr)-1);

			loopCnt++;
			nodeLineCnt++;
		}
		nodeLineNum[i] = nodeLineCnt;	//노드의 한 문장당 몇 라인을 차지하는 가에 대한 정보 획득. 
	}
	
	return loopCnt; //총 몇줄 돌았는지 리턴
}

//3. 스크롤 스위치 로직 분리
int mScrollSwitches(WPARAM wParam, int wordHeight, int curScreenLineNum, int& yPos, int yMax) {
	int yInc = 0;

	switch (LOWORD(wParam)) {
	case SB_LINEUP: {
		yInc = -wordHeight;
		break;
	}
	case SB_PAGEUP: {
		yInc = -20 * wordHeight;
		break;
	}
	case SB_LINEDOWN: {
		yInc = wordHeight;
		if (yPos + (curScreenLineNum*wordHeight) >= yMax) {
			yInc = 0;
		}
		break;
	}
	case SB_PAGEDOWN: {
		yInc = 20 * wordHeight;
		if (yPos + yInc >= yMax) {
			int frontWordNum = yPos / wordHeight;
			int tempLoopCnt = yMax / wordHeight;
			int tempCnt = tempLoopCnt - frontWordNum - curScreenLineNum;
			;				yInc = tempCnt * wordHeight;
		}
		break;
	}
	case SB_THUMBTRACK: {
		int temp_yPos = HIWORD(wParam); //thumb의 위치
		int temp_yInc = (temp_yPos - yPos);
		int temp_lineJump = temp_yInc / wordHeight;
		int temp_jumpMore = temp_yInc % wordHeight;
		int halfCharWidth = wordHeight / 2;

		if (temp_jumpMore >= halfCharWidth) {
			temp_lineJump++;
		}

		yInc = temp_lineJump * wordHeight;/*printf("현재 스크롤바 위치는 %d \n", yPos);*/
										  //printf("썸 위치: %d \n", HIWORD(wParam));

		break;
	}

	}

	if (yPos + yInc < 0) {	//스크롤 위치 필터
		yInc = -yPos;

	}
	if (yPos + yInc > yMax) {
		yInc = yMax - yPos;

	}

	//새로운 썸의 위치 계산
	yPos += yInc;  //새로운 썸의 위치가 여기서 계산된다. 
	return yInc;
	
}




