//텍스트 에디터 클래스화 시키기
//#include "mTextEditor.h"
#include "newTextEditor03.h"
#include "resource3.h"

#include "mRectangle.h"
#include "mCircle.h"
#include "mLine.h"
#include "mShapeContainer.h"
#include "mMouse.h"
#include "mOriginPoint.h"
#include "mZoom.h"

//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif
using namespace std;



//global_variables
mShapeContainer msc;
mMouse mouse;
int orderFlag = -1;
int focusedIdx = -1;
int g_resizePoint;

int g_oldX, g_oldY;
int g_newX, g_newY;

POINT g_ul, g_rd;

bool g_isPulling = false;

mOriginPoint originPoint;
mZoom zoom;



//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	// 전역 변수들 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;


	switch (Message) {
		//커맨드 메시지
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
			focusedIdx = -1;
		case ID_40001: { //원
			orderFlag = Flag::CIRCLE;
			break;
		}
		case ID_40002: { //사각
			orderFlag = Flag::RECTANGLE;
			break;
		}
		case ID_40003: { //선분
			orderFlag = Flag::LINE;
			break;
		}

		}


		break;
	}

	//마우스 메시지
	case WM_RBUTTONDOWN: {
		g_isPulling = true; //풀링중 표시
		g_oldX = LOWORD(lParam);
		g_oldY = HIWORD(lParam); //마우스 찍은 위치 기억 //여기가 원점이 된다. 
		break;
	}

	case WM_RBUTTONUP: {
		g_isPulling = false;
		g_newX = LOWORD(lParam);
		g_newY = HIWORD(lParam); //마우스 찍은 위치 기억 //여기가 원점이 된다. 

		int xdist = g_newX - g_oldX; //어느 정도로 움직인지 파악
		int ydist = g_newY - g_oldY;

		//1. msc의 모든 도형을 움직인다. 
		for (int i = 0; i < msc.getShapeNum(); i++) {
			msc.moveAt(i, xdist, ydist);
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_LBUTTONDOWN: {
		//1. 마우스 잡기
		mouse.setGrap(true);
		//2. 현재 좌표 저장
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);
		POINT tempPoint = { tempX, tempY };
		printf("로직 돌기전 focused: %d, orderflag = %d \n", focusedIdx, orderFlag);
		//도형 선택
		//3. 그려주는 도형이 NOTHING, 즉 그리기 위한 목적이 아니라면 선택하기
		if (focusedIdx != -1) { //어떤 도형이 선택되어 있으면 움직이거나 사이즈 변경하거나 둘 중 하나다. 
			if (g_resizePoint != -1) { //===== 도형이 선택도 되어있고, 리사이즈Point가 -1이아니다. => 꼭짓점 근처에 마우스가 있다. 
									   //resize 로직의 시작점 //여기서 부터 리사이즈를 시작한다. 

				g_ul = msc.getUpLeftPosAt(focusedIdx); //현재 포커스된 도형 대표 좌표 저장해놓기 
				g_rd = msc.getDownRightPosAt(focusedIdx);
				orderFlag = Flag::RESIZE; //resize로 바꿔준다. //이제 wm_mouseUp으로 넘어간다. //wm_mouseMove는 중간 출력 단계에 불과하다. 속지마라.
			}
			else { //===== 도형이 선택은 되어있는데, 리사이즈 포인트는 -1, 즉 꼭짓점 근처가 아니다. => 이동하기 
				if (focusedIdx == msc.whoIsIn(tempPoint, focusedIdx)) { // 그렇더라도 마우스가 현재 선택된  도형 위에 있어야지 이동이 된다. 
					g_oldX = tempX;
					g_oldY = tempY;

					g_ul = msc.getUpLeftPosAt(focusedIdx); //현재 포커스된 도형 좌표 2개 저장해놓기 
					g_rd = msc.getDownRightPosAt(focusedIdx);

					orderFlag = Flag::MOVE; //움직이는 상태로 바꿔준다. 
				}
			}
		}
		if (orderFlag == Flag::NOTHING) { //클릭했는데, 아무것도 안하는 거면 도형을 선택하는 거다. 
			focusedIdx = msc.whoIsIn(tempPoint, focusedIdx);  //1. 컨테이너를 돌면서 선택된 얘를 찾아준다. //아무것도 없으면 -1이다. 
			printf("돌고난후 focused: %d \n", focusedIdx); //어느 도형을 찍었는지 판별해준다 
		}
		mouse.setOldX(tempX);
		mouse.setOldY(tempY);
		break;
	}

	case WM_LBUTTONUP: {
		mouse.setGrap(false);

		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);

		mouse.setNewX(tempX);
		mouse.setNewY(tempY);

		switch (orderFlag) {
		case Flag::CIRCLE: {
			msc.add(new mCircle(mouse.getUpLeft().x, mouse.getUpLeft().y, mouse.getRightDown().x, mouse.getRightDown().y));
			focusedIdx = msc.getShapeNum() - 1;
			break;
		}
		case Flag::RECTANGLE: {
			msc.add(new mRectangle(mouse.getUpLeft().x, mouse.getUpLeft().y, mouse.getRightDown().x, mouse.getRightDown().y));
			focusedIdx = msc.getShapeNum() - 1;
			break;
		}
		case Flag::LINE: {
			msc.add(new mLine(mouse.getOldPos().x, mouse.getOldPos().y, mouse.getNewPos().x, mouse.getNewPos().y));
			focusedIdx = msc.getShapeNum() - 1;
			break;
		}
		case Flag::MOVE: { //움직여 준다. 
			int tempXdist = mouse.getNewPos().x - mouse.getOldPos().x;
			int tempYdist = mouse.getNewPos().y - mouse.getOldPos().y;
			msc.moveAt(focusedIdx, tempXdist, tempYdist);
			break;
		}

		}
		printf("button up 에서 nothing으로 바꿈!!");
		orderFlag = Flag::NOTHING;

		InvalidateRect(hwnd, NULL, TRUE);


		break;
	}

	case WM_MOUSEMOVE: {
		//현재 좌표 받기
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);

		if (g_isPulling) { //누른 상태라면 

			g_newX = tempX;
			g_newY = tempY; //마우스 찍은 위치 기억 //여기가 원점이 된다. 

			int xdist = g_newX - g_oldX; //어느 정도로 움직인지 파악
			int ydist = g_newY - g_oldY;

			//1. msc의 모든 도형을 움직인다. 
			for (int i = 0; i < msc.getShapeNum(); i++) {
				msc.moveAt(i, xdist, ydist);
			}
			originPoint.move(xdist, ydist);

			g_oldX = g_newX;
			g_oldY = g_newY;
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		}

		//뭔가 하는중, 즉 클릭된 상태라면 => 이동, 사이즈, 그리기 셋 중 하나.
		if (mouse.getGrapped()) {
			printf("클릭상태로 마우스 움직임 orderflag = %d focused = %d \n", orderFlag, focusedIdx);

			if (orderFlag == Flag::RESIZE) { //리사이즈 중이라면 
											 //현재 마우스 포지션이 원래 위치보다 초과하면 안되니까 필터링 해준다. //아직 필터링은 안함. 
				POINT tempUl, tempRd;
				printf("사이즈 조절 중");
				if (g_resizePoint == 1) { //좌상단만 변해준다.  
					tempUl = { tempX, tempY };
					tempRd = g_rd;
				}
				else if (g_resizePoint == 2) {
					tempUl = { g_ul.x, tempY };
					tempRd = { tempX, g_rd.y };
				}
				else if (g_resizePoint == 3) { //우하단만 변경

					tempUl = g_ul;
					tempRd = { tempX, tempY };
				}
				else {
					tempUl = { tempX, g_ul.y };
					tempRd = { g_rd.x, tempY };
				}
				msc.resizeAt(focusedIdx, tempUl, tempRd); //사이즈 변경 
			}

			mouse.setNewX(tempX);
			mouse.setNewY(tempY);

			g_newX = tempX;
			g_newY = tempY;

			InvalidateRect(hwnd, NULL, TRUE);

		} //마우스를 누른 상태가 아닌데,
		else {
			if (focusedIdx != -1) { //선택된 도형이 있다. => 꼭짓점에 다다랐는지 아닌지 항상 체크해준다. : resizePoint를 항상 갱신: -1이면 꼭짓점 아니라는 뜻
				POINT tempPoint = { tempX, tempY };
				g_resizePoint = msc.isClosedAt(focusedIdx, tempPoint);
				printf("is closed = %d \n", g_resizePoint);
			}
			else { //선택된 도형이 없다. => 아무일도 일어나지 않는다. 
			}
		}

		break;
	}

	case WM_MOUSEWHEEL: {
		
		if ((short)HIWORD(wParam) > 0) { //증가
			zoom += zoom.getZoomCounter();
		}
		else if ((short)HIWORD(wParam) < 0) {
			zoom -= zoom.getZoomCounter();
			
		}

		printf("zoom: %lf, pixelCnt: %d \n", zoom.getZoomLevel(), zoom.getOnePixelCnt());
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	//키보드 메시지
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		if (!msc.isEmpty())
			msc.procAt(hwnd, Message, wParam, lParam, focusedIdx);


		break;
	}

	 //페인트
	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		switch (orderFlag) {
		case Flag::CIRCLE: {
			mCircle().showProgress(hdc, mouse.getUpLeft().x, mouse.getUpLeft().y, mouse.getRightDown().x, mouse.getRightDown().y);
			break;
		}
		case Flag::RECTANGLE: {
			mRectangle().showProgress(hdc, mouse.getUpLeft().x, mouse.getUpLeft().y, mouse.getRightDown().x, mouse.getRightDown().y);
			break;
		}
		case Flag::LINE: {
			mLine().showProgress(hdc, mouse.getOldPos().x, mouse.getOldPos().y, mouse.getNewPos().x, mouse.getNewPos().y);
			break;
		}

		case Flag::MOVE: {
			int tempXdist, tempYdist, tempUlX, tempUlY, tempDrX, tempDrY;

			tempXdist = g_newX - g_oldX;
			tempYdist = g_newY - g_oldY;

			tempUlX = g_ul.x + tempXdist;
			tempUlY = g_ul.y + tempYdist;
			tempDrX = g_rd.x + tempXdist;
			tempDrY = g_rd.y + tempYdist;

			msc.showProgressAt(hdc, focusedIdx, tempUlX, tempUlY, tempDrX, tempDrY);


			break;
		}

		}


		msc.showAllExcept(hdc, focusedIdx); //showAll하면 showAt이 두번호출되어서 겹쳐보임. 
		msc.showAt(hdc, focusedIdx);
		msc.showDotAt(hdc, focusedIdx);
		originPoint.show(hdc);
		EndPaint(hwnd, &ps);
		break;
	}




				   //======================마지막엔 콘솔 없애주기======================== 
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
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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