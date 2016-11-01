#include "project_methods.h"

//1. ���° ��忡 ����� �������ΰ�? : ��ũ�ѹ��� yPos�� �־��� ���
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

//2. ȭ�鿡 ���߾� ������� �����ִ� ����
int autoLineSwitch(HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int,int>& nodeLineNum) {
	int startIdx = 0;
	int lastIdx = 0;
	int sentenceNum;
	int i, j;
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
		nodeLineNum[i] = nodeLineCnt;	//����� �� ����� �� ������ �����ϴ� ���� ���� ���� ȹ��. 
	}

	return loopCnt; //�� ���� ���Ҵ��� ����
}




