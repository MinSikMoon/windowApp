#include <Windows.h>
#include "mstring.h"
#include <iostream>
//내부 함수들
//1. 한 문장과 startIdx, endIdx를 넣으면 pixel 너비가 얼마인지 리턴해주는 함수
int getStrPixelWidth(HDC hdc, const TCHAR* targetStr, int startIdx, int endIdx) {
	SIZE size;
	//디버그: startIdx가 endIdx보다 커지는 문제printf("=============>start: %d, end: %d \n", startIdx, endIdx);
	if (startIdx > endIdx) {
		startIdx = endIdx;
	}
	TCHAR* tempStr = subTchar(targetStr, startIdx, endIdx);
	GetTextExtentPoint(hdc, tempStr, _tcslen(tempStr), &size);
	delete tempStr;
	return size.cx;
}

//2 한 문장 pixel길이 리턴해주는 함수
int strPxWidth(HDC hdc, const TCHAR* targetStr) {
	SIZE size;
	GetTextExtentPoint(hdc, targetStr, _tcslen(targetStr), &size);
	return size.cx;
}

//3. textout 커스텀 //지정한 곳부터 textout 해주는 함수 
void textOutCustom(HDC hdc, int x, int y, const TCHAR* targetStr, int startIdx, int endIdx) {
	TCHAR* tempStr = subTchar(targetStr, startIdx, endIdx);
	TextOut(hdc, x, y, tempStr, _tcslen(tempStr));
	delete tempStr;
}
// TextOut()



//3. 한 문장, 현재 화면 pixel너비(limit너비), startIdx, 평균 한글자 너비// 4개를 넣으면 endIdx가 몇인지 가르쳐주는 함수
int getEndIdx(HDC hdc, const TCHAR* targetStr, int limitWidth, int startIdx, int avgCharWidth) {
	if (getStrPixelWidth(hdc, targetStr, startIdx, _tcslen(targetStr) - 1) <= limitWidth) {
		return (_tcslen(targetStr) - 1);
	}

	//1. 평균 글자 너비를 이용해서 limitWidth 안에 총 몇글자 정도 들어갈지 예상해본다.
	int estWordNum = limitWidth / avgCharWidth + 1;
	//2. 그 정도 글자로 만든 새로운 문자열 만들기
	int tempEndIdx = startIdx + estWordNum - 1;
	TCHAR* tempStr = subTchar(targetStr, startIdx, tempEndIdx);
	//3. 실제 limitWidth와 비교하면서 가감해본다.
	if (strPxWidth(hdc, tempStr) > limitWidth) { //길면 빼줘야지 한글자씩 
												 //tempEndIdx--;
		while (1) {
			tempEndIdx--;
			TCHAR* cleaner = tempStr;
			tempStr = subTchar(targetStr, startIdx, tempEndIdx);
			delete cleaner;
			if (strPxWidth(hdc, tempStr) <= limitWidth)
				break;
		}
	}
	else {
		while (1) {
			tempEndIdx++;
			TCHAR* cleaner = tempStr;
			tempStr = subTchar(targetStr, startIdx, tempEndIdx);
			delete cleaner;
			if (strPxWidth(hdc, tempStr) >= limitWidth)
				break;
		}
	}
	delete tempStr;

	return tempEndIdx - 1; //하나 빼줘봄.
}

//5. wchar로 바꾸기
wchar_t* toWC(const char *str_param) {
	size_t convertedChars = 0;
	size_t str_param_len = strlen(str_param) + 1;

	wchar_t *wc = new wchar_t[str_param_len];
	setlocale(LC_ALL, "");
	mbstowcs_s(&convertedChars, wc, str_param_len, str_param, _TRUNCATE);

	return wc;
}