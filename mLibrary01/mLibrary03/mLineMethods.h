
#pragma once
#include <Windows.h>
#include <tchar.h>
#include "mString.h"
#include "mTextPixel.h"
#include "mScreenLineContainer.h"


//1. �ϴ� static���� autoLineSwitch�� ����� ����. 
//1. mString�� ������ �����ؼ� ������ش�. //�⺻ �Լ�
 void autoLineSwitch(HDC hdc, mString& mStr, int screenWidth, int startPos, int wordHeight) {
	if (mStr.getLength() == 0) {
		return;
	}
	int startIdx = 0;
	int lastIdx = 0;
	int loopCnt = 0;
	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // ��ü ������ �ȼ��ʺ�
	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // �밭 �ѱ��ڴ� �ȼ� �ʺ� ���ΰ� ����
	int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //ȭ�鿡 �� �ٷ� ��µǳ�.
																 //printf("mstr���� ������ %d, �ټ��� %d \n", mStr.getLength(), screenSentenceNum);
	int wordCnt = 0;
	while (1) {
		//printf("���� wordcnt : %d, ��ǥ�� %d \n", wordCnt, mStr.getLength());
		if (wordCnt == mStr.getLength())
			break;

		lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);

		wordCnt += (lastIdx - startIdx + 1);
		textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
		startIdx = lastIdx + 1;
		//printf("���� loopcnt : %d, lastidx: %d, wordcnt: %d, ���� startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
		loopCnt++;
	}

}
////2. TCHAR ����
//void autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight) {
//	mString mStr(_mStr); //�ӽ� mStr �����. 
//
//	if (mStr.getLength() == 0) {
//		return;
//	}
//	int startIdx = 0;
//	int lastIdx = 0;
//	int loopCnt = 0;
//	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // ��ü ������ �ȼ��ʺ�
//	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // �밭 �ѱ��ڴ� �ȼ� �ʺ� ���ΰ� ����
//	int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //ȭ�鿡 �� �ٷ� ��µǳ�.
//																 //printf("mstr���� ������ %d, �ټ��� %d \n", mStr.getLength(), screenSentenceNum);
//	int wordCnt = 0;
//	while (1) {
//		//printf("���� wordcnt : %d, ��ǥ�� %d \n", wordCnt, mStr.getLength());
//		if (wordCnt == mStr.getLength())
//			break;
//
//		lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);
//
//		wordCnt += (lastIdx - startIdx + 1);
//		textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
//		startIdx = lastIdx + 1;
//		//printf("���� loopcnt : %d, lastidx: %d, wordcnt: %d, ���� startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
//		loopCnt++;
//	}
//
//}

//2. TCHAR ���� // loop cnt ����ִ� �Ź��� ����
int autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight) {
	mString mStr(_mStr); //�ӽ� mStr �����. 

	if (mStr.getLength() == 0) {
		return 0;
	}
	int startIdx = 0;
	int lastIdx = 0;
	int loopCnt = 0;
	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // ��ü ������ �ȼ��ʺ�
	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // �밭 �ѱ��ڴ� �ȼ� �ʺ� ���ΰ� ����
	int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //ȭ�鿡 �� �ٷ� ��µǳ�.
																 //printf("mstr���� ������ %d, �ټ��� %d \n", mStr.getLength(), screenSentenceNum);
	int wordCnt = 0;
	while (1) {
		//printf("���� wordcnt : %d, ��ǥ�� %d \n", wordCnt, mStr.getLength());
		if (wordCnt == mStr.getLength())
			break;

		lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);

		wordCnt += (lastIdx - startIdx + 1);
		textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
		startIdx = lastIdx + 1;
		//printf("���� loopcnt : %d, lastidx: %d, wordcnt: %d, ���� startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
		loopCnt++;
	}
	return loopCnt;

}

//�׽�Ʈ : autoLineSwitch�� screenLineContainer �־ ���� ������ ����
//screenLineContainer�� nodeIdx�� �־��ָ� node�� ���ο� ���� �������� �Է����ش�. 
int autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight, mScreenLineContainer& screenLineContainer, size_m nodeIdx) {
	mString mStr(_mStr); //�ӽ� mStr �����. 

	if (mStr.getLength() == 0) {
		return 0;
	}
	int startIdx = 0;
	int lastIdx = 0;
	int loopCnt = 0;
	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // ��ü ������ �ȼ��ʺ�
	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // �밭 �ѱ��ڴ� �ȼ� �ʺ� ���ΰ� ����
	int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //ȭ�鿡 �� �ٷ� ��µǳ�.
																 //printf("mstr���� ������ %d, �ټ��� %d \n", mStr.getLength(), screenSentenceNum);
	int wordCnt = 0;
	int tempLineWordCnt;

	//���� ���⼭ nodeIdx�� ���δ� ���ڼ�, ù �ε���, ���ε����� �������� �Ѵ�. 
	while (1) {
		if (wordCnt == mStr.getLength())
			break;

		int tempLineWordCnt = 0;

		lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);

		tempLineWordCnt = (lastIdx - startIdx + 1);
		wordCnt += tempLineWordCnt;
		screenLineContainer.setNodeLineData(nodeIdx, loopCnt, tempLineWordCnt, startIdx, lastIdx);

		textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
		startIdx = lastIdx + 1;
		//printf("���� loopcnt : %d, lastidx: %d, wordcnt: %d, ���� startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
		loopCnt++;
	}
	return loopCnt; //���� ���� �����ϳ��� ��� �������� ��µǰ� �ִ°�
}