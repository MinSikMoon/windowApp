
#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <iostream>
using namespace std;





//================================INNER METHODS==================================

	//0. ���� ���� ������
	CONST TCHAR* STANDARD_LENG= "ABD";   
	
	//1. �� ����� �� ������ �Ǿ�� �ϴ��� ������ �ִ� �Լ�
	LONG getSentenceLength(LONG windowLength, LONG strLength){
		//strLength / windowLength �ϸ� �ѵ��� ����� ���� �Ǿ�� �ϴ��� ��������.
		return strLength / windowLength;  
	} 
	
	
	//2.  




//=================================WIN PROC======================================
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//MEMBERS
	HDC hdc;
	PAINTSTRUCT ps;
	SIZE size;
	static RECT rect;
	
	
	
	//------------------------------------------------------------------------------
	switch(Message) {
		//�ܼ� ����
		  case WM_CREATE: {
		  	AllocConsole();
		  	freopen("CONOUT$", "wt", stdout);
       		break;
	 	 }
      
		
		//////////// SIZE ///////////////////////
		case WM_SIZE : {
			//rectangle ���
			GetClientRect(hwnd, &rect); 
			InvalidateRect(hwnd, &rect, TRUE);		
			break;
		} 
				
		
		//////////// PAINT //////////////////////
		case WM_PAINT : {
			//1. DC ������ 
			hdc = BeginPaint(hwnd, &ps);


	//TEST CASE==================================			
			//2. ȭ�� ũ�� �˷��ִ� ���ڿ� �����
			TCHAR screenSize[50];
			wsprintf(screenSize, TEXT("%d, %d  //"), rect.left, rect.right );  
		
			printf("%d, %d  //", rect.left, rect.right );
			//3. ���� ���� �׽�Ʈ�� ���ڿ� ����� 
				//1. ���� ���̸� ���Ѵ�.  
				GetTextExtentPoint(hdc, STANDARD_LENG, _tcslen(STANDARD_LENG), &size);  
				//2. ���ڱ��� �˷��ִ� ���ڿ� �����. 
				TCHAR textLen[50];
				wsprintf(textLen, TEXT("%d \n"), size.cx);  //size�� 8�� ���´�.  
				
			//4. ȭ�� ũ�� ���ڿ��� , ���ڱ��� ���ڿ��� ����غ���. 
			//TCHAR testing[50];
			lstrcat(screenSize, textLen);
			DrawText(hdc, screenSize, _tcslen(screenSize), &rect, DT_LEFT ); 
	//=======================================================================
	
	
			//end: dc ���� 
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
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
