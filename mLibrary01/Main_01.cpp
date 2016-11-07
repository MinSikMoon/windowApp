#include <iostream>
#include <tchar.h>
#include <Windows.h>
typedef int size_m;


class mString {
	//MEMBERS : private
	TCHAR* str;
	size_m length;

public:
	//1. CONSTRUCTOR : const TCHAR가 들어왔을 때// 복사해서 붙여준다. //즉 깊은 복사가 일어난다. 
	//1.1 문자열이 들어왔을 때
	mString(const TCHAR* _inStr) {
		size_m tempLength = _tcslen(_inStr); // 길이 저장
		size_m bufferSize = (tempLength+1) * sizeof(TCHAR); // 유니코드를 대비해 2배로 잡아준다. 
		str = new TCHAR[bufferSize]; //str에 공간할당
		
		_tcscpy_s(str, bufferSize, _inStr); //str에 문자열을 복사한다. 
		length = tempLength; //길이도 저장한다. 
	}

	//1.2 문자가 들어왔을 때
	mString(TCHAR _inChar) {
		size_m bufferSize = 2 * sizeof(TCHAR); //2로 잡는다. 
		str = new TCHAR[bufferSize]; //str에 공간할당

		str[0] = _inChar;
		str[1] = 0; //nul을 넣어준다. 
		length = 1; //한글자 들어왔으니 길이는 1
	}
		
	//2. DESTRUCTOR
	~mString() {
		delete str;
	}


	//--------------METHODS---------------------
	//------------------------------ < 1. ADD : 오버로딩들 > ------------------------------------------
	//1.1. mString끼리 더하기
	void add(mString& _inStr) {
		size_m tempLength = length + _inStr.getLength(); //일단 길이를 임시 계산
		size_m bufferSize = (tempLength + 1) * sizeof(TCHAR); //두개를 합친 임시 공간 사이즈
		TCHAR* tempStr = new TCHAR[bufferSize]; //임시 공간을 만든다. 

		_tcscpy_s(tempStr, bufferSize, str); //가상 공간에 str을 복사
		_tcscat_s(tempStr, tempLength + 1, _inStr.getStr()); //뒤에다가 붙인다. 
		delete str; //현재 str을 해제시키고 
		
		str = tempStr; //tempStr로 갱신시킨다.
		length = tempLength; //길이도 갱신 시킨다. 
	}

	//1.2 일반 TCHAR문자열 더하기
	void add(const TCHAR* _inStr) {
		mString tempStr(_inStr); //임시 mString 하나 만들고
		(*this).add(tempStr);// 붙인다. 
	}

	//1.3 문자 더하기
	void add(TCHAR _inChar) {
		mString tempStr(_inChar); //임시 mString을 만들고
		(*this).add(tempStr); //붙인다. 
	}
	
	//------------------------------------ <2. subMstr : 문자열 잘라서 새로운 mString 배출 > ------------------------------
	//2.1 from인덱스부터 to인덱스까지 잘라서(인덱스 포함) 새로운 문자열 리턴.
	TCHAR* subFromTo(size_m startIdx, size_m endIdx) { 
		
		if (startIdx < 0 || endIdx >(length - 1) || startIdx > endIdx) { //필터 for 잘못된 입력
			printf("!!!!!!!!!********>>>>잘못된 범위 지정 in subFromTo() : sIdx: %d, eIdx: %d \n", startIdx, endIdx);
			system("pause");
			exit(-1); //에러
		}
		
		size_m tempLength = endIdx - startIdx + 1; //단순히 잘려진 문자의 갯수일 뿐, 인덱스와 헷갈리지 말자.  
		size_m bufferSize = (length + 1) * sizeof(TCHAR); //버퍼는 nul 공간까지 더해서 생성해주자. //주의 사항 str보다 무조건 같거나 커야함.
		printf("디버깅중: tempLength는 %d, buffersize는 %d \n", tempLength, bufferSize);
		TCHAR* tempStr = new TCHAR[bufferSize]; //임시 문자열 공간
		
		_tcscpy_s(tempStr, bufferSize, str + startIdx);
		tempStr[tempLength] = 0; //nul 값 넣어주기.
		
		return tempStr;
	}

	//2.2 from인덱스부터 끝까지 자르기
	TCHAR* subFromToEnd(size_m startIdx) {
		return subFromTo(startIdx, length - 1);
	}


	
	//-------------GETTER----------------------
	//1. str포인터 리턴
	TCHAR* getStr() {
		return str;
	}
	//2. 길이 리턴
	size_m getLength() {
		return length;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//debugging : show
	void show() {
		wprintf(L"%s", str);
		printf("\n");
		printf("문자열 길이는 : %d \n", length);
		
	}

};




int main() {
	mString m1 = TEXT("HI");
	mString m2 = TEXT("bye");
	
	m1.add(m2);
	m1.add(TEXT("what"));
	m1.add('7');
	
	m1.show();


	mString m3 = L"abcd";
	m3.show();
	wprintf(L"%ls \n", m3.subFromTo(0, 0));
	//wprintf(L"%ls \n", m3.subFromToEnd(5));

	












	system("pause");
	return 0;
}