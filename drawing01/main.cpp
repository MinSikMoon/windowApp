#include <windows.h>

/* This is where all the input to the window goes to */
//1. lbutton : �Ϲ� ��
//2. rbutton : �Ķ� ��
//3. l+r button : �β��� ��
//4. mouseMove : ���׸���
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	//MEMBERS
	HDC hdc;
	HPEN oldPen, newPen;
	static BOOL isDrawing = FALSE; 
	static BOOL isRdown = FALSE;
	static BOOL isLdown = FALSE;
	static BOOL isMovingRdown = FALSE;
	static BOOL isMovingLdown = FALSE;
	static DWORD blueColor = RGB(0,0,255);
	//�������� 0 ���� �Ǵϱ�. ���� ���� ���� 
	static int x,y;
	
	switch(Message) {
		//1. �ϴ� ���콺 ��ư�� �ö󰡸� is clicked false���ֱ�
		case WM_LBUTTONUP : {
			isDrawing = FALSE;
			isLdown = FALSE;
			break;
		}
		
		case WM_RBUTTONUP : {
			isDrawing = FALSE;
			isRdown = FALSE;
			break;
		}
		
		//==================���� ���콺 Ŭ��
		 
		case WM_RBUTTONDOWN : {
			//0. isDrawing == true;
			isDrawing = TRUE;
			isRdown = TRUE;
			//color = RGB(0,0,255); //�Ķ���
			 
			//1. hdc ���
			hdc = GetDC(hwnd);
	
			if(isLdown){
				//2. �� ���� : �β��� �ɷ�
				newPen = CreatePen(PS_SOLID, 50, blueColor); //�Ķ��� 
			
			}else{
				newPen = CreatePen(PS_SOLID, 5, blueColor);
			}
			
			oldPen = (HPEN)SelectObject(hdc, newPen); //���� �� �����ϱ� 
			//3. ��ǥ���ϱ� 
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			
			MoveToEx(hdc, x, y, NULL);
			
			//4. �����
			LineTo(hdc, x, y);
			
			//DC�� �����Ѵ�.
			ReleaseDC(hwnd, hdc); 
			break;
		} 
		
		
		//====================2. ���� �Ǵ� ������ �Ѵ� Ŭ����
		case WM_LBUTTONDOWN : {
			//0. isDrawing == true;
			isDrawing = TRUE;
			isLdown = TRUE;
			//color = RGB(0,0,0); //�Ķ���
			
			//1. hdc ���
			hdc = GetDC(hwnd);
	
			if(isRdown){
				//2. �� ���� : �β��� �ɷ�
				newPen = CreatePen(PS_SOLID, 50, 0); //������
			
			}else{
				newPen = CreatePen(PS_SOLID, 5, 0);
			}
			
			oldPen = (HPEN)SelectObject(hdc, newPen); //���� �� �����ϱ� 
			//3. ��ǥ���ϱ� 
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			
			MoveToEx(hdc, x, y, NULL);
			
			//4. �����
			LineTo(hdc, x, y);
			
			//DC�� �����Ѵ�.
			ReleaseDC(hwnd, hdc); 
			break;
		} 
		
		//=========================================================== 
		//2. ���콺�� �����̴µ�, isDrawing�� ��츸 �׷������.	
		case WM_MOUSEMOVE : {
			
			//1. isDrawing �������� Ȯ��
			if(isDrawing){
				//dc��� 
				hdc = GetDC(hwnd);
				int width;
				
				//�������� ���������� Ȯ��
				if(isLdown){ //�����̸� ���� �� 
					if(isRdown){
						width = 50;
					}else{
						width = 5;
					}
					newPen = CreatePen(PS_SOLID, width, 0); //������
					oldPen = (HPEN)SelectObject(hdc, newPen); //���� �� �����ϱ�
				}else{ //�������̸� �Ķ� �� 
					if(isLdown){
						width = 50;
					}else{
						width = 5;
					}
					newPen = CreatePen(PS_SOLID, width, blueColor); //�Ķ��� 
					oldPen = (HPEN)SelectObject(hdc, newPen); //���� �� �����ϱ�
				} 
				
				//���� �����. ���� �׸���.
				//���� ��ǥ�� cp �Űܳ���
				MoveToEx(hdc, x, y, NULL);
				
				//x,y��ǥ���
				x = LOWORD(lParam);
				y = HIWORD(lParam);
				
				//lineto ���ֱ�
				LineTo(hdc, x, y);
				
				//dc ����
				ReleaseDC(hwnd, hdc);  
			} 
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
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
