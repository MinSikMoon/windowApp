#ifndef _mCaret_
#define _mCaret_
#include <iostream>
using namespace std;

class mCaret {
private:
	int xIdx; //현재 캐럿이 소속된 문장 기준으로 캐럿앞에 몇글자 있는가?
	int yIdx; // 현재 캐럿이 소속된 문장 기준으로 볼 때 몇 번째 줄에 있는가? (상대적)
	int nodeIdx; // 현재 캐럿이 소속된 문장은 리스트의 몇번째 노드 소속 문장인가?
	int nodeLineNum; // 현재 캐럿이 소속된 문장이 소속된 노드는 화면에서 몇 줄로 표현되고 있는가?

public:
	mCaret() : xIdx(0), yIdx(0), nodeIdx(0), nodeLineNum(1) {} //기본 생성자, 0,0,0,1로 초기화

	//1. getters
	int getXIdx() {
		return xIdx;
	}

	int getYIdx() {
		return  yIdx;
	}

	int getNodeIdx() {
		return nodeIdx;
	}

	int getNodeLineNum() {
		return nodeLineNum;
	}

	//2. setters
	void setXIdx(int x_idx) {
		xIdx = x_idx;
	}

	void setYIdx(int y_idx) {
		yIdx = y_idx;
	}

	void setNodeIdx(int node_idx) {
		nodeIdx = node_idx;
	}

	void setNodeLineNum(int node_line_num) {
		nodeLineNum = node_line_num;
	}

	//3. show - debugging
	void show() {
		cout << xIdx << ", " << yIdx << ", " << nodeIdx << ", " << nodeLineNum << endl;
	}
	
	
	
};
#endif // !_mCaret_
