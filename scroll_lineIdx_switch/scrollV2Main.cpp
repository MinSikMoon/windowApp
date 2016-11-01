#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <iostream>
#include "mstring.h"
#include <vector>
#include "mWordPixel.h"
#include <map>
using namespace std;


//전역변수======================================================================================
FILE* fp;
TCHAR* testStr;
vector<TCHAR*> v1;
TCHAR* tempStr;
//글자 너비, 높이 관련
int wordHeight = 16;
int avgCharWidth2;
int textHeight;
int curScreenLineNum; //현재 화면에 보이는 글자 라인수 
//스크롤 관련 전역 변수
int xMax, yMax; //스크롤 범위 최대치 지정
int xPos, yPos; //썸의 위치



//=================================WIN PROC======================================
				/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//-----------------------------------------------------------------------MEMBERS
	//----------------그리기 관련
	HDC hdc;
	PAINTSTRUCT ps;
	//SIZE size;
	static RECT rect;
	//-----------------OFN 관련
	OPENFILENAME OFN;
	//------------------파일 관련 WINDOW API
	HANDLE hFile;
	static TCHAR str[100], lpstrFile[100] = TEXT(""); //lpstrFile에 디렉터리 정보가 저장된다. 
	TCHAR filter[] = TEXT("\0텍스트 문서(.txt) \0 *.txt \0 모든파일 \0 *.* \0");
	char buf[1024];
	//-----------------스크롤 관련
	//int xInc;
	SCROLLINFO si;
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
		GetClientRect(hwnd, &rect); //화면 사이즈가 나오고 화면에 몇줄 들어갈 수 있는지도 구할 수 있겠지.
		curScreenLineNum = rect.bottom / wordHeight;
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	//3. 스크롤 메시지 관련
	case WM_VSCROLL: {
		yInc = 0;
		
		switch (LOWORD(wParam)) {
		case SB_LINEUP: {
			yInc = -wordHeight;
			break;
		}
		case SB_PAGEUP: {
			yInc = -20 * wordHeight;
			break;
		}
		case SB_LINEDOWN: {
			yInc = wordHeight;
			if (yPos + (curScreenLineNum*wordHeight) >= yMax) {
				yInc = 0;
			}
			break;
		}
		case SB_PAGEDOWN: {
			yInc = 20 * wordHeight;
			if (yPos + yInc >= yMax) {
				int frontWordNum = yPos / wordHeight;
				int tempLoopCnt = yMax / wordHeight;
				int tempCnt = tempLoopCnt - frontWordNum - curScreenLineNum;
;				yInc = tempCnt * wordHeight;
			}
			break;
		}
		case SB_THUMBTRACK: {
			int temp_yPos = HIWORD(wParam); //thumb의 위치
			int temp_yInc = (temp_yPos - yPos);
			int temp_lineJump = temp_yInc / wordHeight;
			int temp_jumpMore = temp_yInc % wordHeight;
			int halfCharWidth = wordHeight / 2;

			if (temp_jumpMore >= halfCharWidth) {
				temp_lineJump++;
			}

			yInc = temp_lineJump * wordHeight;
		
			/*printf("현재 스크롤바 위치는 %d \n", yPos);*/
			//printf("썸 위치: %d \n", HIWORD(wParam));
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
		yPos += yInc;  //새로운 썸의 위치가 여기서 계산된다. 

	
		printf("====>현재 yMax는 %d \n",yMax);
		printf("현재 yPos는 %d \n", yPos);
		printf("현재 화면 크기는 %d \n", rect.bottom);
		printf("한 화면에 몇줄? => %d줄 \n", curScreenLineNum);
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

		//실험
		map<int, int> nodeLineNum; //한 노드당 몇 라인을 차지하느냐.

		for (i = 0; i < v1.size(); i++) { //노드 하나당 돈다. 
			testStr = NULL;
			testStr = v1[i];
			avgCharWidth2 = strPxWidth(hdc, testStr) / _tcslen(testStr);
			sentenceNum = strPxWidth(hdc, testStr) / curWndWidth + 1;
			startIdx = 0;
			//실험
			int nodeLineCnt = 0;

			if (curWndWidth < strPxWidth(hdc, testStr)) {
				for (j = 0; j < sentenceNum; j++) {
					lastIdx = getEndIdx(hdc, testStr, rect.right, startIdx, avgCharWidth2);
					textOutCustom(hdc, 0, loopCnt*wordHeight - yPos, testStr, startIdx, lastIdx);
					startIdx = lastIdx + 1;
					loopCnt++;
					nodeLineCnt++;
				}
			}
			else {
				TextOut(hdc, 0, loopCnt*wordHeight - yPos, testStr, getLen(testStr));
				loopCnt++;
				nodeLineCnt++;
			}
			//이제 노드 하나당 몇 라인을 차지하는지 알았다. 
			nodeLineNum[i] = nodeLineCnt;
		}

		textHeight = (loopCnt)* wordHeight; //텍스트의 높이 계산// 이게 yMax가 된다.
		
		for (int i = 0; i < nodeLineNum.size(); i++) {
			printf("%d 번째 노드 라인 수 : %d \n", i, nodeLineNum[i]);
		}
		printf("총 라인수는 => %d \n", loopCnt);
		//화면의 첫번째 문장이 몇번째 노드의 문장인지 알아보자. 
		int sum = 0;
		int tempSum = 0;
		int frontLine = yPos / wordHeight; //앞에 몇 문장 있는지
		printf("->현재 앞에는 %d개 문장이 있다. \n", frontLine);

		int nodeIdx;
		for (int i = 0; i < nodeLineNum.size(); i++) {
			
			tempSum += nodeLineNum[i]; 
			if (frontLine >= sum &&frontLine < tempSum) {
				nodeIdx = i;
				break;
			}
			sum = tempSum;
		}
		printf("첫 문장은 %d번째 노드 소속입니다. \n", nodeIdx);

		//비례 스크롤바 호출
		yMax = textHeight;
		int alwaysLittleThanMax = yMax - 1;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL; //sif nocontroll 없애주니까 첫번째 부터 스크롤바 나온다. //아마 ypos가 0이면 숨기는듯.
		si.nMin = 0;
		si.nMax = yMax;
		si.nPage = rect.bottom; //화면의 높이가 yMax보다 textHeight보다 커지면 사라진다. 

		if (rect.bottom >= yMax) { //항상 켜있도록 
			si.nPage = textHeight - yPos;
		}
		si.nPos = yPos;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		

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