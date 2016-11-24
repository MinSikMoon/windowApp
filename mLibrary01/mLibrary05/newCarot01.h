#pragma once
#include "mString.h"

class newCarot{
private:
	size_m cIdx;
	size_m cNodeIdx;
	size_m cLineIdx;
	size_m frontWordNum;

	int width;
	int height;
	size_m upperLineNum;

	//1. 일단 cNodeIdx, cIdx를 먼저 입력해놔야 나머지를 모두 해결
	void setCidx(size_m newCidx) {
		cIdx = newCidx;
	}
	void setCnodeIdx(size_m newNodeIdx) {
		cNodeIdx = newNodeIdx;
	}

	//2. cLineIdx 알아내기 : cnodeidx와 cidx가 필요 + lc
	size_m getClineIdx(mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(cNodeIdx); //현재 캐럿소속 노드는 몇줄 차지하는가 //갯수다. 
		printf("$$$$$현재 getclineidx인데 curnodelinenum이 이렇게 나왔다. %d \n", curNodeLineNum);

		for (int i = 0; i < curNodeLineNum; i++) { //한줄도 안 차지한다? 첫째 시작일 확률이 높다. 
			int startCaretIdx = lc.getFirstIdx(cNodeIdx, i) + i;
			printf("$$$$$현재 %d번째 getclineidx인데 startcaretidx이 이렇게 나왔다. %d \n",i, startCaretIdx);
			int endCaretIdx = lc.getWordCnt(cNodeIdx, i) + startCaretIdx;
			printf("$$$$$현재 getclineidx인데 endcaretidx이 이렇게 나왔다. %d \n", endCaretIdx);
			if (startCaretIdx <= cIdx && cIdx <= endCaretIdx) {
				printf("<getClineIdx> 현재cIdx는 %d고 %d줄이 %d와 %d사이에 있다. 따라서 이 줄로 cline장착 할께 \n",cIdx, i, startCaretIdx, endCaretIdx);
				printf("<getClineIdx> %d 줄을 새롭게 장착 \n", i);
				return  i;
			}
		}

		return 0; //모든 것 안에 들지 않으면 0을 배출, 
	}
	void setClineIdx(mScreenLineContainer& lc) {
		cLineIdx = getClineIdx(lc);
	}

	//3. lineContainer가 있고 cIdx가 주어질 때  노드안에서 캐럿앞에 실제로 몇 글자가 있는지 파악해주는 함수
	size_m getFrontWordNum(mScreenLineContainer& lc) {
		if (cIdx == 0) //캐럿인덱스가 0이면 노드안에서 캐럿앞에 0개 글자가 있다. 
			return 0;
		
		return cIdx - cLineIdx;

	}
	void setFrontWordNum(mScreenLineContainer& lc) {
		frontWordNum = getFrontWordNum(lc);
	}

	//4. upperLineNum 구해주기
	size_m getUpperLineNum(mScreenLineContainer& lc) {
		int sum = 0;
		for (int i = 0; i < cIdx; i++) {
			sum += lc.getNodeLineNumInfo(i); //구하고자 하는 캐럿노드 이전까지의 노드 라인을 모두 더해준다. 
		}
		sum += cLineIdx; //캐럿의 현재 라인인덱스를 더해주면 캐럿위로 몇개의 라인이 있는지 sum에 저장된다. 
		return sum;
	}
	void setUpperLineNum(mScreenLineContainer& lc) {
		upperLineNum = getUpperLineNum(lc);
	}

	//4. 라인의 첫 캐럿 인덱스를 구해주는 함수. 
	size_m getFirstCaretIdx(size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		if (lineIdx == 0) {
			return 0;
		}

		return lc.getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	}

	//5. 해당 라인의 마지막 캐럿 인덱스를 구해주는 함수
	size_m getLastCaretIdxInLine(int nodeIdx, int lineIdx, mScreenLineContainer& lc) {
		int tempFirstIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc); //문장의 첫 캐럿인덱스
		//printf("<getlastCaretInLine> firstCaretIdx[%d][%d] = %d \n", nodeIdx, lineIdx, tempFirstIdx);
		//int tempWordCnt = lc.getLastIdx(nodeIdx, lineIdx) - lc.getFirstIdx(nodeIdx, lineIdx) + 1;
		int tempWordCnt = lc.getWordCnt(nodeIdx, lineIdx);
		//printf("******현재 캐럿의 마지막 캐럿인덱스 메소드에서 lc.getlastidx 0,0 하면 %d \n", lc.getLastIdx(0, 0));
		//printf("******현재 lastcaretidxinline에서 tempFirstIdx = %d, lastIdx = %d, tempWordCnt = %d \n", tempFirstIdx, lc.getLastIdx(nodeIdx, lineIdx), tempWordCnt);
		
