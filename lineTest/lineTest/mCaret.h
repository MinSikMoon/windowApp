#ifndef _mCaret_
#define _mCaret_
#include <iostream>
#include <map>
#include <vector>
#include <tchar.h>
#include <Windows.h>
#include "lineContainer.h"
#include "mWordPixel.h"
using namespace std;

//��� : �ᱹ ���� ���̴� ���� ĳ���ε����� �����̴°�/ ���ڿ� ������ ĳ���ε����� ���� ���� �ε����� ������. 


class mCaret {
private:
	//�Է� ����
	int curNodeIdx; //1. ĳ���� � ��忡 �����ִ°�? (��� �ε���)
	int frontWordIdxInNode; //ĳ�� �տ� ����� �ִ°�? (ĳ�� �ε����� �򰥸��� ����. �̰Ŵ� ö���� �Է¿����� ���ڿ� �ε���-1�� ȣȯ��.
	//��� ���� 
	int upperLineCnt; //ĳ�� ���� �� ���̳� �ֳ�?
	int caretIdx; //ĳ���� strlen(����� ���ڿ�) + �ټ� �� ������ ������. 
	int caretLineIdxInNode; //ĳ���� ���� ���ȿ��� �� ��° ���ο� �ִ���?
	
	//���� �Լ�
	//1. ĳ�� �ε����� ������ ���� ����� �� ��° ���ο� �����ִ��� �������ִ� �Լ�--------------------------- -1����
	int getLineInNode(int caretIdx, map<int,int>& nodeLineNum, lineContainer& lc) {
		int lineCnt = nodeLineNum[curNodeIdx]; //���� ��尡 �� �� ���� �ϴ��� ���´�. 

		for(int i = 0; i < lineCnt; i++) { //���� ����ŭ ���鼭 üũ���ش�. 
			int startIdx; //ĳ���ε����� ���� �ε����� �ӽ�����.
			startIdx = lc.getFirstIdx(curNodeIdx, i) + i; //ĳ�� �ε����̴�! �򰥸��� ����.
			int endIdx;
			endIdx = startIdx + lc.getWordCnt(curNodeIdx, i); //���� ĳ�� �ε������� ���ڼ���ŭ �����ָ� �� ĳ�� �ε���.

			if (startIdx <= caretIdx && caretIdx <= endIdx) //���� �ȿ� ���� �ش� ���� �ε��� ����
				return i;
		}

		//�������� 0���� �غ���.// ���� -1�̾���. �̰� ���� ���� �ɱ��
		return 0; 
	}

	//2. ĳ���� ���� ����� �� ��° ���ο� �ִ��� �˾����Ƿ�, ��ü �ؽ�Ʈ�������� ���� �� �� �ִ��� ���Ѵ�. 
	int getUpperLineCnt(int caretLineIdxInNode, map<int, int>& nodeLineNum) {
		//1. curNode������ ���Ѵ�. 
		int sum = 0;
		for (int i = 0; i < curNodeIdx; i++) {
			sum += nodeLineNum[i];
		}
		sum += caretLineIdxInNode; //�� ���� �� ���ϰ� �� �� �����ָ� ĳ������ �� �� �ִ��� ����.
		return sum;
	}
	
