#include "lineContainer.h"
//1. 생성자
lineContainer::lineContainer() {};
//2. 데이터 setter // 0,1,7,8 //0노드의 1번 라인 인덱스는 7개 숫자를 가지고, 첫 문장의 인덱스는 8이다. 
void lineContainer::setData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx, int lineLastIdx) {
	//밑에 주석처리된 것처럼 하면 데이터 영역 자체가 스택에 형성되어 데이터를 잃어버리게 된다. 
	//map<int, int> wordCntAndFirstIdx;
	//wordCntAndFirstIdx[0] = lineWordCnt; //0은 한줄에 몇 글자 들어가는지 저장 
	//wordCntAndFirstIdx[1] = lineFirstIdx; //1은 문장 첫줄의 인덱스 저장
	//printf("%d, %d에 %d글자 저장했습니다. \n", nodeIdx, lineIdx, wordCntAndFirstIdx[0]);
	//map<int, map<int, int> > lineIdxAndOther;
	//lineIdxAndOther[lineIdx] = wordCntAndFirstIdx;

	//container[nodeIdx] = lineIdxAndOther; //컨테이너에 저장 [0][1][0] 이런식으로
										  //0번째 노드의 1번 인덱스 라인의 글자수, 문장 첫줄의 인덱스를 조회 할 수 있다. 

	container[nodeIdx][lineIdx][0] = lineWordCnt;
	container[nodeIdx][lineIdx][1] = lineFirstIdx;
	container[nodeIdx][lineIdx][2] = lineLastIdx;
}

//3. 라인의 글자수를 구한다. 
int lineContainer::getWordCnt(int nodeIdx, int lineIdx) {
	return container[nodeIdx][lineIdx][0];
}

//4. 라인의 첫번째 인덱스를 구한다. 
int lineContainer::getFirstIdx(int nodeIdx, int lineIdx) {
	return container[nodeIdx][lineIdx][1];
}

//5. 라인의 끝 인덱스를 구한다. 
int lineContainer::getLastIdx(int nodeIdx, int lineIdx) {
	return container[nodeIdx][lineIdx][2];
}