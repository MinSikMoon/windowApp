#include "mHeaders.h"
#include "mString.h"
#include "mTextPixel.h"
#include "mScreenLineContainer.h"
#pragma once


class mCaret {
	//ĳ�� �ε����� ¯�̴�. // �������� �Լ��� �� ������.
	size_m caretIdx; //ĳ�� �ε���, strlen(str)+���μ� �� ������ ������.

	size_m curNodeIdx; //ĳ���� �Ҽӵ� ��� �ε���
	size_m curLineIdxInNode; //ĳ���� �Ҽӵ� ��� �ȿ����� ���� �ε���
	size_m frontWordNumInNode; //ĳ���տ� ���� � ���ҳ�?
	size_m upperLineNum; //ĳ�� ���� �� ���� �ֳ�?


						 //private setters //ĳ�� �ε����� �� ���������� ��� �̰����� ��������.
	void setCurNodeIdx(size_m n) {
		curNodeIdx = n;
	}
	void setCurLineIdxInNode(size_m n) {
		curLineIdxInNode = n;
	}
	void setFrontWordNumInNode(size_m n) {
		frontWordNumInNode = n;
	}
	void setUpperLineNum(size_m n) {
		upperLineNum = n;
	}

	//ĳ�� �ε����� ������ �������� �ڵ����� �����ִ� �����̺� �Լ���

	//���� ���� textSource�� �ƹ��͵� �� ���ִ� ��Ȳ �̶�� ������ �߻��� ������ �ִ�. 
	//1. ĳ�� �Ҽ� ��� �ε����� ĳ�� �ε����� ������ ���� ����� �� ��° ���ο� �����ִ��� �˾ƺ��ִ� �Լ�--------------------------- -1����
	size_m getLineInNode(size_m curNodeIdx, size_m caretIdx, map<size_m, size_m>& nodeLineNum, mScreenLineContainer& lc) {
		size_m lineCnt = nodeLineNum[curNodeIdx]; //���� ��尡 �� �� ���� �ϴ��� ���´�. //textSource�� 0 ����� ���ڿ��� ȭ�鿡 �� �ٷ� ��µǴ°�?

		for (size_m i = 0; i < lineCnt; i++) { //���� ����ŭ ���鼭 üũ���ش�. 
			size_m startIdx; //ĳ���ε����� ���� �ε����� �ӽ�����.
			startIdx = lc.getFirstIdx(curNodeIdx, i) + i; //ĳ�� �ε����̴�! �򰥸��� ����.
			size_m endIdx;
			endIdx = startIdx + lc.getWordCnt(curNodeIdx, i); //���� ĳ�� �ε������� ���ڼ���ŭ �����ָ� �� ĳ�� �ε���.

			if (startIdx <= caretIdx && caretIdx <= endIdx) //���� �ȿ� ���� �ش� ���� �ε��� ����
				return i;
		}

		//�������� 0���� �غ���.// ���� -1�̾���. �̰� ���� ���� �ɱ��
		return 0;
	}
	//2. ĳ���� ���� ������ ù��°// ������ ĳ�� �ε��� ���� �Լ�
	size_m getFirstCaretIdxInLine(size_m nodeIdx, size_m lineIdxInNode, mScreenLineContainer& lc) {
		if (lineIdxInNode == 0) {
			return 0;
		}
		else {
			size_m startCaretIdx = lc.getFirstIdx(nodeIdx, lineIdxInNode) + lineIdxInNode;
			return startCaretIdx;
		}
	}

