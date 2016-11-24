//�ؽ�Ʈ ������ Ŭ����ȭ ��Ű��
#include "newTextEditor03.h"
#include "newCarot02.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;



//���� ��ü 
//�׽�Ʈ ���ؽ�Ʈ������
mTextEditor textEditor(2, 16);
mTextEditor textEditor2(2, 16);
newCarot carot;

//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// ���� ������ 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;

	switch (Message) {
	case WM_CREATE: {
		textEditor.addText(TEXT("�ι�° �ؽ�Ʈ ������ �Դϴ�."));
		break;
	}

	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		//int isBacked = 0;
		textEditor2.mProc(hwnd, Message, wParam, lParam);
		//printf("isbacked = %d \n", isBacked);
		textEditor2.replaceCurText(0); //replace�� carot�� ���� ����Ű�� ���� ������ �ٲ���� �Ѵ�. �̰� ���ľ���. 
		textEditor2.changeStartPoint(100, 100);
		SendMessage(hwnd, WM_PAINT, NULL, NULL); //lc�� ���Ӱ� ���ĵȴ�. 
												 //textEditor2.carotMoveRight(); //right�� ������ �ٽ� ������. 
												 //1. lc�� ���ɷ� �����ȴ�.
												 //2. ���Ӱ� ������ lc�� ���ο� �����ε����� frontwordnum, cIdx�� ���Ӱ� �����ȴ�.
												 //3. �̰� �������� ���ο� cIdx���ϱ�.

		textEditor2.caretInput();
		printf("���� sendmessage�� ���� print ���� �ϰ� �ٽ� invalidate \n ");
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

		textEditor2.showAllText(hdc, 100, 100, 100); //���⼭ �׻� 

		SetCaretPos(textEditor2.getCaretXpixel(hdc), textEditor2.getCaretYpixel()); //�齺���̽� ��� ������ �齺���̽� �ϸ� ������. 
		textEditor2.getCarotInfo();
		EndPaint(hwnd, &ps);
		break;
	}




	//======================�� �� ���̽�======================== 
	//ĳ�� ���� �׽�Ʈ
	//---------------------------ĳ������
	//1. ��Ŀ�� ���� ��
	case WM_KILLFOCUS: {
		DestroyCaret(); //��Ŀ���� ������ ���� �����쿡 ������ ĳ���� ���ش�. 
		break;
	}

	//2. ��Ŀ�� ���� �� //ĳ���� �� �� ������ �Ѵ�. 
	case WM_SETFOCUS: {
		CreateCaret(hwnd, NULL, 2, 16); //ĳ�� �����// 2��° ��Ʈ���� NULL�̴ϱ�, ĳ���� 2, 16 ���� ����(���� ����)�� ���������. 
		ShowCaret(hwnd); //�տ��� hwnd�� ĳ���� ��������Ƿ�, ShowCaret()�ϸ� ĳ���� ��������.(��������)
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