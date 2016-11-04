#include "project_methods.h"
#include "lineContainer.h"

//1. ���° ��忡 ����� �������ΰ�? : ��ũ�ѹ��� yPos�� �־��� ���
int getNodeIdx(map<int, int> v, int yPos, int wordHeight, int& out_lineNum) {
	int sum = 0;
	int tempSum = 0;
	int nodeIdx = 0;
	int frontLine = yPos / wordHeight; //�տ� ����� �ִ��� ����. 

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

//1.5 getNodeIdx�� �ι�° ���� �����ε� //�� ��° ����� ���° ����(out_lineNum)���� �̾��ش�. 
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

//2. ������ ����ε����� ���� �˾Ƴ�����. // 
int getLastScreenNodeIdx(map<int, int> nodeLineNum, int totalLineCnt, int upperLineCnt, int restLineCnt, int screenAvailableCnt, int& out_lineNum) {
	//1. ���� : ������ ���α��� ��µǴµ� ȭ�� ũ�Ⱑ �� Ŭ�� //������ ������ ����� ������ ���� ����
	int tempUpperCnt;
	if (upperLineCnt + screenAvailableCnt > totalLineCnt) {
		tempUpperCnt = totalLineCnt - 1;
		return getNodeIdx(nodeLineNum, tempUpperCnt, out_lineNum);
	}
	else { //ȭ�� �ȿ��� �ؽ�Ʈ ������ ���� ȭ�� ������ �ؽ�Ʈ�� �� �������� ��
		tempUpperCnt = upperLineCnt + screenAvailableCnt - 1;
		return getNodeIdx(nodeLineNum, tempUpperCnt, out_lineNum);
	}
}


//2. ȭ�鿡 ���߾� ������� �����ִ� ����
int autoLineSwitch( HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int,int>& nodeLineNum, lineContainer& lc1) {
	int startIdx = 0;
	int lastIdx = 0;
	int sentenceNum;
	unsigned int i;
	int j;
	int loopCnt = 0;
	int avgCharWidth2;
	TCHAR* testStr;

	
	for (i = 0; i < v1.size(); i++) { //��� �ϳ��� ����. 
		testStr = NULL; //testStr ����ֱ�
		testStr = v1[i];
		avgCharWidth2 = strPxWidth(hdc, testStr) / _tcslen(testStr); //�밭�� �ѱ��� �� ���غ���
		sentenceNum = strPxWidth(hdc, testStr) / screenWidth + 1;
		startIdx = 0;
		//��� �� �� ���ΰ� �����ֱ�.
		int nodeLineCnt = 0;

		if (screenWidth < strPxWidth(hdc, testStr)) {
			for (j = 0; j < sentenceNum; j++) {
				lastIdx = getEndIdx(hdc, testStr, screenWidth, startIdx, avgCharWidth2);
				textOutCustom(hdc, 0, loopCnt*wordHeight - yPos, testStr, startIdx, lastIdx);
				//i: ���° ����ΰ�// ���⼭ nodeLineCnt-1�ϸ� ���ȿ��� ���° ������// lastIdx-startIdx+1�ϸ� �� �ٿ� ���� �����
				//startIdx: ������ ù������ �ε����� ���ΰ�?// �ʿ� 1���� �迭�� key ������ ���� �� �ִ��� ����.
				//printf("%d �� ����� %d ��° �����ε� ���̴� %d �Դϴ�. \n", i, j, lastIdx - startIdx + 1); //����� �� ����.
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
		nodeLineNum[i] = nodeLineCnt;	//����� �� ����� �� ������ �����ϴ� ���� ���� ���� ȹ��. 
	}
	
	return loopCnt; //�� ���� ���Ҵ��� ����
}

//3. ��ũ�� ����ġ ���� �и�
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
		int temp_yPos = HIWORD(wParam); //thumb�� ��ġ
		int temp_yInc = (temp_yPos - yPos);
		int temp_lineJump = temp_yInc / wordHeight;
		int temp_jumpMore = temp_yInc % wordHeight;
		int halfCharWidth = wordHeight / 2;

		if (temp_jumpMore >= halfCharWidth) {
			temp_lineJump++;
		}

		yInc = temp_lineJump * wordHeight;/*printf("���� ��ũ�ѹ� ��ġ�� %d \n", yPos);*/
										  //printf("�� ��ġ: %d \n", HIWORD(wParam));

		break;
	}

	}

	if (yPos + yInc < 0) {	//��ũ�� ��ġ ����
		yInc = -yPos;

	}
	if (yPos + yInc > yMax) {
		yInc = yMax - yPos;

	}

	//���ο� ���� ��ġ ���
	yPos += yInc;  //���ο� ���� ��ġ�� ���⼭ ���ȴ�. 
	return yInc;
	
}




