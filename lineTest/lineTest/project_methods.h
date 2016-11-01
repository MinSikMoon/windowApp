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
using namespace std;
int getNodeIdx(map<int, int> v, int yPos, int wordHeight);
int autoLineSwitch(HDC hdc, vector<TCHAR*> v1, int screenWidth, int wordHeight, int yPos, map<int, int>& nodeLineNum);
#endif

