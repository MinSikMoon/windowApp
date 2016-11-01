#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <iostream>
#include "mstring.h"
#include <vector>
using namespace std;
//const TCHAR* testStr = _T("abcdefasfasfasfasfasd가나다라마바사 아자차카타파하fasdfasfasfasdfasdfasasdfasdfdfasdfasfasdfasdfasdfasdfa12345P");


//내부 함수들
//1. 한 문장과 startIdx, endIdx를 넣으면 pixel 너비가 얼마인지 리턴해주는 함수
int getStrPixelWidth(HDC hdc, const TCHAR* targetStr, int startIdx, int endIdx) {
	SIZE size;
	TCHAR* tempStr = subTchar(targetStr, startIdx, endIdx);
	GetTextExtentPoint(hdc, tempStr, _tcslen(tempStr), &size);
	delete tempStr;
	return size.cx;
}

//2 한 문장 pixel길이 리턴해주는 함수
int strPxWidth(HDC hdc, const TCHAR* targetStr) {
	SIZE size;
	GetTextExtentPoint(hdc, targetStr, _tcslen(targetStr), &size);
	return size.cx;
}

//3. textout 커스텀 //지정한 곳부터 textout 해주는 함수 
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

//5. 한 문장, 현재 화면 pixel너비(limit너비), startIdx, 평균 한글자 너비// 4개를 넣으면 endIdx가 몇인지 가르쳐주는 함수
int getEndIdx(HDC hdc, const TCHAR* targetStr, int limitWidth, int startIdx, int avgCharWidth) {
	if (getStrPixelWidth(hdc, targetStr, startIdx, _tcslen(targetStr) - 1) <= limitWidth) {
		return (_tcslen(targetStr) - 1);
	}

	//1. 평균 글자 너비를 이용해서 limitWidth 안에 총 몇글자 정도 들어갈지 예상해본다.
	int estWordNum = limitWidth / avgCharWidth + 1;
	//2. 그 정도 글자로 만든 새로운 문자열 만들기
	int tempEndIdx = startIdx + estWordNum - 1;
	TCHAR* tempStr = subTchar(targetStr, startIdx, tempEndIdx);
	//3. 실제 limitWidth와 비교하면서 가감해본다.
	if (strPxWidth(hdc, tempStr) > limitWidth) { //길면 빼줘야지 한글자씩 
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

	return tempEndIdx - 1; //하나 빼줘봄.
}
//-----------------------------------------------------------------------------------------------------
//전역
int cnt = 0;
FILE* fp;
vector<TCHAR*> v1;
TCHAR* testStr;
//=================================WIN PROC======================================
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//MEMBERS
	//PAINT 관련
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	//글자 개행 관련
	//SIZE size;
	static int avgCharWidth2;
	//OFN 관련
	OPENFILENAME OFN;
	static TCHAR str[100], lpstrFile[100] = TEXT(""); //lpstrFile에 디렉터리 정보가 저장된다. 
	TCHAR filter[] = TEXT("\0텍스트 문서(.txt) \0 *.txt \0 모든파일 \0 *.* \0");
	//파일 관련_WINDOW API
	HANDLE hFile;
	//DWORD dwRead;
	//파일 관련_C_API
	//TCHAR buf[1024];
	char buf[1024];
	//------------------------------------------------------------------------------
	switch (Message) {
		//콘솔 띄우기
	case WM_CREATE: {
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);
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
											 //우리는 여기서 파일을 열게 아니므로 파일의 주소만 필요하다.
											 //파일의 경로는 OFN.lpstrFile에 담겨있다. 
											 //이제 파일의 주소를 알았으니, 이 파일을 연다. 
			CloseHandle(hFile); //파일 핸들을 닫는다. // 파일을 닫는다. 
		}
		//1. 이제 파일을 열어본다. 
		//fp = fopen_s(&fp, OFN.lpstrFile, "r");
		_wfopen_s(&fp, OFN.lpstrFile, TEXT("rt")); //파일 읽기 모드로 오픈
												   //2. loop을 돌면서 한행씩 vector에 넣어준다.
		while (fgets(buf, 1024, fp) != NULL) {
			TCHAR* temp = toWC(buf); //buf가 mbcs기반이니까 모두 2바이트짜리 wchar_t 배열로 만들어준다. 
			v1.push_back(temp);
			cnt++;
		}
		//3. test로 vector[0]을 buf에 복사해준다. 
		//swprintf_s(testStr, 1024, TEXT("%ls"), v1[1]);
		_tprintf(_T("%ls"), v1[2]);
		break;
	}


					//화면 크기 바뀔때 rect 얻어오기 //화면 그리는 메시지 강제 호출 
	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

				  //그리기 
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