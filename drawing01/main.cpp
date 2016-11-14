#include <windows.h>

/* This is where all the input to the window goes to */
//1. lbutton : 일반 점
//2. rbutton : 파란 점
//3. l+r button : 두꺼운 점
//4. mouseMove : 선그리기
 
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
	//검정색은 0 쓰면 되니까. 굳이 정의 안함 
	static int x,y;
	
	switch(Message) {
		//1. 일단 마우스 버튼이 올라가면 is clicked false해주기
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
		
		//==================오른 마우스 클릭
		 
		case WM_RBUTTONDOWN : {
			//0. isDrawing == true;
			isDrawing = TRUE;
			isRdown = TRUE;
			//color = RGB(0,0,255); //파란색
			 
			//1. hdc 얻기
			hdc = GetDC(hwnd);
	
			if(isLdown){
				//2. 펜 장착 : 두꺼운 걸로
				newPen = CreatePen(PS_SOLID, 50, blueColor); //파란색 
			
			}else{
				newPen = CreatePen(PS_SOLID, 5, blueColor);
			}
			
			oldPen = (HPEN)SelectObject(hdc, newPen); //옛날 펜 보관하기 
			//3. 좌표구하기 
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			
			MoveToEx(hdc, x, y, NULL);
			
			//4. 점찍기
			LineTo(hdc, x, y);
			
			//DC를 해제한다.
			ReleaseDC(hwnd, hdc); 
			break;
		} 
		
		
		//====================2. 왼쪽 또는 오른쪽 둘다 클릭시
		case WM_LBUTTONDOWN : {
			//0. isDrawing == true;
			isDrawing = TRUE;
			isLdown = TRUE;
			//color = RGB(0,0,0); //파란색
			
			//1. hdc 얻기
			hdc = GetDC(hwnd);
	
			if(isRdown){
				//2. 펜 장착 : 두꺼운 걸로
				newPen = CreatePen(PS_SOLID, 50, 0); //검정색
			
			}else{
				newPen = CreatePen(PS_SOLID, 5, 0);
			}
			
			oldPen = (HPEN)SelectObject(hdc, newPen); //옛날 펜 보관하기 
			//3. 좌표구하기 
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			
			MoveToEx(hdc, x, y, NULL);
			
			//4. 점찍기
			LineTo(hdc, x, y);
			
			//DC를 해제한다.
			ReleaseDC(hwnd, hdc); 
			break;
		} 
		
		//=========================================================== 
		//2. 마우스가 움직이는데, isDrawing일 경우만 그려줘야지.	
		case WM_MOUSEMOVE : {
			
			//1. isDrawing 인지부터 확인
			if(isDrawing){
				//dc얻고 
				hdc = GetDC(hwnd);
				int width;
				
				//왼쪽인지 오른쪽인지 확인
				if(isLdown){ //왼쪽이면 검은 펜 
					if(isRdown){
						width = 50;
					}else{
						width = 5;
					}
					newPen = CreatePen(PS_SOLID, width, 0); //검정색
					oldPen = (HPEN)SelectObject(hdc, newPen); //옛날 펜 보관하기
				}else{ //오른쪽이면 파란 펜 
					if(isLdown){
						width = 50;
					}else{
						width = 5;
					}
					newPen = CreatePen(PS_SOLID, width, blueColor); //파란색 
					oldPen = (HPEN)SelectObject(hdc, newPen); //옛날 펜 보관하기
				} 
				
				//펜을 얻었다. 이제 그린다.
				//옛날 좌표로 cp 옮겨놓기
				MoveToEx(hdc, x, y, NULL);
				
				//x,y좌표얻기
				x = LOWORD(lParam);
				y = HIWORD(lParam);
				
				//lineto 해주기
				LineTo(hdc, x, y);
				
				//dc 해제
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
