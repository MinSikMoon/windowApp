#pragma once
#include <Windows.h>

//내부 함수들
//1. 한 문장과 startIdx, endIdx를 넣으면 pixel 너비가 얼마인지 리턴해주는 함수
int getStrPixelWidth(HDC hdc, const TCHAR* targetStr, int startIdx, int endIdx);

//2 한 문장 pixel길이 리턴해주는 함수
int strPxWidth(HDC hdc, const TCHAR* targetStr);

//3. textout 커스텀 //지정한 곳부터 textout 해주는 함수 
void textOutCustom(HDC hdc, int x, int y, const TCHAR* targetStr, int startIdx, int endIdx);


//4. 한 문장, 현재 화면 pixel너비(limit너비), startIdx, 평균 한글자 너비// 4개를 넣으면 endIdx가 몇인지 가르쳐주는 함수
int getEndIdx(HDC hdc, const TCHAR* targetStr, int limitWidth, int startIdx, int avgCharWidth);

//5. wchar로 바꾸기
wchar_t* toWC(const char *str_param);