
#pragma once
#include <Windows.h>
#include <tchar.h>
#include "mString.h"
#include "mTextPixel.h"


//1. 일단 static으로 autoLineSwitch를 만들어 보자. 
//1. mString이 들어오면 개행해서 출력해준다. //기본 함수
 void autoLineSwitch(HDC hdc, mString& mStr, int screenWidth, int startPos, int wordHeight) {
	if (mStr.getLength() == 0) {
		return;
	}
	int startIdx = 0;
	int lastIdx = 0;
	int loopCnt = 0;
	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // 전체 문장의 픽셀너비
	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // 대강 한글자당 픽셀 너비가 몇인가 예측
	int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //화면에 몇 줄로 출력되나.
																 //printf("mstr글자 갯수는 %d, 줄수는 %d \n", mStr.getLength(), screenSentenceNum);
	int wordCnt = 0;
	while (1) {
		//printf("현재 wordcnt : %d, 목표는 %d \n", wordCnt, mStr.getLength());
		if (wordCnt == mStr.getLength())
			break;

		lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);

		wordCnt += (lastIdx - startIdx + 1);
		textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
		startIdx = lastIdx + 1;
		//printf("현재 loopcnt : %d, lastidx: %d, wordcnt: %d, 다음 startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
		loopCnt++;
	}

}
////2. TCHAR 버젼
//void autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight) {
//	mString mStr(_mStr); //임시 mStr 만든다. 
//
//	if (mStr.getLength() == 0) {
//		return;
//	}
//	int startIdx = 0;
//	int lastIdx = 0;
//	int loopCnt = 0;
//	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // 전체 문장의 픽셀너비
//	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // 대강 한글자당 픽셀 너비가 몇인가 예측
//	int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //화면에 몇 줄로 출력되나.
//																 //printf("mstr글자 갯수는 %d, 줄수는 %d \n", mStr.getLength(), screenSentenceNum);
//	int wordCnt = 0;
//	while (1) {
//		//printf("현재 wordcnt : %d, 목표는 %d \n", wordCnt, mStr.getLength());
//		if (wordCnt == mStr.getLength())
//			break;
//
//		lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);
//
//		wordCnt += (lastIdx - startIdx + 1);
//		textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
//		startIdx = lastIdx + 1;
//		//printf("현재 loopcnt : %d, lastidx: %d, wordcnt: %d, 다음 startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
//		loopCnt++;
//	}
//
//}

//2. TCHAR 버젼 // loop cnt 뱉어주는 신버젼 실험
int autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight) {
	mString mStr(_mStr); //임시 mStr 만든다. 

	if (mStr.getLength() == 0) {
		return 0;
	}
	int startIdx = 0;
	int lastIdx = 0;
	int loopCnt = 0;
	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // 전체 문장의 픽셀너비
	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // 대강 한글자당 픽셀 너비가 몇인가 예측
	int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //화면에 몇 줄로 출력되나.
																 //printf("mstr글자 갯수는 %d, 줄수는 %d \n", mStr.getLength(), screenSentenceNum);
	int wordCnt = 0;
	while (1) {
		//printf("현재 wordcnt : %d, 목표는 %d \n", wordCnt, mStr.getLength());
		if (wordCnt == mStr.getLength())
			break;

		lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);

		wordCnt += (lastIdx - startIdx + 1);
		textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
		startIdx = lastIdx + 1;
		//printf("현재 loopcnt : %d, lastidx: %d, wordcnt: %d, 다음 startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
		loopCnt++;
	}
	return loopCnt;

}