		//printf("\n<getLastCaretIdxInLine> 배출되는 마지막 캐럿인덱스 in [%d][%d] = %d \n", nodeIdx, lineIdx, tempFirstIdx + tempWordCnt);
		return tempFirstIdx + tempWordCnt;
	}


	//6. 라인이 아니라 노드의 마지막 캐럿을 밝혀낸다. 
	size_m getLastCaretIdxInNode(size_m nodeIdx, mScreenLineContainer& lc) {
		int realLastLineIdx = lc.getNodeLineNumInfo(nodeIdx) -1 ;
		//printf("^^^^여기 노드의 마지막캐럿인덱스 함수인데 reallastlineidx가 %d라 나옴 \n", realLastLineIdx);
		//printf("^^^^여기 노드의 마지막캐럿인덱스 함수인데 노드의 마지막 라인의 마지막  캐럿이 %d라 나옴 \n", getLastCaretIdxInLine(nodeIdx, realLastLineIdx, lc));
		return getLastCaretIdxInLine(nodeIdx, realLastLineIdx, lc);
	}



	//7. 테스트 : front word num이 주어질 때 캐럿 인덱스를 구해주는 함수
	size_m getCaretLineIdx(size_m nodeIdx, size_m newFrontWordNum, mScreenLineContainer& lc) {
		//1. lc를 돌면서 wordCnt를 축적해본다. 
		int loopCnt = lc.getNodeLineNumInfo(nodeIdx);
		int wordCntSum = 0;

		for (int i = 0; i < loopCnt; i++) {
			int prevSum = wordCntSum;
			wordCntSum += lc.getWordCnt(nodeIdx, i);

			if (prevSum < frontWordNum || frontWordNum <= wordCntSum) {
				return i;
			}
		}

		return 0; // 루프를 안돌면? => 캐럿인덱스가 0일 경우 , frontWordNum이 0이니까.
	}

