#include "mHeaders.h"
#include "mTextSource.h"
#include "mScreenLineContainer.h"
#pragma once
class mScreenTextController {
	//0. ���� ��ü�� ������ �ִ� Ŭ����
	mTextSource textSource;
	map<int, int> nodeLineNum; //���� ������� ������ �ִ��� ������ ������ �ִ� ��
	mScreenLineContainer lc; //���� ���� ���� �ε����� ������ ���� textSource�� ������ ���ڿ� �ε����� ������ �ε���, ���ڼ��� �˼�����.

							 //1. ��ü ������ ���� ������ 
	size_m wordPixelHeight; // ������ ���̴� �ȼ��� ���ΰ�?
	size_m totalPixelHeight; // ��ü �ؽ�Ʈ�� ���̴� �ȼ��� ���ΰ�? 
	size_m totalLineNum; // ��ü �ؽ�Ʈ�� �� ���ΰ�? 
	size_m upperLineNum; // ȭ�� ���� �߸� ���� �� ���ΰ�?
	size_m restLineNum; // ���� ȭ�鿡 ���̴� �� + ���� ���� ���� totalLineNum - upperLineNum

						//2. ȭ�鿡 ��µǴ� ù��° ����� ȭ�� �ǳ��� ������ ���� ���� �ش� ��� �ȿ����� ���� �ε���
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;



};