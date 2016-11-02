#ifndef _mCaret_
#define _mCaret_
#include <iostream>
using namespace std;

class mCaret {
private:
	int xIdx; //���� ĳ���� �Ҽӵ� ���� �������� ĳ���տ� ����� �ִ°�?
	int yIdx; // ���� ĳ���� �Ҽӵ� ���� �������� �� �� �� ��° �ٿ� �ִ°�? (�����)
	int nodeIdx; // ���� ĳ���� �Ҽӵ� ������ ����Ʈ�� ���° ��� �Ҽ� �����ΰ�?
	int nodeLineNum; // ���� ĳ���� �Ҽӵ� ������ �Ҽӵ� ���� ȭ�鿡�� �� �ٷ� ǥ���ǰ� �ִ°�?

public:
	mCaret() : xIdx(0), yIdx(0), nodeIdx(0), nodeLineNum(1) {} //�⺻ ������, 0,0,0,1�� �ʱ�ȭ

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
