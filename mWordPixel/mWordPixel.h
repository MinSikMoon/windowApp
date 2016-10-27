#pragma once
#include <Windows.h>

//���� �Լ���
//1. �� ����� startIdx, endIdx�� ������ pixel �ʺ� ������ �������ִ� �Լ�
int getStrPixelWidth(HDC hdc, const TCHAR* targetStr, int startIdx, int endIdx);

//2 �� ���� pixel���� �������ִ� �Լ�
int strPxWidth(HDC hdc, const TCHAR* targetStr);

//3. textout Ŀ���� //������ ������ textout ���ִ� �Լ� 
void textOutCustom(HDC hdc, int x, int y, const TCHAR* targetStr, int startIdx, int endIdx);


//4. �� ����, ���� ȭ�� pixel�ʺ�(limit�ʺ�), startIdx, ��� �ѱ��� �ʺ�// 4���� ������ endIdx�� ������ �������ִ� �Լ�
int getEndIdx(HDC hdc, const TCHAR* targetStr, int limitWidth, int startIdx, int avgCharWidth);

//5. wchar�� �ٲٱ�
wchar_t* toWC(const char *str_param);