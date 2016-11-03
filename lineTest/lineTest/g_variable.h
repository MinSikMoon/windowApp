#ifndef _global_variables_
#define _global_variables_
#include<Windows.h>
#include <vector>
#include <cstdio>
#include "mCaret.h"
#include "lineContainer.h"
using namespace std;
//전역변수======================================================================================


FILE* fp;
TCHAR* testStr;
vector<TCHAR*> v1;
TCHAR* tempStr;
//-------------------------------------------------------------------------------------글자 너비, 높이
int wordHeight = 16;
int avgCharWidth2;
int textHeight;
int curScreenLineNum; //현재 화면에 보이는 글자 라인수 
int g_totalLineCnt; //현재 화면에 총 몇 줄이 출력되는가?
//-------------------------------------------------------------------------------------스크롤
int xMax, yMax; //스크롤 범위 최대치 지정
int xPos, yPos; //썸의 위치

//-------------------------------------------------------------------------------------캐럿
int nodeIdx; //첫 문장 소속 노드 인덱스
mCaret caret;
lineContainer lc1; //문장의 노드, 라인수, 글자수, 첫인덱스를 담을 클래스
//-------------------------------------------------------------------------------------입력 문자열
TCHAR curStr[1024]; //문자를 입력받는 맨 끝줄(가장 최근) 문장
#endif // !_global_variables_
