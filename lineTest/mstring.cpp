#include <iostream>
#include <tchar.h>
#include <cstdio>



//1. 길이 구하기
int getLen(const TCHAR* inStr) {
	return _tcslen(inStr);
}

//2. subString
TCHAR* subTchar(const TCHAR* inStr, int fromIdx, int toIdx) {
	int heapSize = (toIdx - fromIdx) + 2; //null 까지 해줘야함.
	TCHAR* newStr = new TCHAR[_tcslen(inStr)*2 + 2]; //공간할당후 //이게 문제 힙 공간 문제
	_tcscpy(newStr, inStr + fromIdx);
	newStr[heapSize - 1] = 0;
	return newStr;

}

//3. cloneTchar
TCHAR* cloneTchar(const TCHAR* source) {
	TCHAR* newStr = new TCHAR[_tcslen(source) + 1];
	_tcscpy(newStr, source);
	return newStr;
}

//4. addString
TCHAR* addTchar(const TCHAR* source, const TCHAR* added) {
	int heapSize = _tcslen(source) + _tcslen(added) + 1;
	TCHAR* newStr = new TCHAR[heapSize]; //공간할당후
	TCHAR* s1 = cloneTchar(source);	//const없는 임시 문자열 만들어주기
	TCHAR* s2 = cloneTchar(added);
	newStr = _tcscat(s1, s2);	//붙이기
	newStr[heapSize - 1] = 0;
	return newStr;
}