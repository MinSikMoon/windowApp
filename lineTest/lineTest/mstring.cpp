#include <iostream>
#include <tchar.h>
#include <cstdio>



//1. ���� ���ϱ�
int getLen(const TCHAR* inStr) {
	return _tcslen(inStr);
}

//2. subString
TCHAR* subTchar(const TCHAR* inStr, int fromIdx, int toIdx) {
	int heapSize = (toIdx - fromIdx) + 2; //null ���� �������.
	TCHAR* newStr = new TCHAR[_tcslen(inStr)*2 + 2]; //�����Ҵ��� //�̰� ���� �� ���� ����
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
	TCHAR* newStr = new TCHAR[heapSize]; //�����Ҵ���
	TCHAR* s1 = cloneTchar(source);	//const���� �ӽ� ���ڿ� ������ֱ�
	TCHAR* s2 = cloneTchar(added);
	newStr = _tcscat(s1, s2);	//���̱�
	newStr[heapSize - 1] = 0;
	return newStr;
}