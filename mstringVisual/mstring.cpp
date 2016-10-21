#include <tchar.h>
#include "mstring.h"


//1. 길이 구하기
int getLen(const TCHAR* inStr) {
	return _tcslen(inStr);
}

//2. subString
TCHAR* subTchar(const TCHAR* inStr, int fromIdx, int toIdx) {
	int heapSize = (toIdx - fromIdx) + 2; //null 까지 해줘야함.
	TCHAR* newStr = new TCHAR[heapSize]; //공간할당후
	_tcscpy_s(newStr, _tcslen(inStr), inStr + fromIdx);
	newStr[heapSize - 1] = 0;
	return newStr;
}

//3. cloneTchar
TCHAR* cloneTchar(const TCHAR* source) {
	TCHAR* newStr = new TCHAR[_tcslen(source) + 1];
	_tcscpy_s(newStr, _tcslen(source) + 1, source);
	return newStr;
}

//4. addString
TCHAR* addTchar(const TCHAR* source, const TCHAR* added) {
	int heapSize = _tcslen(source) + _tcslen(added) + 1;
	TCHAR* tempStr = new TCHAR[heapSize]; //1. 임시 배열 공간을 만든다. 
	_tcscpy_s(tempStr, _tcslen(source) + 1, source); //2. 임시 공간에 source붙여넣는다. //사이즈가 하나더 커야함. //2. 왜 1커야 하는지는 조사해볼것.
	_tcscat_s(tempStr, heapSize, added);
	return tempStr;
}