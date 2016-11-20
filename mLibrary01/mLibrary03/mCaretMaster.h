#pragma once
#include "mString.h"
#include "mScreenLineContainer.h"


class mCaretMaster {
private:
	//1. ĳ�� �ε���
	size_m caretIdx; //ĳ���� �ε��� 
	
	//2, 3 ĳ���� �Ҽӵ� ����Ʈ�� ��� �ε���// ��� �ȿ����� �����ε���
	size_m caretNodeIdx;
	size_m caretLineIdxInNode;
	
	//4. ĳ�� �տ� �� ���� �ִ°�? in node
	size_m frontWordNumInNode;
	
	//5. ĳ�� ���� �� �� �ִ°�?
	size_m upperLineNum;

	//6.,7 ĳ���� x,y �ȼ� ��ǥ
	size_m xPixel, yPixel;

	//8. ĳ���� y������ = ������ ũ��
	size_m width, height;

	//=> private �Լ��� : ĳ�� �ε����� ��� �ε����� �������� �������� �ڵ����� �����ǵ��� 
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

	//���� ������ ���õ� �͵��� ĳ���ȿ��� �������� �и���Ű��. 
	//���� ������ ���õ� linecontainer�� ���� ����Ʈ�� ������. lineContainer�ȿ� ���� ����Ʈ���� �� ����� ����. 
public:
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

	size_m getXpixel() {
		return xPixel;
	}
	size_m getYpixel() {
		return yPixel;
	}
	size_m getWidth() {
		return width;
	}
	size_m getHeight() {
		return height;
	}


	//debugging
	void show() {
		printf("cur caret x,y pixel = %d , %d \n", xPixel, yPixel);
	}



};