#pragma once
#include "mHeaders.h"

//1. �Ϲ� ���� �Լ���
//1. TCHAR�� ���� sub
TCHAR* subFromTo(TCHAR* str, size_m startIdx, size_m endIdx) {
	int length = _tcslen(str);
	if (startIdx < 0 || endIdx >(length - 1) || startIdx > endIdx) { //���� for �߸��� �Է�
		printf("!!!!!!!!!********>>>>�߸��� ���� ���� in subFromTo() : sIdx: %d, eIdx: %d \n", startIdx, endIdx);
		system("pause");
		exit(-1); //����
	}
	size_m tempLength = endIdx - startIdx + 1; //�ܼ��� �߷��� ������ ������ ��, �ε����� �򰥸��� ����.  
	size_m bufferSize = (length + 1) * sizeof(TCHAR); //���۴� nul �������� ���ؼ� ����������. //���� ���� str���� ������ ���ų� Ŀ����.
													  //printf("�������: tempLength�� %d, buffersize�� %d \n", tempLength, bufferSize);
	TCHAR* tempStr = new TCHAR[bufferSize]; //�ӽ� ���ڿ� ����

	_tcscpy_s(tempStr, bufferSize, str + startIdx);
	tempStr[tempLength] = 0; //nul �� �־��ֱ�.

	return tempStr;
}

//2. MBCS ��� ���ڿ��� DBCS ��� ���ڿ��� ��ȯ
wchar_t* toWC(const char *str_param) {
	size_t convertedChars = 0;
	size_t str_param_len = strlen(str_param) + 1;

	wchar_t *wc = new wchar_t[str_param_len];
	setlocale(LC_ALL, "");
	mbstowcs_s(&convertedChars, wc, str_param_len, str_param, _TRUNCATE);

	return wc;
}

//3. const tchar ���ڿ��� �׳� tchar�� ����
TCHAR* cloneStr(const TCHAR* _inStr) {
	int bufferSize = (_tcslen(_inStr) + 1) * sizeof(TCHAR);
	TCHAR* tempStr = new TCHAR[bufferSize];
	_tcscpy_s(tempStr, bufferSize, _inStr);
	return tempStr;
}

//////////////////////////////////////////// mString Ŭ���� ///////////////////////////////////////////////////////
class mString {
	//MEMBERS : private
	TCHAR* str;
	size_m length;

public:
	mString() { //�� ���ڿ��� �⺻ ����
		size_m tempLength = 1; //�ٷ� null�� �����ϱ� ���� 0�� ��ġ�� 
		size_m bufferSize = tempLength * sizeof(TCHAR); // �����ڵ带 ����� 2��� ����ش�. 
		str = new TCHAR[bufferSize];
		_tcscpy_s(str, bufferSize, TEXT(""));
		length = 0;
	}

	//1. CONSTRUCTOR : const TCHAR�� ������ ��// �����ؼ� �ٿ��ش�. //�� ���� ���簡 �Ͼ��. 
	//1.1 ���ڿ��� ������ ��
	mString(const TCHAR* _inStr) {
		size_m tempLength = _tcslen(_inStr); // ���� ����
		size_m bufferSize = (tempLength + 1) * sizeof(TCHAR); // �����ڵ带 ����� 2��� ����ش�. 
		str = new TCHAR[bufferSize]; //str�� �����Ҵ�

		_tcscpy_s(str, bufferSize, _inStr); //str�� ���ڿ��� �����Ѵ�. 
		length = tempLength; //���̵� �����Ѵ�. 
	}

	//1.2 ���ڰ� ������ ��
	mString(TCHAR _inChar) {
		size_m bufferSize = 2 * sizeof(TCHAR); //2�� ��´�. 
		str = new TCHAR[bufferSize]; //str�� �����Ҵ�

		str[0] = _inChar;
		str[1] = 0; //nul�� �־��ش�. 
		length = 1; //�ѱ��� �������� ���̴� 1
	}

	//2. DESTRUCTOR
	~mString() {
		delete str;
	}


