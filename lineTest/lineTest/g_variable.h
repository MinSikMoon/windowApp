#ifndef _global_variables_
#define _global_variables_
#include<Windows.h>
#include <vector>
#include <cstdio>
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

//-------------------------------------------------------------------------------------스크롤
int xMax, yMax; //스크롤 범위 최대치 지정
int xPos, yPos; //썸의 위치

//-------------------------------------------------------------------------------------캐럿
int nodeIdx; //첫 문장 소속 노드 인덱스

#endif // !_global_variables_
