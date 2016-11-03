#include <Windows.h>
#include "mstring.h"
#include <iostream>
//���� �Լ���
//1. �� ����� startIdx, endIdx�� ������ pixel �ʺ� ������ �������ִ� �Լ�
int getStrPixelWidth(HDC hdc, const TCHAR* targetStr, int startIdx, int endIdx) {
	SIZE size;
	//�����: startIdx�� endIdx���� Ŀ���� ����printf("=============>start: %d, end: %d \n", startIdx, endIdx);
	if (startIdx > endIdx) {
		startIdx = endIdx;
	}
	TCHAR* tempStr = subTchar(targetStr, startIdx, endIdx);
	GetTextExtentPoint(hdc, tempStr, _tcslen(tempStr), &size);
	delete tempStr;
	return size.cx;
}

//2 �� ���� pixel���� �������ִ� �Լ�
int strPxWidth(HDC hdc, const TCHAR* targetStr) {
	SIZE size;
	GetTextExtentPoint(hdc, targetStr, _tcslen(targetStr), &size);
	return size.cx;
}

//3. textout Ŀ���� //������ ������ textout ���ִ� �Լ� 
void textOutCustom(HDC hdc, int x, int y, const TCHAR* targetStr, int startIdx, int endIdx) {
	TCHAR* tempStr = subTchar(targetStr, startIdx, endIdx);
	TextOut(hdc, x, y, tempStr, _tcslen(tempStr));
	delete tempStr;
}
// TextOut()



//3. �� ����, ���� ȭ�� pixel�ʺ�(limit�ʺ�), startIdx, ��� �ѱ��� �ʺ�// 4���� ������ endIdx�� ������ �������ִ� �Լ�
int getEndIdx(HDC hdc, const TCHAR* targetStr, int limitWidth, int startIdx, int avgCharWidth) {
	if (getStrPixelWidth(hdc, targetStr, startIdx, _tcslen(targetStr) - 1) <= limitWidth) {
		return (_tcslen(targetStr) - 1);
	}

	//1. ��� ���� �ʺ� �̿��ؼ� limitWidth �ȿ� �� ����� ���� ���� �����غ���.
	int estWordNum = limitWidth / avgCharWidth + 1;
	//2. �� ���� ���ڷ� ���� ���ο� ���ڿ� �����
	int tempEndIdx = startIdx + estWordNum - 1;
	TCHAR* tempStr = subTchar(targetStr, startIdx, tempEndIdx);
	//3. ���� limitWidth�� ���ϸ鼭 �����غ���.
	if (strPxWidth(hdc, tempStr) > limitWidth) { //��� ������� �ѱ��ھ� 
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

	return tempEndIdx - 1; //�ϳ� ���ຽ.
}

//5. wchar�� �ٲٱ�
wchar_t* toWC(const char *str_param) {
	size_t convertedChars = 0;
	size_t str_param_len = strlen(str_param) + 1;

	wchar_t *wc = new wchar_t[str_param_len];
	setlocale(LC_ALL, "");
	mbstowcs_s(&convertedChars, wc, str_param_len, str_param, _TRUNCATE);

	return wc;
}