	//3. ���� ĳ���ε����� � ���� ���ڿ� �ε����� ������ ��ġ�°�? ------------------------------------- -1 ����
	int getRealIdx(int caretIdx, lineContainer& lc) {
		if (caretIdx == 0)
			return -2; //0�̸� -2�� �����غ���. **********************************���� �߻� ����

		int startIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode) + caretLineIdxInNode; //���� ĳ���� ���� ������ ���� ĳ���ε���
		if (caretIdx == startIdx) { //������ ���� ĳ�� �ε������ .
			return caretIdx - caretLineIdxInNode - 2;
		}
		else {
			return caretIdx - caretLineIdxInNode - 1;
		}
	}

	//4. ����� ������ ĳ�� �ε����� �����ִ� �Լ�
	int getLastCaretIdx(int nodeIdx, lineContainer& lc, map<int, int>& nodeLineNum) {
		//1. ��尡 �� �� �����ϴ��� ���غ���.
		int lineCnt = nodeLineNum[nodeIdx];
		//2. lineCnt-1 �� �ε����� ������ �ε����� lineCnt ��ŭ �����ָ� ������ ĳ�� �ε����� �ȴ�. 
		return lc.getLastIdx(nodeIdx, lineCnt - 1) + lineCnt;
	}

	//5. ���� ĳ���ε����� ������ ù���ΰ�? �����ε� ����
	bool isLineFirst(lineContainer& lc, map<int, int>& nodeLineNum) {
		int startIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode) + caretLineIdxInNode;
		if (caretIdx == startIdx || caretIdx == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	//6. ���� ���,������ ù��° ĳ���ε����� �����ΰ�
	int getFirstCaretIdx(int nodeIdx, int lineIdx, lineContainer& lc) {
		return lc.getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	}
	//7. ���� ���, ������ ������ ĳ���ε����� �����ΰ�
	int getLastCaretIdx(int nodeIdx, int lineIdx, lineContainer& lc) {
		int tempFirstIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc); //������ ù ĳ���ε���
		int tempWordCnt = lc.getLastIdx(nodeIdx, lineIdx) - lc.getFirstIdx(nodeIdx, lineIdx) + 1;
		return tempFirstIdx + tempWordCnt;
	}


	// �����ε� //ĳ�� �ε����� ��� �ε���, ������ �ε����� �־����� ���ڷ� �ѱ� ĳ���ε����� ������ ù��° ���� �Ǵ�
	bool isLineFirst(int caretIdx,int nodeIdx, int lineIdx, lineContainer& lc, map<int, int>& nodeLineNum) {
		int startIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc);
		if (caretIdx == startIdx || caretIdx == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	

	//6. ���� ���忡�� �ٸ� �ѹ������� �Ѿ�� � ĳ���ε����� ���� �Ǵ��� ����������.
	//�� ���, �� ���� �� �˸� -> ù �ε����� ���ε����� �˰� -> ���̸� �ȴ�. -> ���ϸ� �����ؼ� ĳ���ε����� ������.
	//6.1�� ���, �� ������ ������ � ĳ���ε����� �������� �� ĳ���ε����� ���ΰ�?
	//6.2 � ����� � ������ � ĳ���ε����� ������ x�ȼ� ���� ���ΰ�

	
	//////////////////////////////////////////////////////////////////////////////////////////����
	////6. ���� ���,������ ù��° ĳ���ε����� �����ΰ�
	//int getFirstCaretIdx(int nodeIdx, int lineIdx, lineContainer& lc) {
	//	return lc.getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	//}

	//// �����ε� //ĳ�� �ε����� ��� �ε���, ������ �ε����� �־����� ���ڷ� �ѱ� ĳ���ε����� ������ ù��° ���� �Ǵ�
	//bool isLineFirst(int caretIdx, int nodeIdx, int lineIdx, lineContainer& lc, map<int, int>& nodeLineNum) {
	//	int startIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc);
	//	if (caretIdx == startIdx || caretIdx == 0) {
	//		return true;
	//	}
	//	else {
	//		return false;
	//	}
	//}
	////5. getXpixel //ĳ���ε���, ��� �ε���, ������ �ε����� �־����� xpixel���� ���Ѵ�. 
	//int getXpixel(HDC hdc, int caretIdx, int nodeIdx, int lineIdx, vector<TCHAR*>& v1, lineContainer& lc, map<int, int>& nodeLineNum) {
	//	int result;
	//	//������ ù ĳ���ε����̸� ������ 0�ȼ� ����,
	//	if (isLineFirst(caretIdx, nodeIdx, lineIdx, lc, nodeLineNum)) {
	//		result = 0;
	//	}
	//	else { //�ƴϸ� 
	//		int tempStartIdx = lc.getFirstIdx(nodeIdx, lineIdx); //ù��° ���� �ε����� �˾Ƴ���.
	//		int tempRealIdx = getRealIdx(caretIdx, lc); //���� ĳ���� ������ ������ ��ġ�� ���� �ε��� 
	//		result = getStrPixelWidth(hdc, v1[nodeIdx], tempStartIdx, tempRealIdx); //���̸� �˾Ƴ���. 
	//	}
	//	return result;
	//}
	//
	
	

	//final test: 1. Ÿ�� ����/ 2. ���� �ε���/ 3. ���� �ʺ�/ 4. �Ѱ� pixel �ʺ�
	//4���� ������ �Ѱ� �ȼ������ ���� ������ �̾��ش�. 
	//getendidx�� �������� ȭ�� ����� �ƴ� ���ڿ��� �ȼ� ���̰� ������ �ȴٴ� �� ��
	//target���� v1[i] ó�� ��°���� ���ڿ��� ����. 
	//startidx���� ������ ù �ε����� ������.
	int getFitWordNum(HDC hdc, TCHAR* targetStr, int limitPixelWidth, int startIdx, int avgCharWidth) {
		
		if (getStrPixelWidth(hdc, targetStr, startIdx, _tcslen(targetStr) - 1) <= limitPixelWidth) {
			int tempIdx;
			tempIdx = (_tcslen(targetStr) - 1);
			return tempIdx-startIdx + 1; //������ ó������ ������ ������ ������ ���� 
		}
		//1. ��� ���� �ʺ� �̿��ؼ� limitWidth �ȿ� �� ����� ���� ���� �����غ���.
		int estWordNum = limitPixelWidth / avgCharWidth + 1;
		//2. �� ���� ���ڷ� ���� ���ο� ���ڿ� �����
		int tempEndIdx = startIdx + estWordNum - 1;
		TCHAR* tempStr = subTchar(targetStr, startIdx, tempEndIdx);
		//3. ���� limitWidth�� ���ϸ鼭 �����غ���.
		if (strPxWidth(hdc, tempStr) > limitPixelWidth) { //��� ������� �ѱ��ھ� 
			while (1) {
				tempEndIdx--;
				TCHAR* cleaner = tempStr;
				tempStr = subTchar(targetStr, startIdx, tempEndIdx);
				delete cleaner;
				if (strPxWidth(hdc, tempStr) <= limitPixelWidth)
					break;
			}
		}
		else {
			while (1) {
				tempEndIdx++;
				TCHAR* cleaner = tempStr;
				tempStr = subTchar(targetStr, startIdx, tempEndIdx);
				delete cleaner;
				if (strPxWidth(hdc, tempStr) >= limitPixelWidth)
					break;
			}
		}
		delete tempStr; //������ ����
		return tempEndIdx - startIdx + 1; //������ ��������.
	}


	/////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////



	void setCaretLineIdxInNode(int n) {
		caretLineIdxInNode = n;
	}

	void setUpperLineCnt(int n) {
		upperLineCnt = n;
	}

	void setFrontWordIdxInNode(int n) {
		frontWordIdxInNode = n;
	}

public:
	//1. �⺻ ������
	mCaret() : curNodeIdx(0), frontWordIdxInNode(0), upperLineCnt(0), caretIdx(0), caretLineIdxInNode(0){}
	
	//2. setters //�ᱹ �ۿ����� ĳ�� �ε����� ��常 �������ָ� �ȿ��� �ڵ����� ���ְԲ� �Ѵ�. 
	/*void setCurNodeIdx(int n){
		curNodeIdx = n;
	}*/ //���� �ʿ���µ�
	void setCaretIdx(int n, map<int, int>& nodeLineNum,lineContainer& lc) {
		caretIdx = n; //ĳ���� �����Ǹ� frontWordCnt�� �Ǿ�� ����.
		setCaretLineIdxInNode(getLineInNode(caretIdx, nodeLineNum, lc)); //���� ��� �ȿ��� �� �پ� ���� ����.
		setFrontWordIdxInNode(getRealIdx(caretIdx, lc)); //ĳ���� ������ ��ġ�� ���� �ε��� ����
		setUpperLineCnt(getUpperLineCnt(caretLineIdxInNode, nodeLineNum)); //���� �������� ����
	}
	//3. ĳ���� �̵�
	//3.1 ����
	void moveLeft(lineContainer& lc, map<int, int>& nodeLineNum) {
		int tempCaretIdx = caretIdx - 1; //�������� �ѹ� �Űܺ���. 
		if (tempCaretIdx < 0) { // x==0 ��ġ���� ���� ȭ��ǥ�� ������. 
			if (curNodeIdx == 0) { //ù �����
				tempCaretIdx = 0; //�� �̻� �� �����̰�.
			}
			else { //ù ��尡 �ƴ϶�� 
				curNodeIdx--; //��带 �ϳ� �������� �Ű��ش�. 
				tempCaretIdx = getLastCaretIdx(curNodeIdx, lc, nodeLineNum); //���� ����� ������ ĳ�� �ε����� �Ű��ش�. //����� ������ ĳ�� ��ġ�� �ƴ� �Լ��� ������. 
			}
		}
		else { // x==0��ġ�� �ƴ� ������ �����ٸ� �״�� tempcaretidx�� caretidx�� �ǰ���.

		}
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc); //�ڵ����� ������ ������ �������.
	}

	//3.2 ������ //�� ��尡 ������� �˾ƾ� �ϱ⿡ v1�� �� ����Ѵ�. 
	void moveRight(lineContainer& lc, map<int, int>& nodeLineNum, vector<TCHAR*> v1) {
		int tempCaretIdx;
		//1. ĳ���� ������ ĳ�� �ε������ 
		if (caretIdx == getLastCaretIdx(curNodeIdx, lc, nodeLineNum)) {
			if ( curNodeIdx == (v1.size()-1) ) { //1.1 �ٵ� ������ ����� 
				tempCaretIdx = caretIdx; //���� ���� ����
			}
			else { //������ ��尡 �ƴϸ� ���� ����� ù��° ĳ�� �ε����� �Ű� ��߰���.
				curNodeIdx++;
				tempCaretIdx = 0;
			}
		}
		else { //�̵� ���� �ƴϸ� �׳� +1���ָ� �ǰ���.
			tempCaretIdx = caretIdx + 1;
		}
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc);
	}
	
	//����� ������ ������ �������� ������ �ִ� ��
	int getLastLineIdxInNode(int nodeIdx, map<int, int>& nodeLineNum) {
		return nodeLineNum[nodeIdx] - 1;
	}
	
	//3.4 ��
	void moveUp(HDC hdc, map<int,int>& nodeLineNum, lineContainer& lc, vector<TCHAR*>& v1, int avgCharWidth) {
		int tempCaretIdx;
		int tempNodeIdx;
		int tempLineIdx;
		int tempCurXwidth;
		int tempPrevXwidth;

		//1. ��ü �ؽ�Ʈ�� ù��° ���̸� �ƹ��ϵ� �Ͼ�� �ʴ´�.
		if (curNodeIdx == 0 && caretLineIdxInNode == 0) {
			return;
		}
		else { //ù°���� �ƴϸ� ��尡 �̵��� ���̳� �ƴϳķ� �� ������. ----------------- ��尡 �ϳ� ������ ����. 
			if (caretLineIdxInNode == 0) { //ù°���̶� ��尡 �̵��ϴ� ��� //�̾ȿ����� ù° ĳ���̳� �ƴϳķ� ��������.
				tempNodeIdx = curNodeIdx - 1; //��尡 ���� ���� �̵� �Ѵ�. 

				if (isLineFirst(lc, nodeLineNum)) { //ù°ĳ��? //����� �������� ù° ĳ������ �� ��
					tempLineIdx = getLastLineIdxInNode(tempNodeIdx, nodeLineNum); //���� ����� ������ ����
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc); //���� ��� ������ ������ ù° ĳ��
				}
				else { //ù° ĳ���� �ƴϸ� ���� ������ ���ذ��鼭 �Űܺ���. // getfitwordnum �׽�Ʈ
					//1. ���� ĳ���� ��ġ�ϴ� �������� xpixel�� ���Ѵ�. �̰� limit width�� �� ���̴�. 
					tempLineIdx = getLastLineIdxInNode(tempNodeIdx, nodeLineNum); //���� ����� ������ ����
					
					int tempPrevStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx); //���������� ù°���� �ε���
					int tempPrevEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //���� ������ ������ ���� �ε���
					tempCurXwidth = getXpixel(hdc, v1, lc, nodeLineNum); //���� ĳ���� �ִ� ���� xpixel ������. //limit width�� �ȴ�.
					tempPrevXwidth = getStrPixelWidth(hdc, v1.at(tempNodeIdx), tempPrevStartIdx, tempPrevEndIdx); //���� ������ ���̸� �������� ����.

					if (tempPrevXwidth < tempCurXwidth) { //���� ������ ���̰�  ���� ĳ���� �ִ� ���庸�� ª�ٸ� ���� ������ ������ ĳ������ 
						tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
					}
					else { //�ƴϰ� ���� ������ ���� ���庸�� ���̰� ���Ŵ� ��ٸ� 
						int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempPrevStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //���� ���� ù° �ε������� ���� �����ظ�ŭ
					}

				}
			}
			else { //--------------------------------------------------���� �ٲ��� �ʴ´�. //���ε� ������
				tempNodeIdx = curNodeIdx; //���� �ٲ��� �ʰ���.
				tempLineIdx = caretLineIdxInNode - 1; //���� ���ȿ��� ���� ���� // ���⼭ ���� ������.

				if (isLineFirst(lc, nodeLineNum)) { //ù°ĳ��?
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc); //���� ��� ������ ������ ù° ĳ��
				}
				else {
					int tempPrevStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx); //���������� ù°���� �ε���
					int tempPrevEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //���� ������ ������ ���� �ε���
					tempCurXwidth = getXpixel(hdc, v1, lc, nodeLineNum); //���� ĳ���� �ִ� ���� xpixel ������. //limit width�� �ȴ�.
					tempPrevXwidth = getStrPixelWidth(hdc, v1.at(tempNodeIdx), tempPrevStartIdx, tempPrevEndIdx); //���� ������ ���̸� �������� ����.

					if (tempPrevXwidth < tempCurXwidth) { //���� ������ ���̰�  ���� ĳ���� �ִ� ���庸�� ª�ٸ� ���� ������ ������ ĳ������ 
						tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
					}
					else { //�ƴϰ� ���� ������ ���� ���庸�� ���̰� ���Ŵ� ��ٸ� 
						int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempPrevStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //���� ���� ù° �ε������� ���� �����ظ�ŭ
					}
				}
			}
			
		}
		curNodeIdx = tempNodeIdx;
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc);
	}
	//===========================================================================================================
	//���� ������ ������ �װ� �ش� ����� ������ �������� �������ִ� �Լ�
	bool isLastLine(map<int, int>& nodeLineNum, int NodeIdx, int lineIdx) {
		int temp = nodeLineNum[NodeIdx] - 1;

		if (lineIdx == temp) {
			return true;
		}
		else {
			return false;
		}
	}

	//3.5 �� // �Ʒ��� �����̱�
	void moveDown(HDC hdc, map<int, int>& nodeLineNum, lineContainer& lc, vector<TCHAR*>& v1, int avgCharWidth) {
		int tempCaretIdx;
		int tempNodeIdx;
		int tempLineIdx;
		int tempCurXwidth;
		int tempNextXwidth;
		
		int tempLastNodeIdx = v1.size() - 1; //������ ��� �����ϴ� ���� //�� 5, -1�� ���ñ�. //�ϳ� �������. 
		int tempLastLineIdx = getLastLineIdxInNode(tempLastNodeIdx, nodeLineNum); //������ ����
		
		printf("������ ����ε����� ==> %d , ������ %d \n", tempLastNodeIdx, tempLastLineIdx);
		//1. ��ü �ؽ�Ʈ�� ������ ���, ������ �����̸� �ƹ��ϵ� �Ͼ�� �ʴ´�. 
		if (curNodeIdx == tempLastNodeIdx && caretLineIdxInNode == tempLastLineIdx) {
			return;
		}
		else { //1.1 ��尡 �̵��Ѵ�.
			if (isLastLine(nodeLineNum, curNodeIdx, caretLineIdxInNode)) {//ĳ���� �ش� ����� ������ �ٿ� �ִ°��� ������ �����ؾ� �ǳ�. // lineNodeNum[curNodeIdx] - 1 �ϸ� ����.
				tempNodeIdx = curNodeIdx + 1; //���� ���� �� ��
				tempLineIdx = 0; //��尡 �ö󰡸� ���� ���� ������ �ε����� 0�̴ϱ�. 

				//1.1.1 �ش� ������ ù° ĳ���̸� ���� ��� ù°���� ù° ĳ������(0) �� �� 
				if (isLineFirst(lc, nodeLineNum)) {
					tempCaretIdx = 0;
				}
				else { //1.1.2 ������ ù° ĳ���� �ƴ϶�� ������ ���ϸ鼭 �ű��. 
								
					int tempNextStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx) ;
					int tempNextEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //tempLineIdx�� 0 �̰���.

					tempCurXwidth = getXpixel(hdc, v1, lc, nodeLineNum);// ���� ĳ���� �ִ� ���� xpixel // limit width�� �ȴ�. 
					tempNextXwidth = getStrPixelWidth(hdc, v1.at(tempNodeIdx), tempNextStartIdx, tempNextEndIdx); //���� ���� ���̸� �ӽ÷� ����.
					//1.1.2.1 ���� ���� ���̰� ���� ĳ���ִ� ���庸�� ª���� �׳� ������ ĳ������ �� ��
					if (tempNextXwidth < tempCurXwidth) {
						tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
					}
					else { //1.1.2.2 ���� ������ ���� ���庸�� ���̰� ���ų� ��ٸ� 
						int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempNextStartIdx, avgCharWidth);
						tempCaretIdx = tempCnt; //0 + tempCnt // ���� ����� ������ ĳ���ε����� 0���� �������ݾ�. 
					}
				}
			}
			else { //1.2 ��尡 �̵����� �ʴ´�. 
				tempNodeIdx = curNodeIdx; //��尡 �̵����� �ʴ´�. 
				tempLineIdx = caretLineIdxInNode + 1;

				int tempNextStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx);
				int tempNextEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx);
				
				//1.1.1 �ش� ������ ù° ĳ���̸� ���� ������ ù° ĳ������ �� �� 
				if (isLineFirst(lc, nodeLineNum)) {
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc);
				}
				else { //1.1.2 ������ ù° ĳ���� �ƴ϶�� ������ ���ϸ鼭 �ű��. 
					int tempNextStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx);
					int tempNextEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //tempLineIdx�� 0 �̰���.

					tempCurXwidth = getXpixel(hdc, v1, lc, nodeLineNum);// ���� ĳ���� �ִ� ���� xpixel // limit width�� �ȴ�. 
					tempNextXwidth = getStrPixelWidth(hdc, v1.at(tempNodeIdx), tempNextStartIdx, tempNextEndIdx); //���� ���� ���̸� �ӽ÷� ����.
					//1.1.2.1 ���� ���� ���̰� ���� ĳ���ִ� ���庸�� ª���� �׳� ������ ĳ������ �� ��
					if (tempNextXwidth < tempCurXwidth) {
						tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
					}
					else { //1.1.2.2 ���� ������ ���� ���庸�� ���̰� ���ų� ��ٸ� 
						int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempNextStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //0 + tempCnt // ���� ����� ������ ĳ���ε����� 0���� �������ݾ�. 
					}
				}
				//if (tempNextXwidth < tempCurXwidth) {
				//	tempCaretIdx = getLastCaretIdx(tempNodeIdx, tempLineIdx, lc);
				//}
				//else { //1.2.2.2 ���� ������ ���� ���庸�� ���̰� ���ų� ��ٸ� 
				//	int tempCnt = getFitWordNum(hdc, v1.at(tempNodeIdx), tempCurXwidth, tempNextStartIdx, avgCharWidth);
				//	tempCaretIdx = tempCnt; //0 + tempCnt // ���� ����� ������ ĳ���ε����� 0���� �������ݾ�. 
				//}


			}
		}
		curNodeIdx = tempNodeIdx;
		caretIdx = tempCaretIdx;
		setCaretIdx(caretIdx, nodeLineNum, lc);
	}
















	//4. ĳ���� ���

	//5. getXpixel //���� ĳ���� x�ȼ� �� ����, 
	int getXpixel(HDC hdc, vector<TCHAR*>& v1, lineContainer& lc, map<int,int>& nodeLineNum) {
		int result;
		//�ȼ��� ������ ù ���̸� ������ 0
		if (isLineFirst(lc, nodeLineNum)) {
			result = 0;
		}
		else { //�ƴϸ� 
			int tempStartIdx = lc.getFirstIdx(curNodeIdx, caretLineIdxInNode); //ù��° �ε����� �˾Ƴ���.
			result = getStrPixelWidth(hdc, v1[curNodeIdx], tempStartIdx, frontWordIdxInNode); //���̸� �˾Ƴ���. 
		}
		return result;
	}

	//6. getYpixel
	int getYpixel(HDC hdc, int wordHeight) {
		return wordHeight*upperLineCnt;
	}


	//7. getters
	int getCurNodeIdx() {
		return curNodeIdx;
	}
	
	int getRealIdx() {
		return frontWordIdxInNode;
	}
	
};
#endif // !_mCaret_
