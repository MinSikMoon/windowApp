#include <tchar.h>
#include "mstring.h"


//1. ���� ���ϱ�
int getLen(const TCHAR* inStr) {
	return _tcslen(inStr);
}

//2. subString
TCHAR* subTchar(const TCHAR* inStr, int fromIdx, int toIdx) {
	int heapSize = (toIdx - fromIdx) + 2; //null ���� �������.
	TCHAR* newStr = new TCHAR[heapSize]; //�����Ҵ���
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
	TCHAR* tempStr = new TCHAR[heapSize]; //1. �ӽ� �迭 ������ �����. 
	_tcscpy_s(tempStr, _tcslen(source) + 1, source); //2. �ӽ� ������ source�ٿ��ִ´�. //����� �ϳ��� Ŀ����. //2. �� 1Ŀ�� �ϴ����� �����غ���.
	_tcscat_s(tempStr, heapSize, added);
	return tempStr;
}