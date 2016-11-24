//텍스트 에디터 클래스화 시키기
#include "newTextEditor03.h"
#include "newCarot02.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;



//전역 객체 
//테스트 엠텍스트에디터
mTextEditor textEditor(2, 16);
mTextEditor textEditor2(2, 16);
newCarot carot;

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// 전역 변수들 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;

	switch (Message) {
	case WM_CREATE: {
		textEditor.addText(TEXT("두번째 텍스트 에디터 입니다."));
		break;
	}

	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		//int isBacked = 0;
		textEditor2.mProc(hwnd, Message, wParam, lParam);
		//printf("isbacked = %d \n", isBacked);
		textEditor2.replaceCurText(0); //replace는 carot이 현재 가리키는 곳의 정보를 바꿔줘야 한다. 이걸 고쳐야함. 
		textEditor2.changeStartPoint(100, 100);
		SendMessage(hwnd, WM_PAINT, NULL, NULL); //lc에 새롭게 정렬된다. 
												 //textEditor2.carotMoveRight(); //right의 개념을 다시 만들자. 
												 //1. lc가 새걸로 장착된다.
												 //2. 새롭게 장착된 lc로 새로운 라인인덱스와 frontwordnum, cIdx가 새롭게 장착된다.
												 //3. 이걸 기준으로 새로운 cIdx구하기.

		textEditor2.caretInput();
		printf("위에 sendmessage로 강제 print 해줌 하고 다시 invalidate \n ");
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}


	case WM_SIZE: {
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		textEditor2.showAllText(hdc, 100, 100, 100); //여기서 항상 

		SetCaretPos(textEditor2.getCaretXpixel(hdc), textEditor2.getCaretYpixel()); //백스페이스 기능 때문에 백스페이스 하면 오류남. 
		textEditor2.getCarotInfo();
		EndPaint(hwnd, &ps);
		break;
	}




	//======================그 외 케이스======================== 
	//캐럿 관련 테스트
	//---------------------------캐럿관련
	//1. 포커스 잃을 때
	case WM_KILLFOCUS: {
		DestroyCaret(); //포커스를 잃으면 현재 윈도우에 장착된 캐럿을 없앤다. 
		break;
	}

	//2. 포커스 받을 때 //캐럿은 이 때 만들어야 한다. 
	case WM_SETFOCUS: {
		CreateCaret(hwnd, NULL, 2, 16); //캐럿 만들기// 2번째 비트맵이 NULL이니까, 캐럿은 2, 16 폭과 높이(문자 높이)로 만들어진다. 
		ShowCaret(hwnd); //앞에서 hwnd에 캐럿을 만들었으므로, ShowCaret()하면 캐럿이 보여진다.(깜빡깜빡)
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
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
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hbrBackground = (HBRUSH)NULL_BRUSH;
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