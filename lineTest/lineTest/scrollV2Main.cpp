#include "main_header.h"

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
		curScreenLineNum = rect.bottom / wordHeight; //현재 화면에 몇 문장을 출력할 수 있는지 계산
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	//3. 스크롤 메시지 관련
	case WM_VSCROLL: {
		yInc = mScrollSwitches(wParam, wordHeight, curScreenLineNum, yPos, yMax);	/*	printf("====>현재 yMax는 %d \n",yMax);		printf("현재 yPos는 %d \n", yPos);		printf("현재 화면 크기는 %d \n", rect.bottom);		printf("한 화면에 몇줄? => %d줄 \n", curScreenLineNum);*/
		ScrollWindow(hwnd, 0, -yInc, NULL, NULL);
		SetScrollPos(hwnd, SB_VERT, yPos, TRUE);
		break;
	}

	//3. 그리기 
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		//map<int, int> nodeLineNum; //노드 하나당 몇 라인을 쓰는지 저장할 맵

		//노드 안에서 몇째줄인가
		int out_lineNum =0;
		g_totalLineCnt = autoLineSwitch(hdc, v1, rect.right, wordHeight, yPos, nodeLineNum, lc1); //개행 함수 //총 몇 줄 나오는지 리턴해줌.
		textHeight = (g_totalLineCnt)* wordHeight; //텍스트의 높이 계산// 이게 yMax가 된다. 

		nodeIdx = getNodeIdx(nodeLineNum, yPos, wordHeight, out_lineNum);//화면의 첫번째 문장이 몇번째 노드의 문장인지 알아보자.
		//여기서 캐럿 정보도 갱신해줌. 
		
		//printf("첫 문장은 %d번째 노드 소속입니다. %d번째 인덱스 줄 \n", nodeIdx, out_lineNum);
		//loopCnt: 현재 총 라인수, // nodeLineNum[i]: i번째 노드 라인수
		//printf("0노드의 라인 수 %d \n", nodeLineNum[0]);
		printf("4번째 노드의 4번 인덱스줄은 몇 글자? => %d/ 첫인덱스? = %d, 끝 인덱스 = %d \n", lc1.getWordCnt(4, 4), lc1.getFirstIdx(4,4), lc1.getLastIdx(4,4));
		
															
															
		//비례 스크롤바 호출
		yMax = textHeight;
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



	//---------------------------캐럿관련
	//1. 포커스 잃을 때
	case WM_KILLFOCUS: {
		DestroyCaret(); //포커스를 잃으면 현재 윈도우에 장착된 캐럿을 없앤다. 
		break;
	}
	
	//2. 포커스 받을 때 //캐럿은 이 때 만들어야 한다. 
	case WM_SETFOCUS: {
		CreateCaret(hwnd, NULL, 2, wordHeight); //캐럿 만들기// 2번째 비트맵이 NULL이니까, 캐럿은 2, 16 폭과 높이(문자 높이)로 만들어진다. 
		ShowCaret(hwnd); //앞에서 hwnd에 캐럿을 만들었으므로, ShowCaret()하면 캐럿이 보여진다.(깜빡깜빡)
		break;
	}
	
	//3. 방향키 움직이는 데로 옮겨보자.
	case WM_KEYDOWN: {
		switch (wParam) {
		//1. 상 //현재 라인을 --시킨다. --라인이 0보다 작아지면 이전 노드의 마지막으로 이동, 노드도 0, 줄도 0이면 안 올라간다. 
		case VK_UP: {

			break;
		}
		//2. 하
		case VK_DOWN: {

			break;
		}
		//3. 좌
		case VK_LEFT: {

			break;
		}
		//4. 우
		case VK_RIGHT: {

			break;
		}

		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}





	//윈도우 해제
	case WM_DESTROY: {
		FreeConsole();
		fclose(fp);
		for (unsigned int i = 0; i < v1.size(); i++) {
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