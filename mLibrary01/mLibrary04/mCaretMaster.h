#pragma once

#include "mScreenLineContainer.h"


class mCaretMaster {
private:
	size_m caretIdx; //ĳ���� �ε��� 
	size_m caretNodeIdx;//2, 3 ĳ���� �Ҽӵ� ����Ʈ�� ��� �ε���// ��� �ȿ����� �����ε���
	size_m caretLineIdxInNode;
	size_m frontWordNumInNode;//4. ĳ�� �տ� �� ���� �ִ°�? in node
	size_m upperLineNum;//5. ĳ�� ���� �� �� �ִ°�?//===================================
	//---------------------------------------------
	size_m xPixel, yPixel;//6.,7 ĳ���� x,y �ȼ� ��ǥ //upperLineNum, textSource, lc �ϼ� // �� 5���� �߿��� ������ 
	size_m width, height;//8. ĳ���� y������ = ������ ũ��



	//private �Լ�
	//1. ĳ�� �ε����� ������ ���� ����� �� ��° ���ο� �����ִ��� �������ִ� �Լ�
	size_m getCaretLineIdxInNode(size_m caretIdx, size_m caretNodeIdx, mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(caretNodeIdx); //���� ĳ���Ҽ� ���� ���� �����ϴ°�
		
		for (int i = 0; i < curNodeLineNum; i++) {
			int startCaretIdx = lc.getFirstIdx(caretNodeIdx, i) + i;
			int endCaretIdx = lc.getWordCnt(caretNodeIdx, i) + startCaretIdx;
		
			if (startCaretIdx <= caretIdx && caretIdx <= endCaretIdx) {
				return  i;
			}
		}
		//printf("getCaretLineIdxInNod = -1 \n");
		return 0; //���� �����ϱ� �ѵ� �ϴ� �غ���. 
	}

	size_m getUpperLineNum(size_m caretNodeIdx, size_m LineIdxInNode, mScreenLineContainer& lc) {
		int sum = 0;
		for (int i = 0; i < caretNodeIdx; i++) {
			sum += lc.getNodeLineNumInfo(i); //���ϰ��� �ϴ� ĳ����� ���������� ��� ������ ��� �����ش�. 
		}
		sum += LineIdxInNode; //ĳ���� ���� �����ε����� �����ָ� ĳ������ ��� ������ �ִ��� sum�� ����ȴ�. 
		return sum;
	}
	
	size_m getFrontWordNumInNode(size_m caretIdx, size_m caretNodeIdx, size_m LineIdxInNode, mScreenLineContainer& lc) {
		if (caretIdx == 0) {
			return 0;
		}

		int startIdx = lc.getFirstIdx(caretNodeIdx, LineIdxInNode) + LineIdxInNode; //�Ű����� ĳ���� ���� ������ ���� ĳ�� �ε���
		
		//printf("���� ĳ���ε��� %d, ĳ�� ��� �ε��� %d, ĳ�� �����ε��� %d, �� ������ ���ڼ� %d \n", caretIdx, caretNodeIdx, LineIdxInNode, lc.getWordCnt(caretNodeIdx, LineIdxInNode));
		if (caretIdx == startIdx) { //
			//printf("���� Ŀ���տ� %d�� ���� �ֽ��ϴ�. \n",caretIdx - LineIdxInNode - 1);
			return caretIdx - LineIdxInNode;
		}
		else {
			//printf("���� Ŀ���տ� %d�� ���� �ֽ��ϴ�. \n", caretIdx - LineIdxInNode );
			return caretIdx - LineIdxInNode;
		}
	}

	size_m getLastCaretIdx(size_m nodeIdx, mScreenLineContainer& lc) { //����� ������ ĳ�� �ε��� ���ϱ� 
		int lineCnt = lc.getNodeLineNumInfo(nodeIdx);
		return lc.getLastIdx(nodeIdx, (lineCnt - 1)) + lineCnt;
	}

