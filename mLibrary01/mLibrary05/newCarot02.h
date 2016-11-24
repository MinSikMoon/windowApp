#pragma once
#include "mString.h"

class newCarot {
private://=====================private 멤버
	size_m frontWordNum;
	size_m cNodeIdx;
	size_m cLineIdx;
	size_m cIdx; //cIdx는 방향키이동때 중요, 입력할 때는 cIdx를 믿으면 안된다. 

	int width;
	int height;
	size_m upperLineNum;
	//==========================private 함수들

	//-. cIdx가 몇 번째 라인에 있는지 알아내기 
	size_m getLineIdxByCidx(size_m cIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(nodeIdx); //현재 노드가 몇줄로 출력하는가 구한다. 
															 // cIdx가 몇 번째 라인에 존재하는지 알아낸다. 
		for (int i = 0; i < curNodeLineNum; i++) {
			int startCidx = lc.getFirstCaretIdx(nodeIdx, i);
			int endCidx = lc.getLastCaretIdx(nodeIdx, i);
			if (startCidx <= cIdx && cIdx <= endCidx) {
				return i;
			}
		}

		return 0; //한줄도 없으면 0 캐럿 인덱스 
	}
	//-. 기존 cIdx앞에는 몇 글자가 존재하는지 알아내주는 함수//=================> cIdx로 frontWordNum알아낸다. 
	//cIdx로 frontWordCnt가 문제가 있는게, 개행을 하면서 같은 cIdx라도 글자 갯수가 달라진다. 따라서 frontWordNum이 더 중요하다. 
	size_m getFrontWordNumByCidx(size_m cIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		if (cIdx == 0) {
			return 0;
		}
		int curLineIdx = getLineIdxByCidx(cIdx, nodeIdx, lc);
		return cIdx - curLineIdx;
	} 
	



	//1. 실제 idx로 실제 라인인덱스 알아내기
	size_m getLineIdxInNode(size_m realIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(nodeIdx); //현재 노드가 몇줄로 출력하는가 구한다. 

		for (int i = 0; i < curNodeLineNum; i++) {
			int startIdx = lc.getFirstIdx(nodeIdx, i); //현재 i번째 줄의 실제 첫번째 인덱스
			int endIdx = lc.getLastIdx(nodeIdx, i); //'' 마지막 인덱스

			if (startIdx <= realIdx && realIdx <= endIdx) //둘 사이에 들어가 있으면 i번째 줄이다. 
				return i;
		}

		return 0; //0일 경우는 0번째 줄이니까.
	}
	
	//2. 글자의 실제 idx로 뒤에 있는 캐럿인덱스 알아내기
	size_m getCidxByRealIdx(size_m realIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curLineIdx = getLineIdxInNode(realIdx, nodeIdx, lc); //현재 realIdx가 소속된 라인인덱스 
		return realIdx + curLineIdx + 1; //realIdx뒤에 있는 캐럿 인덱스 
	}

	//3. 위 두개를 사용해서 이제는 frontWordNum으로 뒤에있는 캐럿인덱스 조사해주기 
	size_m getCidxByFrontWordNum(size_m FrontWordNum, size_m nodeIdx, mScreenLineContainer& lc) {
		if (FrontWordNum == 0)
			return 0;

		int realIdx = FrontWordNum - 1; //실제 인덱스는 갯수에서 하나 빼줘야지. 
		return getCidxByRealIdx(realIdx, nodeIdx, lc);
	}

	//4. frontWordNum으로 라인인덱스 구하기
	size_m getLineIdxByFrontWordNum(size_m frontWordNum, size_m nodeIdx, mScreenLineContainer& lc) {
		int realIdx = frontWordNum - 1;
		return getLineIdxInNode(realIdx, nodeIdx, lc);
	}




	//1. line 세팅
	void setClineIdx(size_m newLineIdx) {
		cLineIdx = newLineIdx;
	}
	void setFrontWordNum(size_m newNum) {
		frontWordNum = newNum;
	}
	void setCidx(size_m newIdx) {
		cIdx = newIdx;
	}
	void setCnodeIdx(size_m newIdx) {
		cNodeIdx = newIdx;
	}












	//============================================================PUBLIC
public:
	//1. 생성자
	newCarot() {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;
		upperLineNum = 0;

		int width = 2;
		int height = 16;
	}
	newCarot(int width, int height) {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;
		upperLineNum = 0;

		this->width = width;
		this->height = height;
	}
	void make(int width, int height) {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;
		upperLineNum = 0;

		this->width = width;
		this->height = height;
	}


	//2. 이동과 입력 따로 만들어줘야 한다. 
	//-1. 입력시  
	void input(mScreenLineContainer& lc) {
		//1. 기존 frontWordNum에 한글자를 추가해준다. 
		frontWordNum++; //lc에는 글자가 들어와있고 캐럿은 가상의 글자 숫자다. lc에 아무런 영향을 못끼치니까 괜찮다. //nodeIdx도 나중에 먼저 갱신해줘야한다. 
		//2. 새로운 frontWordNum기준으로 cIdx와 lineIdx를 정해준다. 
		setClineIdx(getLineIdxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setCidx(getCidxByFrontWordNum(frontWordNum, cNodeIdx, lc));

	}
	//-1. 오른쪽
	




	//디버깅
	void show(mScreenLineContainer& lc) {
		printf(" ====> 현재 캐롯의 cIdx: %d, nodeIdx: %d, cLine: %d, FrontWORDnUM: %d \n", cIdx, cNodeIdx, cLineIdx, frontWordNum);
		//printf(" ====> 실제 물리적 이 문단의 마지막 캐럿 인덱스는 %d \n", getLastCaretIdxInNode(cNodeIdx, lc));
	}

};