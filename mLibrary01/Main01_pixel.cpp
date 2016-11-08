#include "mHeaders.h"
#include "mString.h"
#include "mTextPixel.h"
#include "mTextSource.h"
#include "mScreenLineContainer.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

class mCaret {
	size_m curNodeIdx; //ĳ���� �Ҽӵ� ��� �ε���
	size_m curLineIdxInNode; //ĳ���� �Ҽӵ� ��� �ȿ����� ���� �ε���
	size_m frontWordNumInNode; //ĳ���տ� ���� � ���ҳ�?
	size_m upperLineNum; //ĳ�� ���� �� ���� �ֳ�?
	size_m caretIdx; //ĳ�� �ε���, strlen(str)+���μ� �� ������ ������.


public:

};



class mScreenTextController {
	//0. ���� ��ü�� ������ �ִ� Ŭ����
	mTextSource textSource;
	map<int, int> nodeLineNum; //���� ������� ������ �ִ��� ������ ������ �ִ� ��
	mScreenLineContainer lc; //���� ���� ���� �ε����� ������ ���� textSource�� ������ ���ڿ� �ε����� ������ �ε���, ���ڼ��� �˼�����.
	
	//1. ��ü ������ ���� ������ 
	size_m wordPixelHeight; // ������ ���̴� �ȼ��� ���ΰ�?
	size_m totalPixelHeight; // ��ü �ؽ�Ʈ�� ���̴� �ȼ��� ���ΰ�? 
	size_m totalLineNum; // ��ü �ؽ�Ʈ�� �� ���ΰ�? 
	size_m upperLineNum; // ȭ�� ���� �߸� ���� �� ���ΰ�?
	size_m restLineNum; // ���� ȭ�鿡 ���̴� �� + ���� ���� ���� totalLineNum - upperLineNum
	
	//2. ȭ�鿡 ��µǴ� ù��° ����� ȭ�� �ǳ��� ������ ���� ���� �ش� ��� �ȿ����� ���� �ε���
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;
	
	

};


//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;

	switch (Message) {

	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}
	
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
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