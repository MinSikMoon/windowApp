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
//1. 몇번째 노드에 저장된 데이터인가? : 스크롤바의 yPos가 주어질 경우
int getNodeIdx(map<int, int> v, int yPos, int wordHeight, int& out_lineNum);

//2. 화면에 맞추어 개행시켜 보여주는 로직
int autoLineSwitch(HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int, int>& nodeLineNum, lineContainer& lc1);

//3. 스크롤 관련 로직
int mScrollSwitches(WPARAM wParam, int wordHeight, int curScreenLineNum, int& yPos, int yMax);
#endif

