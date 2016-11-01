#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <iostream>
#include "mstring.h"
#include <vector>
#include "mWordPixel.h"
using namespace std;


//======================================================================================
FILE* fp;
TCHAR* testStr;
vector<TCHAR*> v1;
TCHAR* tempStr;
//���� �ʺ�, ���� ����
int wordHeight = 16;
int avgCharWidth2;
int textHeight;
//��ũ�� ���� ���� ����
int xMax, yMax; //��ũ�� ���� �ִ�ġ ����
int xPos, yPos; //���� ��ġ

				//=================================WIN PROC======================================
				/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//-----------------------------------------------------------------------MEMBERS
	//�׸��� ����
	HDC hdc;
	PAINTSTRUCT ps;
	//SIZE size;
	static RECT rect;
	//OFN ����
	OPENFILENAME OFN;
	//���� ���� WINDOW API
	HANDLE hFile;
	static TCHAR str[100], lpstrFile[100] = TEXT(""); //lpstrFile�� ���͸� ������ ����ȴ�. 
	TCHAR filter[] = TEXT("\0�ؽ�Ʈ ����(.txt) \0 *.txt \0 ������� \0 *.* \0");
	char buf[1024];
	//��ũ�� ����
	//int xInc;
	int yInc; //���� �̵��Ÿ�

			  //----------------------------------------------------------------------MESSAGE LOGIC
	switch (Message) {
		//1. ������ ������
	case WM_CREATE: {
		//1.1 �ܼ� ����
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
		//1.2 ���� ��ȭ���� ���� / ���� �ڵ� ������/ ���� ��� ������
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
			CloseHandle(hFile); //���� �ڵ��� �ݴ´�. // ������ �ݴ´�. 
		}
		//1.3 C������� ������ �б� ���� ����
		_wfopen_s(&fp, OFN.lpstrFile, TEXT("rt")); //���� �б� ���� ����
												   //1.4 loop ���鼭 ���྿ vector�� �־��
		while (fgets(buf, 1024, fp) != NULL) {
			TCHAR* temp = toWC(buf);
			//printf("%d \n", _tcslen(temp));
			v1.push_back(temp);
		}
		//�� ��ġ �ʱ�ȭ
		yPos = 0;
		break;
	}

					//2. ȭ�� ũ�� �ٲ� rect ������ //ȭ�� �׸��� �޽��� ���� ȣ�� //��ü �ؽ�Ʈ�� ���̵� �˾ƾ� �Ѵ�. // ���� â�� ��ģ �ؽ�Ʈ�� ���̵� �˾ƾ�
	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

				  //3. ��ũ�� �޽��� ����
	case WM_VSCROLL: {
		yInc = 0;
		switch (LOWORD(wParam)) {
		case SB_LINEUP: {
			yInc = -1;
			break;
		}
		case SB_PAGEUP: {
			yInc = -20;
			break;
		}
		case SB_LINEDOWN: {
			yInc = 1;
			break;
		}
		case SB_PAGEDOWN: {
			yInc = 20;
			break;
		}
		case SB_THUMBTRACK: {
			int temp_yPos = HIWORD(wParam);
			yInc = temp_yPos - yPos;
			//yInc = HIWORD(wParam) - xPos; //
			printf("�� ��ġ: %d \n", HIWORD(wParam));
			break;
		}

		}
		//��ũ�� ��ġ ����
		if (yPos + yInc < 0) {
			yInc = -yPos;
		}
		if (yPos + yInc > yMax) {
			yInc = yMax - yPos;
		}
		//���ο� ���� ��ġ ���
		yPos += yInc;

		ScrollWindow(hwnd, 0, -yInc, NULL, NULL);
		SetScrollPos(hwnd, SB_VERT, yPos, TRUE);

		break;
	}

					 //3. �׸��� 
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		int curWndWidth = rect.right;
		int startIdx = 0;
		int lastIdx = 0;
		int sentenceNum;
		int i, j;
		int loopCnt = 0;
		for (i = 0; i < v1.size(); i++) {
			testStr = NULL;
			testStr = v1[i];
			avgCharWidth2 = strPxWidth(hdc, testStr) / _tcslen(testStr);
			sentenceNum = strPxWidth(hdc, testStr) / curWndWidth + 1;
			startIdx = 0;

			if (curWndWidth < strPxWidth(hdc, testStr)) {
				for (j = 0; j < sentenceNum; j++) {
					lastIdx = getEndIdx(hdc, testStr, rect.right, startIdx, avgCharWidth2);
					textOutCustom(hdc, 0, loopCnt*wordHeight - yPos, testStr, startIdx, lastIdx);
					startIdx = lastIdx + 1;
					loopCnt++;
				}
			}
			else {
				TextOut(hdc, 0, loopCnt*wordHeight - yPos, testStr, getLen(testStr));
				loopCnt++;
			}
		}
		textHeight = (loopCnt)* wordHeight; //�ؽ�Ʈ�� ���� ���// �̰� yMax�� �ȴ�.
											//���� ȭ���� ���̰� �ؽ�Ʈ���̺��� ª������ ��ũ�� ����
											//printf("%d, %d \n", textHeight, rect.bottom);
		if (rect.bottom < textHeight) {
			//��ũ�� ����
			//yPos = 0; //���� ������ġ�� 0�̴�.
			yMax = textHeight - rect.bottom;
			SetScrollRange(hwnd, SB_VERT, 0, yMax, TRUE);
			SetScrollPos(hwnd, SB_VERT, yPos, TRUE); //�⺻�� 0���� ����
		}
		else {
			SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE); //��ũ�ѹٰ� ��������. 
		}




		EndPaint(hwnd, &ps);
		break;
	}


				   //������ ����
	case WM_DESTROY: {
		FreeConsole();
		fclose(fp);
		for (int i = 0; i < v1.size(); i++) {
			delete v1[i];
		}
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