	size_m getLastCaretIdxInLine(int nodeIdx, int lineIdx, mScreenLineContainer& lc) {
		int tempFirstIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc); //������ ù ĳ���ε���
		int tempWordCnt = lc.getLastIdx(nodeIdx, lineIdx) - lc.getFirstIdx(nodeIdx, lineIdx) + 1;
		return tempFirstIdx + tempWordCnt;
	}

	size_m getFirstCaretIdx(size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		if (lineIdx == 0) {
			return 0;
		}

		return lc.getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	}

	bool isLineFirst(size_m caretIdx, size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		int startIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc);
		if (caretIdx == startIdx) {
			return true;
		}

		return false;
	}
	size_m getLastLineIdxInNode(size_m nodeIdx, mScreenLineContainer& lc) {
		return lc.getNodeLineNumInfo(nodeIdx) - 1;
	}
	bool isLineLast(size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		size_m tempLineIdx = getLastLineIdxInNode(nodeIdx, lc);
		if (lineIdx == tempLineIdx) {
			return true;
		}
		else {
			return false;
		}
	}
	
	size_m getFitWordNum(HDC hdc, mString& mStr, size_m limitWidth, size_m startIdx, size_m avgCharWidth) {
		if (startIdx < 0) {
			printf("====================================> �߸��� ���� int getFitWordNum() startIdx�� 0���� �۾��� : %d \n", startIdx);
			system("pause");
			exit(-1);
		}
		if (getMstrPixelWidth(hdc, mStr, startIdx, mStr.getLength() - 1) <= limitWidth) {
			return (mStr.getLength() - 1);
		}

		//1. ��� ���� �ʺ� �̿��ؼ� limitWidth �ȿ� �� ����� ���� ���� �����غ���.
		int estWordNum = limitWidth / avgCharWidth + 1;
		if (estWordNum > mStr.getLength()) {
			estWordNum = mStr.getLength();
		}

		//2. �� ���� ���ڷ� ���� ���ο� ���ڿ� �����
		int tempEndIdx = startIdx + estWordNum - 1;
		//�߸��� �ε��� ���͸�
		if (tempEndIdx > (mStr.getLength() - 1))
			tempEndIdx = mStr.getLength() - 1;

		mString tempStr(mStr.subFromTo(startIdx, tempEndIdx)); //�ӽ� mString �ϳ� ����� ���� //3. ���� limitWidth�� ���ϸ鼭 �����غ���.
															  

		if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth) { //��� ������� �ѱ��ھ� 
			while (1) {
				tempEndIdx--;
				tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //���ɷ� ��ü 
				if (getMstrPixelWidthFull(hdc, tempStr) <= limitWidth)
					break;
			}
		}
		else {
			while (1) { //ª���� �Ѱ��� �����ش�. 
				tempEndIdx++;
				tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //���ɷ� ��ü 
				if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth)
					break;
			}
		}

		return tempEndIdx - startIdx + 1; //������ ��������.
	}



	////=> private �Լ��� : ĳ�� �ε����� ������ ��� ��������. ĳ�� ��� �ε����� 0���� ������ ĳ���� �����϶����� ���ŵǴ� �ɷ� 
	void setCaretNodeIdx(size_m n) {
		caretNodeIdx = n;
	}
	void setCaretLineIdxInNode(size_m n) {
		caretLineIdxInNode = n;
	}
	void setFrontWordNumInNode(size_m n) {
		frontWordNumInNode = n;
	}
	void setUpperLineNum(size_m n) {
		upperLineNum = n;
	}

	/////////////////////////////////////////////////////PUBLIC//////////////////////////////////////////
