#include <tchar.h>
#include "mstring.h"


//1. ���� ���ϱ�
int getLen(const TCHAR* inStr) {
	return _tcslen(inStr);
}

//2. subString
TCHAR* subTchar(const TCHAR* inStr, int fromIdx, int toIdx) {
	int heapSize = (toIdx - fromIdx) + 2; //null ���� �������.
	int bufferSize = _tcslen(inStr) * 2 + 2;
	TCHAR* newStr = new TCHAR[bufferSize]; //�����Ҵ���
	_tcscpy_s(newStr, bufferSize, inStr + fromIdx);
	newStr[heapSize - 1] = 0;
	return newStr;
}

//3. cloneTchar
TCHAR* cloneTchar(const TCHAR* source) {
	int bufferSize = _tcslen(source) + 1;
	TCHAR* newStr = new TCHAR[bufferSize];
	_tcscpy_s(newStr, bufferSize, source);
	return newStr;
}

//4. addString
TCHAR* addTchar(const TCHAR* source, const TCHAR* added) {
	int heapSize = _tcslen(source) + _tcslen(added) + 1;
	int bufferSize = heapSize * 2 + 2;
	TCHAR* tempStr = new TCHAR[bufferSize]; //1. �ӽ� �迭 ������ �����. 
	_tcscpy_s(tempStr, bufferSize, source); //2. �ӽ� ������ source�ٿ��ִ´�. //����� �ϳ��� Ŀ����. //2. �� 1Ŀ�� �ϴ����� �����غ���.
	_tcscat_s(tempStr, heapSize, added);
	return tempStr;
}

//5. charToStr 
TCHAR* charToStr(TCHAR inChar) {
	TCHAR* tempStr = new TCHAR[2];
	tempStr[0] = inChar;
	tempStr[1] = 0;
	return tempStr;
}



//6. insert char //inChar�� sourceStr�� targetIdx�� �����ؼ� ���ο� ���ڿ��� �̾��ش�. 
TCHAR* insertChar(const TCHAR* sourceStr, TCHAR inChar, int targetIdx) {
	TCHAR* tmpStr1; //���� �Ҵ� �� �� �κ� ���� ��
	TCHAR* tmpStr2; //�� �κ� ���� ��
	TCHAR* temp3;
	TCHAR* charStr = charToStr(inChar);
	TCHAR* result;

	tmpStr1 = subTchar(sourceStr, 0, targetIdx - 1);
	tmpStr2 = subTchar(sourceStr, targetIdx, _tcslen(sourceStr) - 1);
	temp3 = addTchar(tmpStr1, charStr);
	result = addTchar(temp3, tmpStr2);

	delete tmpStr1;
	delete tmpStr2;
	delete temp3;
	delete charStr;

	return result;
}
