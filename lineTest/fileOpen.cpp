#include <tchar.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include "mstring.h"
using namespace std;

int _tmain() {
	//1. FILE
	FILE* fp;
	fopen_s(&fp, "test.txt", "r");
	int cnt = 0;

	//2. buffer & vector
	TCHAR buf[100];
	vector<TCHAR*> v1;

	//3. loop
	while (_fgetts(buf, 100, fp) != NULL) {
		TCHAR* temp = cloneTchar(buf);
		v1.push_back(temp);
		cnt++;
	}

	//4. 출력
	for(int i = 0; i < cnt; i++) {
		_tprintf(_T("%ls"), v1[i]);
	}

	//5. 메모리해제
	for (int i = 0; i < cnt; i++) {
		delete v1[i];
	}

	fclose(fp);
	system("pause");
	return 0;
}
