#include <windows.h>
#include <tchar.h>

//���� ����
TCHAR buf[1024];

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	//PAINT ����
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	//OFN����
	OPENFILENAME OFN;
	static TCHAR str[100], lpstrFile[100] = TEXT(""); //lpstrFile�� ���͸� ������ ����ȴ�. 
	TCHAR filter[] = TEXT("\0�ؽ�Ʈ ����(.txt) \0 *.txt \0 ������� \0 *.* \0");
	//���ϰ���
	HANDLE hFile;
	DWORD dwRead;

	///////////////////////////////////////////////////////////////
	switch (Message) {
		//�����ɶ� ofn �������
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
			_stprintf_s(str, TEXT("%s ������ ���ڽ��ϱ�?"), OFN.lpstrFile); //str�� �޽��� �ڽ��� ���ؼ� ���� ���� //
			MessageBox(hwnd, str, TEXT("���⼱��"), MB_OK);
		}
		hFile = CreateFile(lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //���� ������ ����, ���Ͽ� ���� �ڵ��� �����´�. 

		if (hFile != INVALID_HANDLE_VALUE) { //���������� ���� �ڵ��� �޾Ҵٸ�
			ReadFile(hFile, buf, 1024, &dwRead, NULL); //1. ��� ������ �ڵ�// 2. �����͸� ������ ����// 3. �а��� �ϴ� ��, �翬�� �ι�°���� ����
													   //4. ������ ��������, DWORD�� ����� ������ ����ȴ�. ������ �������� �����̹Ƿ� �׻� ������ �����־���. //5. �񵿱� ������� ���� �����Ƿ� NULL
			CloseHandle(hFile); //���� �ڵ��� �ݴ´�. // ������ �ݴ´�. 
			InvalidateRect(hwnd, NULL, TRUE); //ȭ�� ��ü�� �ٽ� �׷��ش�. //WM_PAINT ȣ��
		}
		break;
	}
	case WM_PAINT: {
		//1. hdc���
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect); //rect�� ����� ���Ѵ�.
		DrawText(hdc, buf, -1, &rect, DT_WORDBREAK); //����ǰ� buf�� rect�� ���缭 drawtext���ش�.
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
