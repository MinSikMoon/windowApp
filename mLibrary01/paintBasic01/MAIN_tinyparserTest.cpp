
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


class mXml {
public:
	TCHAR* namingStr;
	IXMLDOMDocumentPtr xmlDocument; //xml 문서
	IXMLDOMProcessingInstructionPtr xmlProcessingInstruction; //xml 문서 종류 지정 엘리먼트
	IXMLDOMElementPtr elem_file, elem_zoomLevel, elem_nowScreenXY; //element들 종류만큼 있어야 한다. 
};



//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {

	case WM_CREATE: {
		//::setlocale(LC_ALL, "");
		if (SUCCEEDED(::CoInitialize(NULL))) {
			{
				BSTR bstr1, bstr2, bstr3;
				_variant_t variant;
				IXMLDOMDocumentPtr xmlDocument;
				IXMLDOMProcessingInstructionPtr xmlProcessingInstruction;
				IXMLDOMElementPtr elem_file, elem_zoomLevel, elem_nowScreenXY;
				//if (SUCCEEDED(xmlDocument.CreateInstance(CLSID_XMLDocument))) return -1;

				if (!SUCCEEDED(xmlDocument.CreateInstance(__uuidof(DOMDocument)))) {
					::wprintf(L"xmlDocument가 생성되지 않았습니다.\n");
					::CoUninitialize();
					return -1;
				}

				//0. xml instruction을 붙이는 작업
				bstr1 = ::SysAllocString(TEXT("xml"));
				bstr2 = ::SysAllocString(TEXT("version=\"1.0\""));
				xmlDocument->createProcessingInstruction(bstr1, bstr2, &xmlProcessingInstruction);
				::SysFreeString(bstr1);
				::SysFreeString(bstr2);
				xmlDocument->appendChild(xmlProcessingInstruction, NULL);

			
				//1. <file></file> 붙이기
				bstr1 = SysAllocString(TEXT("file"));
				xmlDocument->createElement(bstr1, &elem_file); //bstr1을 xmlelement로 만들고
				//SysFreeString(bstr1);
				xmlDocument->appendChild(elem_file, NULL); // xmlElement1을 xmlDocument에 붙인다. 

				//2. zoomLevel을 file밑에 붙인다.
				bstr1 = TEXT("zoomLevel");
				xmlDocument->createElement(bstr1, &elem_zoomLevel);
				_bstr_t zoomLevelStr = 1.2;
				elem_zoomLevel->put_text(zoomLevelStr);
				elem_file->appendChild(elem_zoomLevel, NULL); //xmlElement2를 xmlElement1에 붙인다. 파일밑에 줌레벨, 
				//3. nowScreenXY를 붙인다. 
				bstr1 = TEXT("nowScreenXY");
				xmlDocument->createElement(bstr1, &elem_nowScreenXY); 
				elem_file->appendChild(elem_nowScreenXY, NULL); //xmlElement2를 xmlElement1에 붙인다. 파일밑에 나우스크린xy
				//3. zoomLevel에 double 값을 넣어보자. 
				
				
				
				//. 디버깅 
				xmlDocument->get_xml(&bstr3);
				wprintf(L"%ls \n", bstr3);


			
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