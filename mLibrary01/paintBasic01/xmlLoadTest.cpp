#include <cstdio>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#import <msxml6.dll>
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
using namespace std;

//전역 변수들
HINSTANCE g_Inst;


//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
	case WM_CREATE: {
		
		if (SUCCEEDED(::CoInitialize(NULL))) {
			{
				_variant_t variant;
				BSTR tempBstr1;
				IXMLDOMDocumentPtr xmlDocument;
				IXMLDOMNodeListPtr xmlNodeList1 = NULL;
				IXMLDOMNodePtr pNode = NULL;
				IXMLDOMNodePtr parentNode = NULL;
				IXMLDOMNodePtr childNode = NULL;
				long listLength;

				VARIANT_BOOL variantBool;
				
				if (!SUCCEEDED(xmlDocument.CreateInstance(__uuidof(DOMDocument)))) {
					::wprintf(L"xmlDocument가 생성되지 않았습니다.\n");
					::CoUninitialize();
					return NULL;
				}
			
				variant = TEXT("test.xml");
				xmlDocument->load(variant, &variantBool); //xmlDocument를 load한다. 
				
				//1. zoomLevel을 얻는다. //1 얻어옴 
				xmlDocument->selectSingleNode(_bstr_t("/file/zoomLevel"), &pNode);
				pNode->get_text(&tempBstr1);
				wprintf(L"%ls \n",tempBstr1);

				//2. now screen xy
				xmlDocument->selectSingleNode(_bstr_t("/file/nowScreenXY"), &pNode);
				pNode->get_text(&tempBstr1);
				wprintf(L"%ls \n", tempBstr1);

				//3. shapeList 읽어오기 
				xmlDocument->selectNodes(_bstr_t("/file/shapeList/shape"), &xmlNodeList1);
				xmlNodeList1->get_length(&listLength);
				printf("%d 개 \n", listLength); //3개 라고 나온다. 

				//4. 
				for (int i = 0; i < (int)listLength; i++) {
					xmlNodeList1->get_item(i, &parentNode);
					parentNode->selectSingleNode(_bstr_t("p1"), &childNode);
					childNode->get_text(&tempBstr1);
					wprintf(L"%ls \n", tempBstr1);
					
				}
			
				
			}
		

			::CoUninitialize();
		}

		break;
	}



		//======================마지막엔 콘솔 없애주기======================== 
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



//-------------------------------------------------WIN MAIN-------------------------------------------------------------------
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	_wsetlocale(LC_ALL, _T("korean"));
	g_Inst = hInstance;
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