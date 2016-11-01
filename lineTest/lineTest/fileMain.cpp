#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <iostream>
#include "mstring.h"
#include <vector>
using namespace std;
//const TCHAR* testStr = _T("abcdefasfasfasfasfasd�����ٶ󸶹ٻ� ������īŸ����fasdfasfasfasdfasdfasasdfasdfdfasdfasfasdfasdfasdfasdfa12345P");


//���� �Լ���
//1. �� ����� startIdx, endIdx�� ������ pixel �ʺ� ������ �������ִ� �Լ�
int getStrPixelWidth(HDC hdc, const TCHAR* targetStr, int startIdx, int endIdx) {
	SIZE size;
	TCHAR* tempStr = subTchar(targetStr, startIdx, endIdx);
	GetTextExtentPoint(hdc, tempStr, _tcslen(tempStr), &size);
	delete tempStr;
	return size.cx;
}

//2 �� ���� pixel���� �������ִ� �Լ�
int strPxWidth(HDC hdc, const TCHAR* targetStr) {
	SIZE size;
	GetTextExtentPoint(hdc, targetStr, _tcslen(targetStr), &size);
	return size.cx;
}

//3. textout Ŀ���� //������ ������ textout ���ִ� �Լ� 
void textOutCustom(HDC hdc, int x, int y, const TCHAR* targetStr, int startIdx, int endIdx) {
	TCHAR* tempStr = subTchar(targetStr, startIdx, endIdx);
	TextOut(hdc, x, y, tempStr, _tcslen(tempStr));
	delete tempStr;
}

//4. toWc
wchar_t* toWC(const char *str_param) {
	size_t convertedChars = 0;
	size_t str_param_len = strlen(str_param) + 1;

	wchar_t *wc = new wchar_t[str_param_len];
	setlocale(LC_ALL, "");
	mbstowcs_s(&convertedChars, wc, str_param_len, str_param, _TRUNCATE);

	return wc;
}

