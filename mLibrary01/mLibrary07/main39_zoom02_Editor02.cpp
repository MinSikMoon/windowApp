//�ؽ�Ʈ ������ Ŭ����ȭ ��Ű��
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
	// ���� ������ 
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;


	switch (Message) {
		//Ŀ�ǵ� �޽���
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
			focusedIdx = -1;
		case ID_40001: { //��
			orderFlag = Flag::CIRCLE;
			break;
		}
		case ID_40002: { //�簢
			orderFlag = Flag::RECTANGLE;
			break;
		}
		case ID_40003: { //����
			orderFlag = Flag::LINE;
			break;
		}

		}


		break;
	}

	//���콺 �޽���
	case WM_RBUTTONDOWN: {
		g_isPulling = true; //Ǯ���� ǥ��
		g_oldX = LOWORD(lParam);
		g_oldY = HIWORD(lParam); //���콺 ���� ��ġ ��� //���Ⱑ ������ �ȴ�. 
		break;
	}

	case WM_RBUTTONUP: {
		g_isPulling = false;
		g_newX = LOWORD(lParam);
		g_newY = HIWORD(lParam); //���콺 ���� ��ġ ��� //���Ⱑ ������ �ȴ�. 

		int xdist = g_newX - g_oldX; //��� ������ �������� �ľ�
		int ydist = g_newY - g_oldY;

		//1. msc�� ��� ������ �����δ�. 
		for (int i = 0; i < msc.getShapeNum(); i++) {
			msc.moveAt(i, xdist, ydist);
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_LBUTTONDOWN: {
		//1. ���콺 ���
		mouse.setGrap(true);
		//2. ���� ��ǥ ����
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);
		POINT tempPoint = { tempX, tempY };
		printf("���� ������ focused: %d, orderflag = %d \n", focusedIdx, orderFlag);
		//���� ����
		//3. �׷��ִ� ������ NOTHING, �� �׸��� ���� ������ �ƴ϶�� �����ϱ�
		if (focusedIdx != -1) { //� ������ ���õǾ� ������ �����̰ų� ������ �����ϰų� �� �� �ϳ���. 
			if (g_resizePoint != -1) { //===== ������ ���õ� �Ǿ��ְ�, ��������Point�� -1�̾ƴϴ�. => ������ ��ó�� ���콺�� �ִ�. 
									   //resize ������ ������ //���⼭ ���� ������� �����Ѵ�. 

				g_ul = msc.getUpLeftPosAt(focusedIdx); //���� ��Ŀ���� ���� ��ǥ ��ǥ �����س��� 
				g_rd = msc.getDownRightPosAt(focusedIdx);
				orderFlag = Flag::RESIZE; //resize�� �ٲ��ش�. //���� wm_mouseUp���� �Ѿ��. //wm_mouseMove�� �߰� ��� �ܰ迡 �Ұ��ϴ�. ��������.
			}
			else { //===== ������ ������ �Ǿ��ִµ�, �������� ����Ʈ�� -1, �� ������ ��ó�� �ƴϴ�. => �̵��ϱ� 
				if (focusedIdx == msc.whoIsIn(tempPoint, focusedIdx)) { // �׷����� ���콺�� ���� ���õ�  ���� ���� �־���� �̵��� �ȴ�. 
					g_oldX = tempX;
					g_oldY = tempY;

					g_ul = msc.getUpLeftPosAt(focusedIdx); //���� ��Ŀ���� ���� ��ǥ 2�� �����س��� 
					g_rd = msc.getDownRightPosAt(focusedIdx);

					orderFlag = Flag::MOVE; //�����̴� ���·� �ٲ��ش�. 
				}
			}
		}
		if (orderFlag == Flag::NOTHING) { //Ŭ���ߴµ�, �ƹ��͵� ���ϴ� �Ÿ� ������ �����ϴ� �Ŵ�. 
			focusedIdx = msc.whoIsIn(tempPoint, focusedIdx);  //1. �����̳ʸ� ���鼭 ���õ� �긦 ã���ش�. //�ƹ��͵� ������ -1�̴�. 
			printf("������ focused: %d \n", focusedIdx); //��� ������ ������� �Ǻ����ش� 
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
		case Flag::MOVE: { //������ �ش�. 
			int tempXdist = mouse.getNewPos().x - mouse.getOldPos().x;
			int tempYdist = mouse.getNewPos().y - mouse.getOldPos().y;
			msc.moveAt(focusedIdx, tempXdist, tempYdist);
			break;
		}

		}
		printf("button up ���� nothing���� �ٲ�!!");
		orderFlag = Flag::NOTHING;

		InvalidateRect(hwnd, NULL, TRUE);


		break;
	}

	case WM_MOUSEMOVE: {
		//���� ��ǥ �ޱ�
		int tempX = LOWORD(lParam);
		int tempY = HIWORD(lParam);

		if (g_isPulling) { //���� ���¶�� 

			g_newX = tempX;
			g_newY = tempY; //���콺 ���� ��ġ ��� //���Ⱑ ������ �ȴ�. 

			int xdist = g_newX - g_oldX; //��� ������ �������� �ľ�
			int ydist = g_newY - g_oldY;

			//1. msc�� ��� ������ �����δ�. 
			for (int i = 0; i < msc.getShapeNum(); i++) {
				msc.moveAt(i, xdist, ydist);
			}
			originPoint.move(xdist, ydist);

			g_oldX = g_newX;
			g_oldY = g_newY;
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		}

		//���� �ϴ���, �� Ŭ���� ���¶�� => �̵�, ������, �׸��� �� �� �ϳ�.
		if (mouse.getGrapped()) {
			printf("Ŭ�����·� ���콺 ������ orderflag = %d focused = %d \n", orderFlag, focusedIdx);

			if (orderFlag == Flag::RESIZE) { //�������� ���̶�� 
											 //���� ���콺 �������� ���� ��ġ���� �ʰ��ϸ� �ȵǴϱ� ���͸� ���ش�. //���� ���͸��� ����. 
				POINT tempUl, tempRd;
				printf("������ ���� ��");
				if (g_resizePoint == 1) { //�»�ܸ� �����ش�.  
					tempUl = { tempX, tempY };
					tempRd = g_rd;
				}
				else if (g_resizePoint == 2) {
					tempUl = { g_ul.x, tempY };
					tempRd = { tempX, g_rd.y };
				}
				else if (g_resizePoint == 3) { //���ϴܸ� ����

					tempUl = g_ul;
					tempRd = { tempX, tempY };
				}
				else {
					tempUl = { tempX, g_ul.y };
					tempRd = { g_rd.x, tempY };
				}
				msc.resizeAt(focusedIdx, tempUl, tempRd); //������ ���� 
			}

			mouse.setNewX(tempX);
			mouse.setNewY(tempY);

			g_newX = tempX;
			g_newY = tempY;

			InvalidateRect(hwnd, NULL, TRUE);

		} //���콺�� ���� ���°� �ƴѵ�,
		else {
			if (focusedIdx != -1) { //���õ� ������ �ִ�. => �������� �ٴٶ����� �ƴ��� �׻� üũ���ش�. : resizePoint�� �׻� ����: -1�̸� ������ �ƴ϶�� ��
				POINT tempPoint = { tempX, tempY };
				g_resizePoint = msc.isClosedAt(focusedIdx, tempPoint);
				printf("is closed = %d \n", g_resizePoint);
			}
			else { //���õ� ������ ����. => �ƹ��ϵ� �Ͼ�� �ʴ´�. 
			}
		}

		break;
	}

	case WM_MOUSEWHEEL: {
		
		if ((short)HIWORD(wParam) > 0) { //����
			zoom += zoom.getZoomCounter();
		}
		else if ((short)HIWORD(wParam) < 0) {
			zoom -= zoom.getZoomCounter();
			
		}

		printf("zoom: %lf, pixelCnt: %d \n", zoom.getZoomLevel(), zoom.getOnePixelCnt());
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	//Ű���� �޽���
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		if (!msc.isEmpty())
			msc.procAt(hwnd, Message, wParam, lParam, focusedIdx);


		break;
	}

	 //����Ʈ
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


		msc.showAllExcept(hdc, focusedIdx); //showAll�ϸ� showAt�� �ι�ȣ��Ǿ ���ĺ���. 
		msc.showAt(hdc, focusedIdx);
		msc.showDotAt(hdc, focusedIdx);
		originPoint.show(hdc);
		EndPaint(hwnd, &ps);
		break;
	}




				   //======================�������� �ܼ� �����ֱ�======================== 
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