	bool isLineFirstCaret(size_m nodeIdx, size_m lineIdx, mScreenLineContainer& lc) {
		int tempStartCaretIdx = getFirstCaretIdxInLine(nodeIdx, lineIdx, lc);
		if (lineIdx == tempStartCaretIdx || lineIdx == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	//���ΰ� ����� ������ ĳ�� �ε����� ã���ִ� �Լ�// �򰥸��� ����.
	size_m getLastCaretIdxInLine(size_m nodeIdx, size_m lineIdxInNode, mScreenLineContainer& lc) {
		int tempLastRealIdxInNode = lc.getLastIdx(nodeIdx, lineIdxInNode);
		return tempLastRealIdxInNode + lineIdxInNode + 1;

	}

	size_m getLastCaretIdxInNode(int nodeIdx, map<size_m, size_m>& nodeLineNum, mScreenLineContainer& lc) {
		int tempLineNum = nodeLineNum[nodeIdx];
		int answer = lc.getLastIdx(nodeIdx, tempLineNum - 1) + tempLineNum;
	}

	//3. ĳ�� �տ� �� ���� �ִ��� �˾ƺ��ִ� �Լ� : 
	size_m getFrontWordNumInNode(size_m nodeIdx, size_m lineIdxInNode, size_m caretIdx, mScreenLineContainer& lc) {
		if (caretIdx == 0) //ĳ�� �ε����� 0�̸� �տ� �ƹ� ���ڵ� ������. 0�� 
			return 0;

		int startCaretIdx = getFirstCaretIdxInLine(nodeIdx, lineIdxInNode, lc); //���ο��� ù��° ĳ���ε���
		if (caretIdx == startCaretIdx) { //���� ĳ�� �ε����� �ڽ��� �����ε������� 2��ū ���� ���ָ� �װ� �ձ��� ����
			return caretIdx - (lineIdxInNode + 2); //�ε����� �ƴ϶� ������ ������ �����!!
		}
		else { //���������� �ڽ��� �����ε������� 1�� ū ���� ���ָ� �װ� �ձ��� ����
			return caretIdx - (lineIdxInNode + 1);
		}
	}

	//4. ĳ�� ���� �� �� �ִ� �� �����ش�. (��ü �ؽ�Ʈ �������� ) (textController�� ������ �򰥸��� ����. �̰Ŵ� ĳ������ �� ���̳� �̰Ŵ�.)
	size_m getUpperLineNum(size_m nodeIdx, size_m lineIdx, map<size_m, size_m> nodeLineNum) {
		int sum = 0;
		for (int i = 0; i < nodeIdx; i++) { //���� ���� ���� ���� �� ������.
			sum += nodeLineNum[i];
		}
		sum += lineIdx;
		return sum;
	}

	//5. ĳ�� �տ� �� ���� �ܾ ���� ����, ������ ��. //mStr�� �ȼ� ���̸� 
	int getFitWordNum(HDC hdc, mString& mStr, int limitWidth, int startIdx, int avgCharWidth) {
		if (startIdx < 0) {
			printf("====================================> �߸��� ���� int getScreenEndIdx() startIdx�� 0���� �۾��� : %d \n", startIdx);
			system("pause");
			exit(-1);
		}


		if (getMstrPixelWidth(hdc, mStr, startIdx, mStr.getLength() - 1) <= limitWidth) { //���� �ȼ� ����� ���庸�� �� ��� �׳� ������ ������ �ε��� �̾��ָ� �ǰ���. 
			return (mStr.getLength() - startIdx); //startIdx������ ���� ����
		}

		//1. ��� ���� �ʺ� �̿��ؼ� limitWidth �ȿ� �� ����� ���� ���� �����غ���.
		int estWordNum = limitWidth / avgCharWidth + 1;

		//2. �� ���� ���ڷ� ���� ���ο� ���ڿ� �����
		int tempEndIdx = startIdx + estWordNum - 1;
		mString tempStr(mStr.subFromTo(startIdx, tempEndIdx));

		//3. ���� limitWidth�� ���ϸ鼭 �����غ���.
		if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth) { //��� ������� �ѱ��ھ� 
			while (1) {
				tempEndIdx--;
				tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //���ɷ� ��ü 
				if (getMstrPixelWidthFull(hdc, tempStr) <= limitWidth)
					break;
			}
		}
		else {
			while (1) {
				tempEndIdx++;
				tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //���ɷ� ��ü 
				if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth)
					break;
			}
		}
		return tempEndIdx - startIdx + 1; //������ ��������.
	}

public:
	//1. CONSTRUCTOR // �� 0���� �ʱ�ȭ 
	mCaret() : curNodeIdx(0), curLineIdxInNode(0), frontWordNumInNode(0), upperLineNum(0), caretIdx(0) {}

	//2. setCaretIdx() : ĳ�� �ε����� �����ϸ� curNodeIdx�� �� ������ �������� ���� �����ȴ�. 
	void setCaretIdx(int nodeIdx, int lineIdx, int _caretIdx, map<size_m, size_m>& nodeLineNum, mScreenLineContainer& lc) {
		caretIdx = _caretIdx;
		setCurLineIdxInNode(getLineInNode(nodeIdx, caretIdx, nodeLineNum, lc));
		setFrontWordNumInNode(getFrontWordNumInNode(nodeIdx, lineIdx, caretIdx, lc));
		setUpperLineNum(getUpperLineNum(nodeIdx, lineIdx, nodeLineNum));
	}

	//degugging : show()
	void show() {
		printf("\n ** < ĳ�� ���� > ** \n");
		printf("���� ĳ�� �Ҽ� ��� �ε��� : %d \n", curNodeIdx);
		printf("���� ĳ�� �Ҽ� ���� ���� �ε��� : %d \n", curLineIdxInNode);
		printf("���� ĳ�� �տ� �� ���ڰ� �ֳ�? (��� ���ڿ���) : %d \n", frontWordNumInNode);
		printf("���� ĳ�� ���� �� �� �ֳ�? : %d \n", upperLineNum);
		printf("���� ĳ���� ĳ�� �ε��� : %d \n", caretIdx);
	}
};


