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

//2. ȭ�鿡 ���߾� ������� �����ִ� ����
int autoLineSwitch(HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int, int>& nodeLineNum, lineContainer& lc1);

//3. ��ũ�� ���� ����
int mScrollSwitches(WPARAM wParam, int wordHeight, int curScreenLineNum, int& yPos, int yMax);
#endif

