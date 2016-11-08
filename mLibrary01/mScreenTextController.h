#include "mHeaders.h"
#include "mTextSource.h"
#include "mScreenLineContainer.h"
#pragma once
class mScreenTextController {
	//0. 본문 자체를 가지고 있는 클래스
	mTextSource textSource;
	map<int, int> nodeLineNum; //노드당 몇라인을 가지고 있는지 갯수를 가지고 있는 맵
	mScreenLineContainer lc; //노드와 노드안 라인 인덱스를 넣으면 실제 textSource의 노드상의 문자열 인덱스와 마지막 인덱스, 글자수를 알수있음.

							 //1. 전체 사이즈 관련 변수들 
	size_m wordPixelHeight; // 문자의 높이는 픽셀로 몇인가?
	size_m totalPixelHeight; // 전체 텍스트의 높이는 픽셀로 몇인가? 
	size_m totalLineNum; // 전체 텍스트는 몇 줄인가? 
	size_m upperLineNum; // 화면 위로 잘린 줄은 몇 줄인가?
	size_m restLineNum; // 현재 화면에 보이는 줄 + 남은 줄의 갯수 totalLineNum - upperLineNum

						//2. 화면에 출력되는 첫번째 문장과 화면 맨끝의 문장이 속한 노드와 해당 노드 안에서의 라인 인덱스
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;



};