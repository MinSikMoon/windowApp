#pragma once
#include "mHeaders.h"
#include "mString.h"
#include "mTextSource.h"

//1. mString을 받아서 픽셀 가로길이를 리턴해주는 함수
size_m getMstrPixelWidth(HDC hdc, mString& mStr, size_m startIdx, size_m endIdx) {
	size_m length = mStr.getLength(); //mStr의 str 길이
	if (startIdx < 0 || endIdx >(length - 1) || startIdx > endIdx) { //필터 for 잘못된 입력
		printf("!!!!!!!!!********>>>>잘못된 범위 지정 in getStrPixelWidth() : sIdx: %d, eIdx: %d \n", startIdx, endIdx);
		system("pause");
		exit(-1); //에러
	}

	SIZE size;
	if (startIdx > endIdx) { //잘못된 케이스 필터용도 
		startIdx = endIdx;
	}

	TCHAR* tempStr = mStr.subFromTo(startIdx, endIdx); //임시 문자열 뽑고.
	GetTextExtentPoint(hdc, tempStr, _tcslen(tempStr), &size); //길이 재보고
	delete tempStr; //임시 문자열 해제

	return size.cx;
}

//2. mString의 str 픽셀 가로 길이 그대로 리턴
size_m getMstrPixelWidthFull(HDC hdc, mString& mStr) {
	SIZE size;
	GetTextExtentPoint(hdc, mStr.getStr(), _tcslen(mStr.getStr()), &size); //길이 재보고
	return size.cx;
}

//3. textOut 커스텀
void textOutCustom(HDC hdc, mString& mStr, size_m startIdx, size_m endIdx, int xPos, int yPos) {
	TCHAR* tempStr = mStr.subFromTo(startIdx, endIdx);
	int tempLength = _tcslen(tempStr);

	TextOut(hdc, xPos, yPos, tempStr, tempLength);
	delete tempStr;
}

//4. 현재 화면에서 한줄에 문장의 몇 인덱스까지 출력이 가능한가?
int getScreenEndIdx(HDC hdc, mString& mStr, int limitWidth, int startIdx, int avgCharWidth, int detailCustom) {
	if (startIdx < 0) {
		printf("====================================> 잘못된 범위 int getScreenEndIdx() startIdx가 0보다 작아짐 : %d \n", startIdx);
		system("pause");
		exit(-1);
	}
	if (getMstrPixelWidth(hdc, mStr, startIdx, mStr.getLength() - 1) <= limitWidth) {
		return (mStr.getLength() - 1);
	}


	//if (getMstrPixelWidthFull(hdc, mStr) <= limitWidth) { //화면이 문장보다 더 길면 그냥 문장의 마지막 인덱스 뽑아주면 되겠지. 
	//	return (mStr.getLength() - 1);
	//}

	//1. 평균 글자 너비를 이용해서 limitWidth 안에 총 몇글자 정도 들어갈지 예상해본다.
	int estWordNum = limitWidth / avgCharWidth + 1;
	if (estWordNum > mStr.getLength()) {
		estWordNum = mStr.getLength();
	}

	//2. 그 정도 글자로 만든 새로운 문자열 만들기
	int tempEndIdx = startIdx + estWordNum - 1;
	//테스트용 삽입
	if (tempEndIdx > (mStr.getLength() - 1))
		tempEndIdx = mStr.getLength() - 1;

	mString tempStr(mStr.subFromTo(startIdx, tempEndIdx)); //임시 mString 하나 만들기
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
		while (1) { //짧으면 한개씩 더해준다. 
			tempEndIdx++;
			tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //새걸로 교체 
			if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth)
				break;
		}
	}

	return tempEndIdx - detailCustom; //하나 빼줘야 화면 바깥으로 안 나가더라.
}

////5. getScreenEndIdx를 응용해서 개행 함수 
//size_m autoLineSwitch(HDC hdc, size_m screenLimitWidth, size_m thumbYpos, size_m wordHeight, mTextSource& textSource, map<int, int>& nodeLineNum, mScreenLineContainer& lc) {
//	int startIdx = 0;
//	int lastIdx = 0;
//	int sentenceNum;
//	unsigned int i, j;
//	int loopCnt = 0;
//	int avgCharWidth;
//	mString tempStr; //빈 문자열 생성
//
//	for (i = 0; i < textSource.getTextNum(); i++) { //문단의 갯수만큼 돈다. 
//		tempStr = 
//
//	}
//
//
//}