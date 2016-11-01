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
//글자 너비, 높이 관련
int wordHeight = 16;
int avgCharWidth2;
int textHeight;
//스크롤 관련 전역 변수
int xMax, yMax; //스크롤 범위 최대치 지정
int xPos, yPos; //썸의 위치

				//=================================WIN PROC======================================
				/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//-----------------------------------------------------------------------MEMBERS
	//그리기 관련
	HDC hdc;
	PAINTSTRUCT ps;
	//SIZE size;
	static RECT rect;
	//OFN 관련
	OPENFILENAME OFN;
	//파일 관련 WINDOW API
	HANDLE hFile;
	static TCHAR str[100], lpstrFile[100] = TEXT(""); //lpstrFile에 디렉터리 정보가 저장된다. 
	TCHAR filter[] = TEXT("\0텍스트 문서(.txt) \0 *.txt \0 모든파일 \0 *.* \0");
	char buf[1024];
	//스크롤 관련
	//int xInc;
	int yInc; //썸의 이동거리

			  //----------------------------------------------------------------------MESSAGE LOGIC
	switch (Message) {
		//1. 윈도우 생성시
	case WM_CREATE: {
		//1.1 콘솔 열기
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
		//1.2 파일 대화상자 열기 / 파일 핸들 얻어오기/ 파일 경로 얻어오기
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hwnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 100;
		OFN.lpstrInitialDir = TEXT("."); //현재 디렉토리가 INITIAL이다. 
		if (GetOpenFileName(&OFN) != 0) {
			_stprintf_s(str, TEXT("%s 파일을 열겠습니까?"), OFN.lpstrFile); //str은 메시지 박스를 위해서 만든 변수 //
			MessageBox(hwnd, str, TEXT("열기선택"), MB_OK);
		}
		hFile = CreateFile(lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //기존 파일을 열고, 파일에 대한 핸들을 가져온다. 

		if (hFile != INVALID_HANDLE_VALUE) { //정상적으로 파일 핸들을 받았다면
			CloseHandle(hFile); //파일 핸들을 닫는다. // 파일을 닫는다. 
		}
		//1.3 C방식으로 파일을 읽기 모드로 열기
		_wfopen_s(&fp, OFN.lpstrFile, TEXT("rt")); //파일 읽기 모드로 오픈
												   //1.4 loop 돌면서 한행씩 vector에 넣어보기
		while (fgets(buf, 1024, fp) != NULL) {
			TCHAR* temp = toWC(buf);
			//printf("%d \n", _tcslen(temp));
			v1.push_back(temp);
		}
		//썸 위치 초기화
		yPos = 0;
		break;
	}

					//2. 화면 크기 바뀔때 rect 얻어오기 //화면 그리는 메시지 강제 호출 //전체 텍스트의 높이도 알아야 한다. // 현재 창에 비친 텍스트의 높이도 알아야
	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

				  //3. 스크롤 메시지 관련
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
			printf("썸 위치: %d \n", HIWORD(wParam));
			break;
		}

		}
		//스크롤 위치 필터
		if (yPos + yInc < 0) {
			yInc = -yPos;
		}
		if (yPos + yInc > yMax) {
			yInc = yMax - yPos;
		}
		//새로운 썸의 위치 계산
		yPos += yInc;

		ScrollWindow(hwnd, 0, -yInc, NULL, NULL);
		SetScrollPos(hwnd, SB_VERT, yPos, TRUE);

		break;
	}

					 //3. 그리기 
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
		textHeight = (loopCnt)* wordHeight; //텍스트의 높이 계산// 이게 yMax가 된다.
											//만약 화면의 높이가 텍스트높이보다 짧아지면 스크롤 생성
											//printf("%d, %d \n", textHeight, rect.bottom);
		if (rect.bottom < textHeight) {
			//스크롤 생성
			//yPos = 0; //썸의 시작위치는 0이다.
			yMax = textHeight - rect.bottom;
			SetScrollRange(hwnd, SB_VERT, 0, yMax, TRUE);
			SetScrollPos(hwnd, SB_VERT, yPos, TRUE); //기본은 0에서 시작
		}
		else {
			SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE); //스크롤바가 숨겨진다. 
		}




		EndPaint(hwnd, &ps);
		break;
	}


				   //윈도우 해제
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