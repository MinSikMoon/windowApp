#ifndef _global_variables_
#define _global_variables_
#include<Windows.h>
#include <vector>
#include <cstdio>
#include "mCaret.h"
#include "lineContainer.h"
#include "project_methods.h"
using namespace std;
//��������======================================================================================


FILE* fp;
TCHAR* testStr;
vector<TCHAR*> v1;
TCHAR* tempStr;
//-------------------------------------------------------------------------------------���� �ʺ�, ����
int wordHeight = 16;
int avgCharWidth2;
int textHeight;
int curScreenLineNum; //���� ȭ�鿡 ���̴� ���� ���μ� 
int g_totalLineCnt; //���� ȭ�鿡 �� �� ���� ��µǴ°�?
int g_upperLineCnt; //���� ������ �ֳ�?
int g_restLineCnt; //�ڽ��� ���� ������ �� ���� ���ҳ�?
map<int, int> nodeLineNum; //��� �ϳ��� �� ������ ������ ������ ��
//-------------------------------------------------------------------------------------��ũ��
int xMax, yMax; //��ũ�� ���� �ִ�ġ ����
int xPos, yPos; //���� ��ġ

//-------------------------------------------------------------------------------------ĳ��
int nodeIdx; //ù ���� �Ҽ� ��� �ε���
int lastNodeIdx; //ȭ�� ������ -----------------
mCaret caret;
lineContainer lc1; //������ ���, ���μ�, ���ڼ�, ù�ε����� ���� Ŭ����
//-------------------------------------------------------------------------------------�Է� ���ڿ�
TCHAR curStr[1024]; //���ڸ� �Է¹޴� �� ����(���� �ֱ�) ����
#endif // !_global_variables_
