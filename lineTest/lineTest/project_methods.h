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
int getNodeIdx(map<int, int> nodeLineNum, int upperLineCnt, int& out_lineNum);

//2. 마지막 노드 인덱스와 라인 리턴
int getLastScreenNodeIdx(map<int, int> nodeLineNum, int totalLineCnt, int upperLineCnt, int restLineCnt, int screenAvailableCnt, int& out_lineNum);

//3. 화면에 맞추어 개행시켜 보여주는 로직
int autoLineSwitch(HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int, int>& nodeLineNum, lineContainer& lc1);

//4. 스크롤 관련 로직
int mScrollSwitches(WPARAM wParam, int wordHeight, int curScreenLineNum, int& yPos, int yMax);
#endif

