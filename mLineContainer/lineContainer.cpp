#include "lineContainer.h"
//1. ������
lineContainer::lineContainer() {};
//2. ������ setter // 0,1,7,8 //0����� 1�� ���� �ε����� 7�� ���ڸ� ������, ù ������ �ε����� 8�̴�. 
void lineContainer::setData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx) {
	map<int, int> wordCntAndFirstIdx;
	wordCntAndFirstIdx[0] = lineWordCnt; //0�� ���ٿ� �� ���� ������ ���� 
	wordCntAndFirstIdx[1] = lineFirstIdx; //1�� ���� ù���� �ε��� ����

	map<int, map<int, int> > lineIdxAndOther;
	lineIdxAndOther[lineIdx] = wordCntAndFirstIdx;

	container[nodeIdx] = lineIdxAndOther; //�����̳ʿ� ���� [0][1][0] �̷�������
										  //0��° ����� 1�� �ε��� ������ ���ڼ�, ���� ù���� �ε����� ��ȸ �� �� �ִ�. 
}

//3. ������ ���ڼ��� ���Ѵ�. 
int lineContainer::getWordCnt(int nodeIdx, int lineIdx) {
	return container[nodeIdx][lineIdx][0];
}

//4. ������ ù��° �ε����� ȣ���Ѵ�. 
int lineContainer::getFirstIdx(int nodeIdx, int lineIdx) {
	return container[nodeIdx][lineIdx][1];
}
