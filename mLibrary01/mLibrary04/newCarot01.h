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

	//1. �ϴ� cNodeIdx, cIdx�� ���� �Է��س��� �������� ��� �ذ�
	void setCidx(size_m newCidx) {
		cIdx = newCidx;
	}
	void setCnodeIdx(size_m newNodeIdx) {
		cNodeIdx = newNodeIdx;
	}

	//2. cLineIdx �˾Ƴ��� : cnodeidx�� cidx�� �ʿ� + lc
	size_m getClineIdx(mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(cIdx); //���� ĳ���Ҽ� ���� ���� �����ϴ°� //������. 

		for (int i = 0; i < curNodeLineNum; i++) { //���ٵ� �� �����Ѵ�? ù° ������ Ȯ���� ����. 
			int startCaretIdx = lc.getFirstIdx(cIdx, i) + i;
			int endCaretIdx = lc.getWordCnt(cIdx, i) + startCaretIdx;

			if (startCaretIdx <= cIdx && cIdx <= endCaretIdx) {
				return  i;
			}
		}

		return 0; //��� �� �ȿ� ���� ������ 0�� ����, 
	}
	void setClineIdx(mScreenLineContainer& lc) {
		cLineIdx = getClineIdx(lc);
	}

	//3. lineContainer�� �ְ� cIdx�� �־��� ��  ���ȿ��� ĳ���տ� ������ �� ���ڰ� �ִ��� �ľ����ִ� �Լ�
	size_m getFrontWordNum(mScreenLineContainer& lc) {
		if (cIdx == 0) //ĳ���ε����� 0�̸� ���ȿ��� ĳ���տ� 0�� ���ڰ� �ִ�. 
			return 0;

		return cIdx - cLineIdx;

	}
	void setFrontWordNum(mScreenLineContainer& lc) {
		frontWordNum = getFrontWordNum(lc);
	}

	//4. upperLineNum �����ֱ�
	size_m getUpperLineNum(mScreenLineContainer& lc) {
		int sum = 0;
		for (int i = 0; i < cIdx; i++) {
			sum += lc.getNodeLineNumInfo(i); //���ϰ��� �ϴ� ĳ����� ���������� ��� ������ ��� �����ش�. 
		}
		sum += cLineIdx; //ĳ���� ���� �����ε����� �����ָ� ĳ������ ��� ������ �ִ��� sum�� ����ȴ�. 
		return sum;
	}
	void setUpperLineNum(mScreenLineContainer& lc) {
		upperLineNum = getUpperLineNum(lc);
	}

	//4. ������ ù ĳ�� �ε����� �����ִ� �Լ�. 
	size_m getFirstCaretIdx(size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		if (lineIdx == 0) {
			return 0;
		}

		return lc.getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	}

	//5. �ش� ������ ������ ĳ�� �ε����� �����ִ� �Լ�
	size_m getLastCaretIdxInLine(int nodeIdx, int lineIdx, mScreenLineContainer& lc) {
		int tempFirstIdx = getFirstCaretIdx(nodeIdx, lineIdx, lc); //������ ù ĳ���ε���
		int tempWordCnt = lc.getLastIdx(nodeIdx, lineIdx) - lc.getFirstIdx(nodeIdx, lineIdx) + 1;
		printf("���� ĳ���� ������ ĳ���ε��� �޼ҵ忡�� lc.getlastidx 0,0 �ϸ� %d \n", lc.getLastIdx(0, 0));
		//printf("���� lastcaretidxinline���� tempFirstIdx = %d, lastIdx = %d, tempWordCnt = %d \n", tempFirstIdx, lc.getLastIdx(nodeIdx, lineIdx), tempWordCnt);
		return tempFirstIdx + tempWordCnt;
	}



public:
	//1. ������
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
	
	//2. ���� cline�� frontword�� �ƴ� ����� �����Ƿ� cIdx�� cNode �ΰ��� ������ �ڵ����� �� �����ִ� �Լ�
	void setAll(size_m _cIdx, size_m _cNodeIdx, mScreenLineContainer& lc) {
		setCidx(_cIdx);
		setCnodeIdx(_cNodeIdx); //�� �ΰ� �����ָ� ������ 3�� �ڵ� ������// width height�� �״��� �ʿ� ���ݾ�. 
		setClineIdx(lc);
		setFrontWordNum(lc);
		setUpperLineNum(lc);
	}

	
	//. ���������� �����̴� �� ��¥�� �ѹ� ������. //ĳ���� �����̱⿡ �ȼ� �ʿ����.
	void moveRight(mScreenLineContainer& lc) { 
		size_m tempCaretIdx = cIdx;
		size_m tempCaretNodeIdx = cNodeIdx;
		//printf("moveRight ���ö� cidx = %d, cnodeidx = %d , lastcaretidx = %d \n", tempCaretIdx, tempCaretNodeIdx, getLastCaretIdxInLine(cIdx, cLineIdx, lc));

		//1. ĳ���� ������ ĳ�� �ε������ 
		if (cIdx == getLastCaretIdxInLine(cNodeIdx, cLineIdx, lc)) { //nodeidx�� �ƴ϶� cidx�� �־��༭ ��������. 
			if (cNodeIdx == lc.getTextSourceNodeNum() - 1) { //1.1 �ٵ� ������ ����� //�ؽ�Ʈ �ҽ��� ������ 
				tempCaretIdx = cIdx; //���� ���� ����
			}
			else { //������ ��尡 �ƴϸ� ���� ����� ù��° ĳ�� �ε����� �Ű� ��߰���.
				tempCaretNodeIdx = cNodeIdx + 1; //��� �ϳ� ���������ְ� 
				tempCaretIdx = 0;
			}
		}
		else { //�̵� ���� �ƴϸ� �׳� +1���ָ� �ǰ���.
			tempCaretIdx = cIdx + 1;
		}
		//printf("moveRight ���� �� cidx = %d, cnodeidx = %d \n", tempCaretIdx, tempCaretNodeIdx);
		setAll(tempCaretIdx, tempCaretNodeIdx, lc); //���ο� cidx cnodeidx ����
	}

	
	
	
	
	
	
	// cIdx�˱�
	size_m getCidx() {
		return cIdx;
	}


	//�����
	void show() {
		printf(" ====> ���� ĳ���� cIdx: %d, nodeIdx: %d, cLine: %d, FrontWORDnUM: %d \n", cIdx, cNodeIdx, cLineIdx, frontWordNum);
	}

};