//5. �� ����, ���� ȭ�� pixel�ʺ�(limit�ʺ�), startIdx, ��� �ѱ��� �ʺ�// 4���� ������ endIdx�� ������ �������ִ� �Լ�
int getEndIdx(HDC hdc, const TCHAR* targetStr, int limitWidth, int startIdx, int avgCharWidth) {
	if (getStrPixelWidth(hdc, targetStr, startIdx, _tcslen(targetStr) - 1) <= limitWidth) {
		return (_tcslen(targetStr) - 1);
	}

	//1. ��� ���� �ʺ� �̿��ؼ� limitWidth �ȿ� �� ����� ���� ���� �����غ���.
	int estWordNum = limitWidth / avgCharWidth + 1;
	//2. �� ���� ���ڷ� ���� ���ο� ���ڿ� �����
	int tempEndIdx = startIdx + estWordNum - 1;
	TCHAR* tempStr = subTchar(targetStr, startIdx, tempEndIdx);
	//3. ���� limitWidth�� ���ϸ鼭 �����غ���.
	if (strPxWidth(hdc, tempStr) > limitWidth) { //��� ������� �ѱ��ھ� 
												 //tempEndIdx--;
		while (1) {
			tempEndIdx--;
			TCHAR* cleaner = tempStr;
			tempStr = subTchar(targetStr, startIdx, tempEndIdx);
			delete cleaner;
			if (strPxWidth(hdc, tempStr) <= limitWidth)
				break;
		}
	}
	else if (strPxWidth(hdc, tempStr) < limitWidth) {
		tempEndIdx++;
		while (1) {
			tempEndIdx++;
			TCHAR* cleaner = tempStr;
			tempStr = subTchar(targetStr, startIdx, tempEndIdx);
			delete cleaner;
			if (strPxWidth(hdc, tempStr) >= limitWidth)
				break;
		}
	}
	delete tempStr;

	return tempEndIdx - 1; //�ϳ� ���ຽ.
}
//-----------------------------------------------------------------------------------------------------
//����
int cnt = 0;
FILE* fp;
vector<TCHAR*> v1;
TCHAR* testStr;
//=================================WIN PROC======================================
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//MEMBERS
	//PAINT ����
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	//���� ���� ����
	//SIZE size;
	static int avgCharWidth2;
	//OFN ����
	OPENFILENAME OFN;
	static TCHAR str[100], lpstrFile[100] = TEXT(""); //lpstrFile�� ���͸� ������ ����ȴ�. 
	TCHAR filter[] = TEXT("\0�ؽ�Ʈ ����(.txt) \0 *.txt \0 ������� \0 *.* \0");
	//���� ����_WINDOW API
	HANDLE hFile;
	//DWORD dwRead;
	//���� ����_C_API
	//TCHAR buf[1024];
	char buf[1024];
	//------------------------------------------------------------------------------
	switch (Message) {
		//�ܼ� ����
	case WM_CREATE: {
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hwnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 100;
		OFN.lpstrInitialDir = TEXT("."); //���� ���丮�� INITIAL�̴�. 
		if (GetOpenFileName(&OFN) != 0) {
			_stprintf_s(str, TEXT("%s ������ ���ڽ��ϱ�?"), OFN.lpstrFile); //str�� �޽��� �ڽ��� ���ؼ� ���� ���� //
			MessageBox(hwnd, str, TEXT("���⼱��"), MB_OK);
		}
		hFile = CreateFile(lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //���� ������ ����, ���Ͽ� ���� �ڵ��� �����´�. 

		if (hFile != INVALID_HANDLE_VALUE) { //���������� ���� �ڵ��� �޾Ҵٸ�
											 //�츮�� ���⼭ ������ ���� �ƴϹǷ� ������ �ּҸ� �ʿ��ϴ�.
											 //������ ��δ� OFN.lpstrFile�� ����ִ�. 
											 //���� ������ �ּҸ� �˾�����, �� ������ ����. 
			CloseHandle(hFile); //���� �ڵ��� �ݴ´�. // ������ �ݴ´�. 
		}
		//1. ���� ������ �����. 
		//fp = fopen_s(&fp, OFN.lpstrFile, "r");
		_wfopen_s(&fp, OFN.lpstrFile, TEXT("rt")); //���� �б� ���� ����
												   //2. loop�� ���鼭 ���྿ vector�� �־��ش�.
		while (fgets(buf, 1024, fp) != NULL) {
			TCHAR* temp = toWC(buf); //buf�� mbcs����̴ϱ� ��� 2����Ʈ¥�� wchar_t �迭�� ������ش�. 
			v1.push_back(temp);
			cnt++;
		}
		//3. test�� vector[0]�� buf�� �������ش�. 
		//swprintf_s(testStr, 1024, TEXT("%ls"), v1[1]);
		_tprintf(_T("%ls"), v1[2]);
		break;
	}


					//ȭ�� ũ�� �ٲ� rect ������ //ȭ�� �׸��� �޽��� ���� ȣ�� 
	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

				  //�׸��� 
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);
		int curWndWidth = rect.right;
		avgCharWidth2 = strPxWidth(hdc, testStr) / _tcslen(testStr);
		int sentenceNum = strPxWidth(hdc, testStr) / curWndWidth + 1;
		int startIdx = 0;
		int lastIdx = 0;

		if (curWndWidth < strPxWidth(hdc, testStr)) {
			for (int i = 0; i<sentenceNum; i++) {
				lastIdx = getEndIdx(hdc, testStr, rect.right, lastIdx, avgCharWidth2);
				textOutCustom(hdc, 0, i * 16, testStr, startIdx, lastIdx);
				startIdx = lastIdx + 1;
			}
		}
		else {
			TextOut(hdc, 0, 0, testStr, getLen(testStr));
		}
		EndPaint(hwnd, &ps);
		break;
	}

				   /* Upon destruction, tell the main thread to stop */
	case WM_DESTROY: {
		FreeConsole();
		PostQuitMessage(0);
		break;
	}

					 /* All other messages (a lot of them) are processed using default procedures */
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
		500, /* width */
		500, /* height */
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