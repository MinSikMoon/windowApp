#ifndef _project_methods_
#define _project_methods_
#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

#include "mstring.h"
#include "mWordPixel.h"
#include "lineContainer.h"
using namespace std;
//1. ���° ��忡 ����� �������ΰ�? : ��ũ�ѹ��� yPos�� �־��� ���
int getNodeIdx(map<int, int> v, int yPos, int wordHeight, int& out_lineNum);
int getNodeIdx(map<int, int> nodeLineNum, int upperLineCnt, int& out_lineNum);

//2. ������ ��� �ε����� ���� ����
int getLastScreenNodeIdx(map<int, int> nodeLineNum, int totalLineCnt, int upperLineCnt, int restLineCnt, int screenAvailableCnt, int& out_lineNum);

//3. ȭ�鿡 ���߾� ������� �����ִ� ����
int autoLineSwitch(HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int, int>& nodeLineNum, lineContainer& lc1);

//4. ��ũ�� ���� ����
int mScrollSwitches(WPARAM wParam, int wordHeight, int curScreenLineNum, int& yPos, int yMax);
#endif