public:
	//1. 생성자
	newCarot() {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;
	}
	newCarot(int width, int height) {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;

		this->width = width;
		this->height = height;
	}
	void make(int width, int height) {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;

		this->width = width;
		this->height = height;
	}
	
	//2. 이제 cline과 frontword를 아는 기능이 있으므로 cIdx와 cNode 두개만 있으면 자동으로 다 맞춰주는 함수
	void setAll(size_m _cIdx, size_m _cNodeIdx, mScreenLineContainer& lc) {
		
		
		

		setCidx(_cIdx);
		setCnodeIdx(_cNodeIdx); //이 두개 정해주면 나머지 3개 자동 정해짐// width height는 그다지 필요 없잖아. 
		setClineIdx(lc);
		setFrontWordNum(lc);
		setUpperLineNum(lc);


		int realLastCaretIdx = getLastCaretIdxInNode(cNodeIdx, lc);
		//printf("<setAll> 들어온 _cIdx = %d, 새로 장착된 cIdx = %d, 새로운 clineidx = %d,  물리적 마지막 캐럿인덱스 = %d \n", _cIdx, cIdx,cLineIdx, realLastCaretIdx);
		if (_cIdx > realLastCaretIdx)
			setCidx(realLastCaretIdx);
	}

	
	//. 오른쪽으로 움직이는 것 가짜로 한번 만들어보자. //캐럿을 움직이기에 픽셀 필요없음.
	//void moveRight(mScreenLineContainer& lc) { 
	//	size_m tempCaretIdx = cIdx;
	//	size_m tempCaretNodeIdx = cNodeIdx;
	//	//printf("moveRight 들어올때 cidx = %d, cnodeidx = %d , lastcaretidx = %d \n", tempCaretIdx, tempCaretNodeIdx, getLastCaretIdxInLine(cIdx, cLineIdx, lc));
	//	//printf("\n <moveRight> cIdx = %d, 마지막 캐럿 in Line 정보 = %d \n", cIdx, getLastCaretIdxInLine(cNodeIdx, cLineIdx, lc));
	//	//1. 캐럿이 마지막 캐럿 인덱스라면 
	//	if (cIdx == getLastCaretIdxInLine(cNodeIdx, cLineIdx, lc)) { //nodeidx가 아니라 cidx를 넣어줘서 문제였다. 
	//		if (cNodeIdx == lc.getTextSourceNodeNum() - 1) { //1.1 근데 마지막 노드라면 //텍스트 소스의 사이즈 
	//			tempCaretIdx = cIdx; //현재 상태 유지
	//			//printf("\n <moveRight> 보합 \n"); 
	//			//printf("<moveRight in 보합> cNodeIdx = %d, 마지막 노드 정보 lc = %d \n", cNodeIdx, lc.getTextSourceNodeNum() - 1);
	//		}
	//		else { //마지막 노드가 아니면 다음 노드의 첫번째 캐럿 인덱스로 옮겨 줘야겠지.
	//			tempCaretNodeIdx = cNodeIdx + 1; //노드 하나 증가시켜주고 
	//			tempCaretIdx = 0;
	//		}
	//	}
	//	else { //이도 저도 아니면 그냥 +1해주면 되겠지.
	//		tempCaretIdx = cIdx + 1;
	//	}
	//	//printf("<moveRight> 나갈 때 cidx = %d, tempcids = %d \n", cIdx, tempCaretIdx);
	//	//추가 필터링 : lc의 lastIdx밖으로 벗어나려하면 방지해줘야 한다. 
	//	setAll(tempCaretIdx, tempCaretNodeIdx, lc); //새로운 cidx cnodeidx 장착
	//}

	//moveRight 새로 만들기 
	//1. lc에 현재 화면에 맞춰진 캐럿 인덱
	void moveRight(mScreenLineContainer& lc) { 
		int newFrontWordNum = frontWordNum + 1;
		int newClineIdx = getCaretLineIdx(cNodeIdx, newFrontWordNum, lc);
		
		size_m tempCaretIdx = cIdx;
		size_m tempCaretNodeIdx = cNodeIdx;
		//printf("moveRight 들어올때 cidx = %d, cnodeidx = %d , lastcaretidx = %d \n", tempCaretIdx, tempCaretNodeIdx, getLastCaretIdxInLine(cIdx, cLineIdx, lc));
		//printf("\n <moveRight> cIdx = %d, 마지막 캐럿 in Line 정보 = %d \n", cIdx, getLastCaretIdxInLine(cNodeIdx, cLineIdx, lc));
		//1. 캐럿이 마지막 캐럿 인덱스라면 
		if (cIdx == getLastCaretIdxInLine(cNodeIdx, newClineIdx, lc)) { //nodeidx가 아니라 cidx를 넣어줘서 문제였다. 
			if (cNodeIdx == lc.getTextSourceNodeNum() - 1) { //1.1 근데 마지막 노드라면 //텍스트 소스의 사이즈 
				tempCaretIdx = cIdx; //현재 상태 유지
				//printf("\n <moveRight> 보합 \n"); 
				//printf("<moveRight in 보합> cNodeIdx = %d, 마지막 노드 정보 lc = %d \n", cNodeIdx, lc.getTextSourceNodeNum() - 1);
			}
			else { //마지막 노드가 아니면 다음 노드의 첫번째 캐럿 인덱스로 옮겨 줘야겠지.
				tempCaretNodeIdx = cNodeIdx + 1; //노드 하나 증가시켜주고 
				tempCaretIdx = 0;
			}
		}
		else { //이도 저도 아니면 그냥 +1해주면 되겠지.
			tempCaretIdx = cIdx + 1;
		}

		setFrontWordNum(lc);
		setCidx(tempCaretIdx);
		setCnodeIdx(tempCaretNodeIdx);

	}
	
	
	
	
	// cIdx알기
	size_m getCidx() {
		return cIdx;
	}


	//디버깅
	void show(mScreenLineContainer& lc) {
		printf(" ====> 현재 캐롯의 cIdx: %d, nodeIdx: %d, cLine: %d, FrontWORDnUM: %d \n", cIdx, cNodeIdx, cLineIdx, frontWordNum);
		//printf(" ====> 실제 물리적 이 문단의 마지막 캐럿 인덱스는 %d \n", getLastCaretIdxInNode(cNodeIdx, lc));
	}

};