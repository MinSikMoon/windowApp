#include "main_header.h"

//=================================WIN PROC======================================
				/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//-----------------------------------------------------------------------MEMBERS
	//----------------�׸��� ����
	HDC hdc;
	PAINTSTRUCT ps;
	//SIZE size;
	static RECT rect;
	//-----------------OFN ����
	OPENFILENAME OFN;
	//------------------���� ���� WINDOW API
	HANDLE hFile;
	static TCHAR str[100], lpstrFile[100] = TEXT(""); //lpstrFile�� ���͸� ������ ����ȴ�. 
	TCHAR filter[] = TEXT("\0�ؽ�Ʈ ����(.txt) \0 *.txt \0 ������� \0 *.* \0");
	char buf[1024];
	//-----------------��ũ�� ����
	//int xInc;
	SCROLLINFO si;
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
		GetClientRect(hwnd, &rect); //ȭ�� ����� ������ ȭ�鿡 ���� �� �� �ִ����� ���� �� �ְ���.
		curScreenLineNum = rect.bottom / wordHeight; //���� ȭ�鿡 �� ������ ����� �� �ִ��� ���
		InvalidateRect(hwnd, &rect, TRUE);
		break;
	}

	//3. ��ũ�� �޽��� ����
	case WM_VSCROLL: {
		yInc = mScrollSwitches(wParam, wordHeight, curScreenLineNum, yPos, yMax);	/*	printf("====>���� yMax�� %d \n",yMax);		printf("���� yPos�� %d \n", yPos);		printf("���� ȭ�� ũ��� %d \n", rect.bottom);		printf("�� ȭ�鿡 ����? => %d�� \n", curScreenLineNum);*/
		ScrollWindow(hwnd, 0, -yInc, NULL, NULL);
		SetScrollPos(hwnd, SB_VERT, yPos, TRUE);
		break;
	}

	//3. �׸��� 
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		//map<int, int> nodeLineNum; //��� �ϳ��� �� ������ ������ ������ ��

		//��� �ȿ��� ��°���ΰ�
		int out_lineNum =0;
		g_totalLineCnt = autoLineSwitch(hdc, v1, rect.right, wordHeight, yPos, nodeLineNum, lc1); //���� �Լ� //�� �� �� �������� ��������.
		textHeight = (g_totalLineCnt)* wordHeight; //�ؽ�Ʈ�� ���� ���// �̰� yMax�� �ȴ�. 

		nodeIdx = getNodeIdx(nodeLineNum, yPos, wordHeight, out_lineNum);//ȭ���� ù��° ������ ���° ����� �������� �˾ƺ���.
		//���⼭ ĳ�� ������ ��������. 
		
		//printf("ù ������ %d��° ��� �Ҽ��Դϴ�. %d��° �ε��� �� \n", nodeIdx, out_lineNum);
		//loopCnt: ���� �� ���μ�, // nodeLineNum[i]: i��° ��� ���μ�
		//printf("0����� ���� �� %d \n", nodeLineNum[0]);
		printf("4��° ����� 4�� �ε������� �� ����? => %d/ ù�ε���? = %d, �� �ε��� = %d \n", lc1.getWordCnt(4, 4), lc1.getFirstIdx(4,4), lc1.getLastIdx(4,4));
		
															
															
		//��� ��ũ�ѹ� ȣ��
		yMax = textHeight;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL; //sif nocontroll �����ִϱ� ù��° ���� ��ũ�ѹ� ���´�. //�Ƹ� ypos�� 0�̸� ����µ�.
		si.nMin = 0;
		si.nMax = yMax;
		si.nPage = rect.bottom; //ȭ���� ���̰� yMax���� textHeight���� Ŀ���� �������. 

		if (rect.bottom >= yMax) { //�׻� ���ֵ��� 
			si.nPage = textHeight - yPos;
		}
		si.nPos = yPos;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		

		EndPaint(hwnd, &ps);
		break;
	}



	//---------------------------ĳ������
	//1. ��Ŀ�� ���� ��
	case WM_KILLFOCUS: {
		DestroyCaret(); //��Ŀ���� ������ ���� �����쿡 ������ ĳ���� ���ش�. 
		break;
	}
	
	//2. ��Ŀ�� ���� �� //ĳ���� �� �� ������ �Ѵ�. 
	case WM_SETFOCUS: {
		CreateCaret(hwnd, NULL, 2, wordHeight); //ĳ�� �����// 2��° ��Ʈ���� NULL�̴ϱ�, ĳ���� 2, 16 ���� ����(���� ����)�� ���������. 
		ShowCaret(hwnd); //�տ��� hwnd�� ĳ���� ��������Ƿ�, ShowCaret()�ϸ� ĳ���� ��������.(��������)
		break;
	}
	
	//3. ����Ű �����̴� ���� �Űܺ���.
	case WM_KEYDOWN: {
		switch (wParam) {
		//1. �� //���� ������ --��Ų��. --������ 0���� �۾����� ���� ����� ���������� �̵�, ��嵵 0, �ٵ� 0�̸� �� �ö󰣴�. 
		case VK_UP: {

			break;
		}
		//2. ��
		case VK_DOWN: {

			break;
		}
		//3. ��
		case VK_LEFT: {

			break;
		}
		//4. ��
		case VK_RIGHT: {

			break;
		}

		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}





	//������ ����
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