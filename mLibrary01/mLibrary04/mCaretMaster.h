#pragma once
#include "mString.h"
#include "mScreenLineContainer.h"


class mCaretMaster {
private:
	//1. 캐럿 인덱스
	size_m caretIdx; //캐럿의 인덱스 
	
	//2, 3 캐럿의 소속된 리스트의 노드 인덱스// 노드 안에서의 라인인덱스
	size_m caretNodeIdx;
	size_m caretLineIdxInNode;
	
	//4. 캐럿 앞에 몇 글자 있는가? in node
	size_m frontWordNumInNode;
	
	//5. 캐럿 위로 몇 줄 있는가?
	size_m upperLineNum;

	//6.,7 캐럿의 x,y 픽셀 좌표
	size_m xPixel, yPixel;

	//8. 캐럿의 y사이즈 = 글자의 크기
	size_m width, height;

	//=> private 함수들 : 캐럿 인덱스와 노드 인덱스가 정해지면 나머지는 자동으로 결정되도록 
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

	//라인 정보에 관련된 것들은 캐럿안에서 하지말고 분리시키자. 
	//라인 정보에 관련된 linecontainer와 라인 리스트를 만들자. lineContainer안에 라인 리스트까지 다 멤버로 넣자. 
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