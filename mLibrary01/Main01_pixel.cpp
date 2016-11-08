#include "mHeaders.h"
#include "mString.h"
#include "mTextPixel.h"
#include "mTextSource.h"
#include "mScreenLineContainer.h"

//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

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

	//5. ĳ�� �տ� �� ���� �ܾ ���� ����, ������ ��.
	int getFitWordNum(HDC hdc, TCHAR* targetStr, int limitPixelWidth, int startIdx, int avgCharWidth) {

		if (getStrPixelWidth(hdc, targetStr, startIdx, _tcslen(targetStr) - 1) <= limitPixelWidth) {
			int tempIdx;
			tempIdx = (_tcslen(targetStr) - 1);
			return tempIdx - startIdx + 1; //������ ó������ ������ ������ ������ ���� 
		}
		//1. ��� ���� �ʺ� �̿��ؼ� limitWidth �ȿ� �� ����� ���� ���� �����غ���.
		int estWordNum = limitPixelWidth / avgCharWidth + 1;
		//2. �� ���� ���ڷ� ���� ���ο� ���ڿ� �����
		int tempEndIdx = startIdx + estWordNum - 1;
		TCHAR* tempStr = subTchar(targetStr, startIdx, tempEndIdx);
		//3. ���� limitWidth�� ���ϸ鼭 �����غ���.
		if (strPxWidth(hdc, tempStr) > limitPixelWidth) { //��� ������� �ѱ��ھ� 
			while (1) {
				tempEndIdx--;
				TCHAR* cleaner = tempStr;
				tempStr = subTchar(targetStr, startIdx, tempEndIdx);
				delete cleaner;
				if (strPxWidth(hdc, tempStr) <= limitPixelWidth)
					break;
			}
		}
		else {
			while (1) {
				tempEndIdx++;
				TCHAR* cleaner = tempStr;
				tempStr = subTchar(targetStr, startIdx, tempEndIdx);
				delete cleaner;
				if (strPxWidth(hdc, tempStr) >= limitPixelWidth)
					break;
			}
		}
		delete tempStr; //������ ����
		return tempEndIdx - startIdx + 1; //������ ��������.
	}

public:
	//1. CONSTRUCTOR // �� 0���� �ʱ�ȭ 
	mCaret() : curNodeIdx(0), curLineIdxInNode(0), frontWordNumInNode(0), upperLineNum(0), caretIdx(0){}

	//2. setCaretIdx() : ĳ�� �ε����� �����ϸ� curNodeIdx�� �� ������ �������� ���� �����ȴ�. 
	void setCaretIdx() {

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






//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static mCaret caret1;
	
	switch (Message) {

	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}
	
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		//caret1.show();
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		mString m1(TEXT("abcdefghijklmnopABCDEFGH������IJKLMNOPqrstuv1234567890ABCZQLEMONHIHIHI"));
		mTextSource mts1;

		mts1.addText(TEXT("HI"));
		mts1.addText(m1);
		mts1.show();
		mts1.insertTextAt(0, L"77777"); //�ε������� ��ġ�� ���ٰ� ���� �ȴ�. ù������ 0���� ������ 1�� �ι�° ����
		mts1.show();
		mts1.eraseTextAt(2);
		mts1.show();

		int strWidth = getMstrPixelWidthFull(hdc, m1);
		int tempCharWidth = strWidth / m1.getLength();

		int t = getScreenEndIdx(hdc, m1, rect.right, 0, tempCharWidth, 1);
		textOutCustom(hdc, m1, 0, t, 0, 0);
		
		
		
		
		/*printf("====>������ ���� �ε���: %d \n", t);
		printf("ȭ���� ���� ������: %d \n", rect.right);
		printf("��� �����ϳ� ���� ���� : %d \n", tempCharWidth);
		printf("���� ������ estWordNum: %d \n", rect.right / tempCharWidth);
		printf("���ڿ��� ���� ����: %d \n", strWidth);*/
		EndPaint(hwnd, &ps);
		break;
	}





























		//======================�������� �ܼ� �����ֱ�======================== 
	case WM_DESTROY: {

		PostQuitMessage(0);
		break;
	}

					 /* All other messages (a lot of them) are processed using default procedures */
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}


































//-------------------------------------------------WIN MAIN-------------------------------------------------------------------
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	_wsetlocale(LC_ALL, _T("korean"));

	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

			 /* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = _T("WindowClass");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, _T("WindowClass"), _T("Caption"), WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	/*
	This is the heart of our program where all input is processed and
	sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
	this loop will not produce unreasonably high CPU usage
	*/
	while (GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}