	//--------------METHODS---------------------
	//------------------------------ < 1. ADD : �����ε��� > ------------------------------------------
	//1.1. mString���� ���ϱ�
	void add(mString& _inStr) {
		size_m tempLength = length + _inStr.getLength(); //�ϴ� ���̸� �ӽ� ���
		size_m bufferSize = (tempLength + 1) * sizeof(TCHAR); //�ΰ��� ��ģ �ӽ� ���� ������
		TCHAR* tempStr = new TCHAR[bufferSize]; //�ӽ� ������ �����. 

		_tcscpy_s(tempStr, bufferSize, str); //���� ������ str�� ����
		_tcscat_s(tempStr, tempLength + 1, _inStr.getStr()); //�ڿ��ٰ� ���δ�. 
		delete str; //���� str�� ������Ű�� 

		str = tempStr; //tempStr�� ���Ž�Ų��.
		length = tempLength; //���̵� ���� ��Ų��. 
	}

	//1.2 �Ϲ� TCHAR���ڿ� ���ϱ�
	void add(const TCHAR* _inStr) {
		mString tempStr(_inStr); //�ӽ� mString �ϳ� �����
		(*this).add(tempStr);// ���δ�. 
	}

	//1.3 ���� ���ϱ�
	void add(TCHAR _inChar) {
		mString tempStr(_inChar); //�ӽ� mString�� �����
		(*this).add(tempStr); //���δ�. 
	}

	//------------------------------------ <2. subMstr : ���ڿ� �߶� ���ο� mString ���� > ------------------------------
	//2.1 from�ε������� to�ε������� �߶�(�ε��� ����) ���ο� ���ڿ� ����.
	TCHAR* subFromTo(size_m startIdx, size_m endIdx) {

		if (startIdx < 0 || endIdx >(length - 1) || startIdx > endIdx) { //���� for �߸��� �Է�
			printf("!!!!!!!!!********>>>>�߸��� ���� ���� in subFromTo() : sIdx: %d, eIdx: %d \n", startIdx, endIdx);
			system("pause");
			exit(-1); //����
		}

		size_m tempLength = endIdx - startIdx + 1; //�ܼ��� �߷��� ������ ������ ��, �ε����� �򰥸��� ����.  
		size_m bufferSize = (length + 1) * sizeof(TCHAR); //���۴� nul �������� ���ؼ� ����������. //���� ���� str���� ������ ���ų� Ŀ����.
														  //printf("�������: tempLength�� %d, buffersize�� %d \n", tempLength, bufferSize);
		TCHAR* tempStr = new TCHAR[bufferSize]; //�ӽ� ���ڿ� ����

		_tcscpy_s(tempStr, bufferSize, str + startIdx);
		tempStr[tempLength] = 0; //nul �� �־��ֱ�.

		return tempStr;
	}

	//2.2 from�ε������� ������ �ڸ���
	TCHAR* subFromToEnd(size_m startIdx) {
		return subFromTo(startIdx, length - 1);
	}



	//------------------------------------ <3. insertInto : ���ڿ��� Ư�� ��ġ�� ���ο� ���ڿ��� ����> ---------------------------------
	//3.1 Ư�� �ε����� ���ο� ���ڿ��� �����ؼ� str�� ���� �����Ѵ�. 
	void insertInto(size_m targetIdx, const TCHAR* _inStr) {
		if (targetIdx < 0 || targetIdx >(length - 1)) {
			printf("!!!!!!==============> �߸��� ���� ���� in insertInto()/ targetIdx : %d \n", targetIdx);
			system("pause");
			exit(-1);
		}

		size_m tempLength = length + _tcslen(_inStr); //�� ���� ���̸� ���� ���� ���ο� ���̰���. 

													  //3.1.1 ó���� ���� ���̷� �Ҷ�
		if (targetIdx == 0) {

			mString tempStr(_inStr); //_inStr�� �ӽ� mString�� ����� ���⿡ ������ ���� str�� �ٿ��� ��.
			tempStr.add(str); //�ٿ���. ���� str�� ���� tempStr�� str�� �Ǿ�� ��. 
			delete str;
			str = tempStr.cloneStr(); //���� ���ڿ� �����ϰ� �� ���ڿ��� ��ü
		}//3.1.2 �������� ���� ���̷� �Ҷ�// �̰Ŵ� �׳� add�� ����. 
		else if (targetIdx == (length - 1)) {
			add(_inStr);
		}//3.1.3 ���� ��򰡿� ���� ���̴� ���̽� 
		else {
			TCHAR* temp1 = subFromTo(0, targetIdx - 1); //�� �մ� ���ڿ� �и�
			TCHAR* temp3 = subFromToEnd(targetIdx); //�� �޴� ���ڿ� �и�
			mString temp2(temp1); //mString���� �����
			temp2.add(_inStr); //_inStr ���� ���̱�
			temp2.add(temp3); //���� temp2�� str�� �츮�� ���ϴ� ����� ������ �Ǿ���. 

			delete str;
			str = temp2.cloneStr();//str ����
			delete temp1;
			delete temp3; //�ڿ� ����
		}
		length = tempLength;
	}