public:
	//1. ������
	mCaretMaster() {}
	mCaretMaster(size_m width, size_m height) {
		caretIdx = 0;
		caretNodeIdx = 0;
		caretLineIdxInNode = 0;
		frontWordNumInNode = 0;
		upperLineNum = 0;
		xPixel = 0;
		yPixel = 0;
		this->width = width;
		this->height = height;
	}
	void make(size_m width, size_m height) {
		caretIdx = 0;
		caretNodeIdx = 0;
		caretLineIdxInNode = 0;
		frontWordNumInNode = 0;
		upperLineNum = 0;
		xPixel = 0;
		yPixel = 0;
		this->width = width;
		this->height;
	}
	
	//2. setĳ���ε���
	void setCaretIdx(int newCaretIdx, int caretNodeIdx, mScreenLineContainer& lc) {
		caretIdx = newCaretIdx;
		caretLineIdxInNode = getCaretLineIdxInNode(newCaretIdx, caretNodeIdx, lc);
		frontWordNumInNode = getFrontWordNumInNode(newCaretIdx, caretNodeIdx, caretLineIdxInNode, lc);
		upperLineNum = getUpperLineNum(caretNodeIdx, caretNodeIdx, lc);
	}

	size_m getWidth() {
		return width;
	}
	size_m getHeight() {
		return height;
	}

	////set, get xPixel
	size_m getXpixel(HDC hdc, mTextSource& textSource, mScreenLineContainer& lc) {
		int result;
		//�ȼ��� ������ ù ���̸� ������ 0
		if (isLineFirst(caretIdx, caretNodeIdx, caretLineIdxInNode, lc)) {
			result = 0;
		}
		else { //�ƴϸ� 
			int tempStartIdx = lc.getFirstIdx(caretNodeIdx, caretLineIdxInNode); //ù��° �ε����� �˾Ƴ���.
			result = getMstrPixelWidth(hdc, mString(textSource.getTextAt(caretNodeIdx)), tempStartIdx, frontWordNumInNode); //���̸� �˾Ƴ���. 
		}
		return result;
	}

	
	
	size_m getYpixel() {
		return height*upperLineNum;
	}


	//3. ĳ���� ��� �ε����� �̵��ϸ鼭 �����ȴ�. ���� ���Ͱ� ���� �������� �ʴ´�. 
	//1. ��
	void moveUp(HDC hdc, int avgCharWidth, mTextSource& textSource, mScreenLineContainer& lc) {
		int tempCaretIdx;
		int tempNodeIdx;
		int tempLineIdx;
		int tempCurXwidth;
		int tempPrevXwidth;

		//1. ��ü �ؽ�Ʈ�� ù��° ���̸� �ƹ��ϵ� �Ͼ�� �ʴ´�.
		if (caretNodeIdx == 0 && caretLineIdxInNode == 0) {
			return;
		}
		else { //ù°���� �ƴϸ� ��尡 �̵��� ���̳� �ƴϳķ� �� ������. ----------------- ��尡 �ϳ� ������ ����. 
			if (caretLineIdxInNode == 0) { //ù°���̶� ��尡 �̵��ϴ� ��� //�̾ȿ����� ù° ĳ���̳� �ƴϳķ� ��������.
				tempNodeIdx = caretNodeIdx - 1; //��尡 ���� ���� �̵� �Ѵ�. 

				if (isLineFirst(caretIdx, caretNodeIdx, caretLineIdxInNode, lc)) { //ù°ĳ��? //����� �������� ù° ĳ������ �� ��
					tempLineIdx = getLastLineIdxInNode(tempNodeIdx, lc); //���� ����� ������ ����
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc); //���� ��� ������ ������ ù° ĳ��
				}
				else { //ù° ĳ���� �ƴϸ� ���� ������ ���ذ��鼭 �Űܺ���. // getfitwordnum �׽�Ʈ
					   //1. ���� ĳ���� ��ġ�ϴ� �������� xpixel�� ���Ѵ�. �̰� limit width�� �� ���̴�. 
					tempLineIdx = getLastLineIdxInNode(tempNodeIdx, lc); //���� ����� ������ ����

					int tempPrevStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx); //���������� ù°���� �ε���
					int tempPrevEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //���� ������ ������ ���� �ε���
					tempCurXwidth =getXpixel(hdc, textSource, lc); //���� ĳ���� �ִ� ���� xpixel ������. //limit width�� �ȴ�.
					tempPrevXwidth = getMstrPixelWidth(hdc, mString(textSource.getTextAt(tempNodeIdx)), tempPrevStartIdx, tempPrevEndIdx); //���� ������ ���̸� �������� ����.
					

					if (tempPrevXwidth < tempCurXwidth) { //���� ������ ���̰�  ���� ĳ���� �ִ� ���庸�� ª�ٸ� ���� ������ ������ ĳ������ 
						tempCaretIdx = getLastCaretIdxInLine(tempNodeIdx, tempLineIdx, lc);
					
					}
					else { //�ƴϰ� ���� ������ ���� ���庸�� ���̰� ���Ŵ� ��ٸ� 
						int tempCnt = getFitWordNum(hdc, mString(textSource.getTextAt(tempNodeIdx)), tempCurXwidth, tempPrevStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //���� ���� ù° �ε������� ���� �����ظ�ŭ
					}

				}
			}
			else { //--------------------------------------------------���� �ٲ��� �ʴ´�. //���ε� ������
				tempNodeIdx = caretNodeIdx; //���� �ٲ��� �ʰ���.
				tempLineIdx = caretLineIdxInNode - 1; //���� ���ȿ��� ���� ���� // ���⼭ ���� ������.

				if (isLineFirst(caretIdx, caretNodeIdx, caretLineIdxInNode, lc)) { //ù°ĳ��?
					tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc); //���� ��� ������ ������ ù° ĳ��
				}
				else {
					int tempPrevStartIdx = lc.getFirstIdx(tempNodeIdx, tempLineIdx); //���������� ù°���� �ε���
					int tempPrevEndIdx = lc.getLastIdx(tempNodeIdx, tempLineIdx); //���� ������ ������ ���� �ε���
					tempCurXwidth = getXpixel(hdc, textSource, lc); //���� ĳ���� �ִ� ���� xpixel ������. //limit width�� �ȴ�.
					tempPrevXwidth = getMstrPixelWidth(hdc, mString(textSource.getTextAt(tempNodeIdx)), tempPrevStartIdx, tempPrevEndIdx); //���� ������ ���̸� �������� ����.

					if (tempPrevXwidth < tempCurXwidth) { //���� ������ ���̰�  ���� ĳ���� �ִ� ���庸�� ª�ٸ� ���� ������ ������ ĳ������ 
						tempCaretIdx = getLastCaretIdxInLine(tempNodeIdx, tempLineIdx, lc);
					}
					else { //�ƴϰ� ���� ������ ���� ���庸�� ���̰� ���Ŵ� ��ٸ� 
						int tempCnt = getFitWordNum(hdc, mString(textSource.getTextAt(tempNodeIdx)), tempCurXwidth, tempPrevStartIdx, avgCharWidth);
						tempCaretIdx = getFirstCaretIdx(tempNodeIdx, tempLineIdx, lc) + tempCnt; //���� ���� ù° �ε������� ���� �����ظ�ŭ
					}
				}
			}

		}
		caretNodeIdx = tempNodeIdx;
		caretIdx = tempCaretIdx;
		setCaretIdx(tempCaretIdx, tempNodeIdx, lc);
	}

	//2. ��
	//3. ��
	//4. ��
	void moveRight(mTextSource& textSource,  mScreenLineContainer& lc) { //���� textSOURCE�� LC�� �ڷ���� ���̰� �ִ�. TEXTSOURCE�� ���� �ԷµǴ� ��Ȳ 
		int tempCaretIdx;
	
		//1. ĳ���� ������ ĳ�� �ε������ 
		if (caretIdx == getLastCaretIdxInLine(caretNodeIdx, caretLineIdxInNode, lc)) {
			printf("ĳ���� �������̴�. ");
			if (caretNodeIdx == lc.getTextSourceNodeNum()-1) { //1.1 �ٵ� ������ ����� //�ؽ�Ʈ �ҽ��� ������ 
				tempCaretIdx = caretIdx; //���� ���� ����
			}
			else { //������ ��尡 �ƴϸ� ���� ����� ù��° ĳ�� �ε����� �Ű� ��߰���.
				caretNodeIdx++;
				tempCaretIdx = 0;
			}
		}
		else { //�̵� ���� �ƴϸ� �׳� +1���ָ� �ǰ���.
			tempCaretIdx = caretIdx + 1;
		}
		caretIdx = tempCaretIdx;
		setCaretIdx(tempCaretIdx, caretNodeIdx, lc);
	}

	//����� ������ ������ �������� ������ �ִ� ��
	int getLastLineIdxInNode(int nodeIdx, map<int, int>& nodeLineNum) {
		return nodeLineNum[nodeIdx] - 1;
	}

	
	//get caret width, height
	
	


	

	

	


	//debugging
	void show() {
		printf("cur caret x,y pixel = %d , %d \n", xPixel, yPixel);
	}
	int getFrontWordNum() {
		return frontWordNumInNode;
	}



};