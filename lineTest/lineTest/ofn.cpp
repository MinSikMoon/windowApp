#include <windows.h>
#include <tchar.h>

//전역 변수
TCHAR buf[1024];

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	//PAINT 관련
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	//OFN관련
	OPENFILENAME OFN;
	static TCHAR str[100], lpstrFile[100] = TEXT(""); //lpstrFile에 디렉터리 정보가 저장된다. 
	TCHAR filter[] = TEXT("\0텍스트 문서(.txt) \0 *.txt \0 모든파일 \0 *.* \0");
	//파일관련
	HANDLE hFile;
	DWORD dwRead;

	///////////////////////////////////////////////////////////////
	switch (Message) {
		//생성될때 ofn 띄워보기
	case WM_CREATE: {
		GetClientRect(hwnd, &rect);
		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = hwnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 100;
		OFN.lpstrInitialDir = TEXT(".");
		if (GetOpenFileName(&OFN) != 0) {
			_stprintf_s(str, TEXT("%s 파일을 열겠습니까?"), OFN.lpstrFile); //str은 메시지 박스를 위해서 만든 변수 //
			MessageBox(hwnd, str, TEXT("열기선택"), MB_OK);
		}
		hFile = CreateFile(lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //기존 파일을 열고, 파일에 대한 핸들을 가져온다. 

		if (hFile != INVALID_HANDLE_VALUE) { //정상적으로 파일 핸들을 받았다면
			ReadFile(hFile, buf, 1024, &dwRead, NULL); //1. 대상 파일의 핸들// 2. 데이터를 저장할 버퍼// 3. 읽고자 하는 양, 당연히 두번째보다 이하
													   //4. 실제로 읽혀진양, DWORD로 선언된 변수에 저장된다. 참조로 읽혀지는 변수이므로 항상 변수로 만들어둬야함. //5. 비동기 입출력을 하지 않으므로 NULL
			CloseHandle(hFile); //파일 핸들을 닫는다. // 파일을 닫는다. 
			InvalidateRect(hwnd, NULL, TRUE); //화면 전체를 다시 그려준다. //WM_PAINT 호출
		}
		break;
	}
	case WM_PAINT: {
		//1. hdc얻기
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect); //rect의 사이즈를 구한다.
		DrawText(hdc, buf, -1, &rect, DT_WORDBREAK); //개행되게 buf를 rect에 맞춰서 drawtext해준다.
		EndPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}



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
