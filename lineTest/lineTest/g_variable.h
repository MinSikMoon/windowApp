#ifndef _global_variables_
#define _global_variables_
#include<Windows.h>
#include <vector>
#include <cstdio>
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

//-------------------------------------------------------------------------------------��ũ��
int xMax, yMax; //��ũ�� ���� �ִ�ġ ����
int xPos, yPos; //���� ��ġ

//-------------------------------------------------------------------------------------ĳ��
int nodeIdx; //ù ���� �Ҽ� ��� �ε���

#endif // !_global_variables_