	//3.2 insertInto : mString�� ���ڷ� ���� ���
	void insertInto(size_m targetIdx, mString& _inMstr) {
		if (targetIdx < 0 || targetIdx >(length - 1)) {
			printf("!!!!!!==============> �߸��� ���� ���� in insertInto()/ targetIdx : %d \n", targetIdx);
			system("pause");
			exit(-1);
		}
		TCHAR* _inStr = _inMstr.cloneStr(); //�ڿ� ���� ������.
		size_m tempLength = length + _tcslen(_inStr); //�� ���� ���̸� ���� ���� ���ο� ���̰���. 

													  //3.1.1 ó���� ���� ���̷� �Ҷ�
		if (targetIdx == 0) {

			mString tempStr(_inStr); //_inStr�� �ӽ� mString�� ����� ���⿡ ������ ���� str�� �ٿ��� ��.
			tempStr.add(str); //�ٿ���. ���� str�� ���� tempStr�� str�� �Ǿ�� ��. 
			delete str;
			str = tempStr.cloneStr(); //���� ���ڿ� �����ϰ� �� ���ڿ��� ��ü
		}//3.1.2 �������� ���� ���̷� �Ҷ�// �̰Ŵ� �׳� add�� ����. 
		else if (targetIdx == (length - 1)) {
			add(_inStr);
		}//3.1.3 ���� ��򰡿� ���� ���̴� ���̽� 
		else {
			TCHAR* temp1 = subFromTo(0, targetIdx - 1); //�� �մ� ���ڿ� �и�
			TCHAR* temp3 = subFromToEnd(targetIdx); //�� �޴� ���ڿ� �и�
			mString temp2(temp1); //mString���� �����
			temp2.add(_inStr); //_inStr ���� ���̱�
			temp2.add(temp3); //���� temp2�� str�� �츮�� ���ϴ� ����� ������ �Ǿ���. 

			delete str;
			str = temp2.cloneStr();//str ����
			delete temp1;
			delete temp3; //�ڿ� ����
		}
		delete _inStr;
		length = tempLength;
	}

	//-------------------------------------- <4. cloneStr : Str�� �����ؼ� ���ڿ��� ��������.> -----------------
	TCHAR* cloneStr() {
		int bufferSize = (length + 1) * sizeof(TCHAR);
		TCHAR* tempStr = new TCHAR[bufferSize]; //���� �����ŭ ������ ����
		_tcscpy_s(tempStr, bufferSize, str);
		return tempStr;
	}


	//-------------------------------------- <5. replaceStr : str�� �ٸ� ���ڿ��� ��ü���ش�. > -------------------
	void replaceStr(TCHAR* _inStr) {
		delete str; //str ����
		str = _inStr; //str ��ü
		length = _tcslen(_inStr); //���̵� ��ü
	}

	//-------------GETTER----------------------
	//1. str������ ����
	TCHAR* getStr() {
		return str;
	}
	//2. ���� ����
	size_m getLength() {
		return length;
	}

	//debugging : show
	void show() {
		_tprintf(TEXT("%ls\n"), str);
		printf("���ڿ� ���̴� : %d \n", length);

	}

};