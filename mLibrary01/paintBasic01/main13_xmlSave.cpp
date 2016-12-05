#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include "resource3.h"
///////////////////////////////////////////////////////////////
#include "mOriginPoint.h"

#include "mMouse.h"

#include "mCircle.h"
#include "mRectangle.h"
#include "mLine.h"

#include "mShapeContainer.h"

#include "mOptions.h"

#import <msxml6.dll>
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

using namespace std;

//���� ������
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;

HINSTANCE g_Inst;
int g_focusedIdx = -1;
int g_orderFlag = -1;
int g_resizePoint;
//global ��ü��
mMouse g_mouse;
mOriginPoint ORIGIN_POINT;
mShapeContainer g_msc;
mOptions g_options;

class mXmlSaving{
	BSTR tempStr, tempStr2;
	BSTR bstrInst1, bstrInst2;
	//TCHAR test[100] = L"version=\"1.0\" encoding = \"UTF-8\" ";
	BSTR tempStr3;
	IXMLDOMDocumentPtr xmlDocument;
	IXMLDOMProcessingInstructionPtr xmlProcessingInstruction;
	IXMLDOMElementPtr elem_file;
	IXMLDOMElementPtr	elem_zoomLevel, elem_nowScreenXY, elem_shapeList;
	IXMLDOMElementPtr	elem_shape;
	IXMLDOMElementPtr elem_class, elem_p1, elem_p2, elem_txt;
	_variant_t variant;

public:
	//1. constructor
	mXmlSaving() {
		bstrInst1 = ::SysAllocString(TEXT("xml"));
		bstrInst2 = ::SysAllocString(TEXT("version=\"1.0\" encoding = \"UTF-8\" "));
		
	}

	//2. method : makeXml
	BSTR makeXml(mShapeContainer& msc, mOptions& mOption, mOriginPoint& mOgPoint, TCHAR* fileAddr) {
		if (msc.getShapeNum() == 0)
			return NULL;

		if (SUCCEEDED(::CoInitialize(NULL))) {
			{
				//. ���� ����
				if (!SUCCEEDED(xmlDocument.CreateInstance(__uuidof(DOMDocument)))) {
					::wprintf(L"xmlDocument�� �������� �ʾҽ��ϴ�.\n");
					::CoUninitialize();
					return NULL;
				}
				

				//1. xml Instruction�� �����. 
				xmlDocument -> createProcessingInstruction(bstrInst1, bstrInst2, &xmlProcessingInstruction); //��Ҹ� �����
				xmlDocument->appendChild(xmlProcessingInstruction, NULL); //���δ�. 

				//2. file element ����� ���̱�
				tempStr = SysAllocString(TEXT("file"));
				xmlDocument->createElement(tempStr, &elem_file); //bstr1�� elem_file�� �����   
				xmlDocument->appendChild(elem_file, NULL); // xmlElement1�� xmlDocument�� ���δ�.
				SysFreeString(tempStr);

				//3. zoomLevel ���̱� : file�ؿ� �ִ�. 
				tempStr = SysAllocString(TEXT("zoomLevel"));
				xmlDocument->createElement(tempStr, &elem_zoomLevel); //bstr1�� elem_file�� �����   
				_bstr_t zoomLevelStr = mOption.getZoomLevel(); //zoomLevel�� string���� ���� 
				elem_zoomLevel->put_text(zoomLevelStr); //�� ���� ���̿� �ִ´�. 
				elem_file->appendChild(elem_zoomLevel, NULL); // xmlElement1�� xmlDocument�� ���δ�.
				SysFreeString(tempStr);

				//4. nowScreenXy ���̱� 
				tempStr = SysAllocString(TEXT("nowScreenXY"));
				xmlDocument->createElement(tempStr, &elem_nowScreenXY); //element�����
				_bstr_t nowScreenX = mOgPoint.getOriginPoint().x;
				_bstr_t nowScreenY = mOgPoint.getOriginPoint().y;
				
				tempStr3 = SysAllocString(TEXT(""));
				wsprintf(tempStr3, TEXT("%d %d"), mOgPoint.getOriginPoint().x, mOgPoint.getOriginPoint().y);
				elem_nowScreenXY->put_text((BSTR)tempStr3);
				
				elem_file->appendChild(elem_nowScreenXY, NULL); // ���̱�
				SysFreeString(tempStr);
				SysFreeString(tempStr3);

				//SysFreeString(tempStr2);

				//5. shapeList ���̱� //���� shapeList������ shape���� �ݺ������� ����� ���δ�. 
				tempStr = SysAllocString(TEXT("shapeList"));
				xmlDocument->createElement(tempStr, &elem_shapeList); //bstr1�� elem_file�� �����   
				elem_file->appendChild(elem_shapeList, NULL); // xmlElement1�� xmlDocument�� ���δ�.
				SysFreeString(tempStr);

				//6. shape�� ���̱� 
				for (int i = 0; i < msc.getShapeNum(); i++) {
					//1. shape�����
					tempStr = SysAllocString(TEXT("shape"));
					xmlDocument->createElement(tempStr, &elem_shape); //bstr1�� elem_file�� ����� //shapeList�ؿ� ���δ�. 
					elem_shapeList->appendChild(elem_shape, NULL); // xmlElement1�� xmlDocument�� ���δ�.
					SysFreeString(tempStr);

					//6.1 class ����� 
					tempStr = SysAllocString(TEXT("class"));
					xmlDocument->createElement(tempStr, &elem_class);
					
					if (msc.getNameAt(i) == Flag::CIRCLE) {
						tempStr2 = SysAllocString(TEXT("Circle"));  
					}
					else if (msc.getNameAt(i) == Flag::RECTANGLE) {
						tempStr2 = SysAllocString(TEXT("Rect")); 
					}
					else {
						tempStr2 = SysAllocString(TEXT("Line"));
					}
					elem_class->put_text(tempStr2); //���� ��� �ְ�
					elem_shape->appendChild(elem_class, NULL); //shape�ؿ� class ���̱� 
					SysFreeString(tempStr);
					SysFreeString(tempStr2);

					//6.2 p1�����
					tempStr = SysAllocString(TEXT("p1"));
					xmlDocument->createElement(tempStr, &elem_p1);

					tempStr3 = SysAllocString(TEXT(""));
					wsprintf(tempStr3, TEXT("%d %d"), msc.getUpLeftAt(i).x, msc.getUpLeftAt(i).y);
					elem_nowScreenXY->put_text((BSTR)tempStr3);

					
					elem_p1->put_text(tempStr3);
				

					elem_shape->appendChild(elem_p1, NULL); //shape�ؿ� class ���̱� 
					SysFreeString(tempStr);
					SysFreeString(tempStr3);

					//6.3 p2�����
					tempStr = SysAllocString(TEXT("p2"));
					xmlDocument->createElement(tempStr, &elem_p2);

					tempStr3 = SysAllocString(TEXT(""));
					wsprintf(tempStr3, TEXT("%d %d"), msc.getDownRightAt(i).x, msc.getDownRightAt(i).y);
					elem_nowScreenXY->put_text((BSTR)tempStr3);


					elem_p2->put_text(tempStr3);

					elem_shape->appendChild(elem_p2, NULL); //shape�ؿ� class ���̱� 
					SysFreeString(tempStr);
					SysFreeString(tempStr3);

					//6.4 text�����
					tempStr = SysAllocString(TEXT("txt"));
					xmlDocument->createElement(tempStr, &elem_txt);
					
					tempStr2 = SysAllocString(msc.getText(i));
					elem_txt->put_text(tempStr2);

					elem_shape->appendChild(elem_txt, NULL); //shape�ؿ� class ���̱� 
					SysFreeString(tempStr);
					SysFreeString(tempStr2);

				
				}

				//�����
				xmlDocument->get_xml(&tempStr);
				variant = fileAddr;
				xmlDocument->save(variant);
			}

			::CoUninitialize();
		
		}

		return tempStr;
	}

};
//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	OPENFILENAME OFN;
	TCHAR fileAddr[300];
	TCHAR lpstrFile[MAX_PATH] = TEXT("");
	HANDLE hFile;
	DWORD dwWritten;
	
	switch (Message) {
	case WM_CREATE: {
		ORIGIN_POINT.setStartPoint(g_mouse, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5 * 2);
		g_options.setFontName(TEXT("�ü�"));
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_NEW_FILE: {
			printf("���� ");
			if (g_msc.getShapeNum() != 0) {
			MessageBox(hwnd, TEXT("���� �ۼ����� ������ �ֽ��ϴ�. ������� �ϼ���. "), TEXT("���� ���� �����ϱ�"), MB_OK);
			SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, NULL);
			g_msc.deleteAll();
			g_focusedIdx = -1;
			g_orderFlag = -1;
			}
			SendMessage(hwnd, WM_LBUTTONDOWN, NULL, NULL);
			SendMessage(hwnd, WM_LBUTTONUP, NULL, NULL);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		}
		case ID_40001: { //��
			g_orderFlag = Flag::CIRCLE;
			break;
		}
		case ID_40002: { //�簢
			g_orderFlag = Flag::RECTANGLE;
			break;
		}
		case ID_40003: { //����
			g_orderFlag = Flag::LINE;
			break;
		}
		case ID_40004: { //��Ʈ
			g_options.setFontName(TEXT("�ü�"));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_40005: { //��Ʈ
			g_options.setFontName(TEXT("����"));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_40006: { //��Ʈ
			g_options.setFontName(TEXT("����"));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_40008: { //��Ʈ
			g_options.setFontName(TEXT("����"));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_40007: { //����
			ORIGIN_POINT.goToZero(g_mouse, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5 * 2);
			g_options.setZoomLevel(1.0);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_FILE_OPEN: { //�ҷ�����
			if (g_msc.getShapeNum() != 0) {
				MessageBox(hwnd, TEXT("���� �ۼ����� ������ �ֽ��ϴ�. ������� �ϼ���. "), TEXT("���� ���� �����ϱ�"), MB_OK);
				SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, NULL);
				g_msc.deleteAll();
				g_focusedIdx = -1;
				g_orderFlag = -1;


			}
			memset(&OFN, 0, sizeof(OPENFILENAME)); //OFN �ʱ�ȭ
			OFN.lStructSize = sizeof(OPENFILENAME); //����ü ũ�� ����
			OFN.hwndOwner = hwnd; //��ȭ������ �θ�������
			OFN.lpstrFilter = TEXT("��� ����(*.*)\0*.*\0");
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = MAX_PATH;

			if (GetOpenFileName(&OFN) != 0) { //����� �����ּҰ� ���õǾ��ٸ�
				wsprintf(fileAddr, TEXT("%ls"), OFN.lpstrFile);
				//MessageBox(hwnd, TEXT("���� ���� ���� "), TEXT("���� ���� ���� "), MB_OK); //fileAddr�� �ּҰ� ����ִ�. 
			}
			else {
				break;
			}
			wprintf(L"%ls \n", fileAddr);
			hFile = CreateFile(fileAddr, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			TCHAR buffer[100000];
			if (hFile != INVALID_HANDLE_VALUE) {
				ReadFile(hFile, buffer, 100000, &dwWritten, NULL);
				CloseHandle(hFile); //������ �� ������ �ݴ´�. 
				wprintf(L"%ls \n", buffer); //buffer�� �ؽ�Ʈ�� �������. 
			}

			mString tempMstr;
			TCHAR* oneLine;
			TCHAR* context;
			TCHAR* enterToken = TEXT("\r\n");
			TCHAR* tabToken = TEXT("\t");

			//1. �� �б� : ù°���� ���ͱ������� �ڸ���, ù��° �����ڵ� ǥ�ø� �����ش�. 
			oneLine = _tcstok_s(buffer, enterToken, &context);
			tempMstr.replaceStr(oneLine); //�ְ� ù° �����ڵ� ��ũ�� �����ش�. 
			tempMstr.eraseFirstChar();
			wprintf(L"%ls \n", tempMstr.getStr()); //tempMstr�� ������ ����� �ִ�. 
			double zoomDouble = _wtof(tempMstr.getStr()); //�ܷ��� �����.//tempmstr.getstr()���� ���� tchar ���ڿ��� ����Ǿ� �����Ƿ� �̱� ����Ʈ�� �ٲ��ش� 
			g_options.setZoomLevel(zoomDouble); //�� ������ �� ���� ���� 	//wprintf(L"%.2f \n", testDouble);

			int shapeType;
			int ulX;
			int ulY;
			int drX;
			int drY;
			TCHAR* content;



			while (1) {
				oneLine = _tcstok_s(context, enterToken, &context);// �ܷ��� ���� ���°� context�ϱ� ������ context�� ��ũ����¡ �Ѵ�. 
																   //wprintf(L"%ls \n", oneLine); //tempMstr�� ������ ����� �ִ�. 

				if (oneLine == NULL)
					break;

				TCHAR* tabContext;
				//1. shapeTYPE ���� 		
				shapeType = _wtoi(_tcstok_s(oneLine, tabToken, &tabContext));
				//2. �»��, ���ϴ�
				ulX = _wtoi(_tcstok_s(tabContext, tabToken, &tabContext));
				ulY = _wtoi(_tcstok_s(tabContext, tabToken, &tabContext));
				drX = _wtoi(_tcstok_s(tabContext, tabToken, &tabContext));
				drY = _wtoi(_tcstok_s(tabContext, tabToken, &tabContext));
				content = _tcstok_s(tabContext, tabToken, &tabContext);


				if (shapeType == Flag::CIRCLE) {
					printf("��, %d %d %d %d \n", ulX, ulY, drX, drY);
					g_msc.add(new mCircle(ulX, ulY, drX, drY));
					wprintf(L"content����� %ls \n", content); //tempMstr�� ������ ����� �ִ�. 
					if (content != NULL) {
						g_msc.replaceTextAt(g_msc.getShapeNum() - 1, content);
					}


				}
				else if (shapeType == Flag::RECTANGLE) {
					printf("�簢��, %d %d %d %d \n", ulX, ulY, drX, drY);
					g_msc.add(new mRectangle(ulX, ulY, drX, drY));
					wprintf(L"content����� %ls \n", content); //tempMstr�� ������ ����� �ִ�. 
					if (content != NULL) {
						g_msc.replaceTextAt(g_msc.getShapeNum() - 1, content);
					}
				}
				else if (shapeType == Flag::LINE)
				{
					printf("��, %d %d %d %d \n", ulX, ulY, drX, drY);
					g_msc.add(new mLine(ulX, ulY, drX, drY));

				}

			}

			SendMessage(hwnd, WM_LBUTTONDOWN, NULL, NULL);
			SendMessage(hwnd, WM_LBUTTONUP, NULL, NULL);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_FILE_SAVE: { 
			//if(xml)
			mString saveContents;		
			
			//txt ��Ÿ�� ���� 
			memset(&OFN, 0, sizeof(OPENFILENAME)); //OFN �ʱ�ȭ
			OFN.lStructSize = sizeof(OPENFILENAME); //����ü ũ�� ����
			OFN.hwndOwner = hwnd; //��ȭ������ �θ�������
			OFN.lpstrFilter = TEXT("��� ����(*.*)\0*.*\0");
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = MAX_PATH;

			if (GetSaveFileName(&OFN) != 0) { //����� �����ּҰ� ���õǾ��ٸ�
				wsprintf(fileAddr, TEXT("%ls"), OFN.lpstrFile);
				MessageBox(hwnd, TEXT("����Ǿ����ϴ�."), TEXT("����Ǿ����ϴ�."), MB_OK); //fileAddr�� �ּҰ� ����ִ�. 
			}
			else {
				break;
			}

			//1. fileAddr�� �м��Ѵ�. 
			mString addrMstr;
			addrMstr.replaceStr(cloneStr(fileAddr));
			TCHAR* fileType = addrMstr.subFromToEnd(addrMstr.getLength() - 4);
			wprintf(L"%ls \n", fileType);
			wprintf(L"%d", _tcscmp(TEXT(".xml"), fileType));

			if (_tcscmp(TEXT(".xml"), fileType) == 0) { //xml�϶� 
				if (g_msc.getShapeNum() != 0) {
					//xml Style
					saveContents.add(mXmlSaving().makeXml(g_msc, g_options, ORIGIN_POINT, fileAddr));
					//unsigned short unicodeMark = 0xFEFF;

					//hFile = CreateFile(fileAddr, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					//WriteFile(hFile, &unicodeMark, sizeof(unicodeMark), &dwWritten, NULL); //�̰� ǥ���� �ָ� �ؽ�Ʈ ������ �����ڵ�� �ν��� �Ǿ� �ѱ� �ν��� �ȴ�. 
					//WriteFile(hFile, saveContents.getStr(), (saveContents.getLength() + 1) * sizeof(TCHAR), &dwWritten, NULL);
					
					//CloseHandle(hFile);
				}				
			}
			else { // xml�� �ƴ� �� ���϶� 
				if (g_msc.getShapeNum() != 0) {
					//txt�� �� �� mString�� ������. 
					mString saveContents;
					TCHAR tempOneLine[100]; //�� ���������� saveContents�� add ���Ѽ� txt�� ����. 

											//�� ���� �Է�
					swprintf_s(tempOneLine, TEXT("%.2lf\r\n"), g_options.getZoomLevel());
					saveContents.add(tempOneLine); //���پ� �߰�

					for (int i = 0; i < g_msc.getShapeNum(); i++) {
						//printf("���� �»��: %d \n", g_msc.getUpLeftAt(i).x);
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getNameAt(i));
						saveContents.add(tempOneLine); //���پ� �߰�
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getUpLeftAt(i).x);
						saveContents.add(tempOneLine); //���پ� �߰�
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getUpLeftAt(i).y);
						saveContents.add(tempOneLine); //���پ� �߰�
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getDownRightAt(i).x);
						saveContents.add(tempOneLine); //���پ� �߰�
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getDownRightAt(i).y);
						saveContents.add(tempOneLine); //���پ� �߰�
						saveContents.add(g_msc.getText(i)); //�ؽ�Ʈ�� �߰�
						saveContents.add(TEXT("\r\n"));
					}
					wprintf(L"���� mstr: %ls \n", saveContents.getStr());
					//txt�� ���� ���빰�� �������� ���常����. 
					unsigned short unicodeMark = 0xFEFF;

					hFile = CreateFile(fileAddr, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					WriteFile(hFile, &unicodeMark, sizeof(unicodeMark), &dwWritten, NULL); //�̰� ǥ���� �ָ� �ؽ�Ʈ ������ �����ڵ�� �ν��� �Ǿ� �ѱ� �ν��� �ȴ�. 
					WriteFile(hFile, saveContents.getStr(), (saveContents.getLength() + 1) * sizeof(TCHAR), &dwWritten, NULL);
					CloseHandle(hFile);

				}
			}
			break;
		}
		}


		break;
	}
					 //Ű���� �޽���
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		if (!g_msc.isEmpty())
			g_msc.procAt(hwnd, Message, wParam, lParam, g_focusedIdx);


		break;
	}



	case WM_MOUSEWHEEL: {
		if ((short)HIWORD(wParam) > 0) {
			g_options.zoomIn();
		}
		else if ((short)HIWORD(wParam) < 0) {
			g_options.zoomOut();
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}



	case WM_MOUSEMOVE: {
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel()); //����-> ��//printf("���콺 ���� relative x,y = %d, %d \n", g_mouse.getRelativeNewPos().x, g_mouse.getRelativeNewPos().y);

		if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx) && g_focusedIdx != -1) {
			SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZEALL));
		}
		else {
			SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));
		}

		if (g_mouse.getGrapped()) {
			//. ȭ�� Ǯ�� ����
			if (g_mouse.getPulled()) {
				ORIGIN_POINT.move(g_mouse.getZoomXdist(g_options.getZoomLevel()), g_mouse.getZoomYdist(g_options.getZoomLevel())); //�� -> ����  
				g_mouse.pullingZoomAction(g_options.getZoomLevel());
				g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel()); //���� -> ��

			}
			if (g_orderFlag == Flag::RESIZE) { //�� ��ǥ���� ���� 
				g_msc.moveResizeAction(g_focusedIdx, g_resizePoint, g_mouse);
			}

			//. �����̵� ����
			if (g_orderFlag == Flag::MOVE) {
				g_msc.moveAt(g_focusedIdx, g_mouse.getXdist(), g_mouse.getYdist());
				g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel()); //���� -> �� 
			}
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else {
			if (g_focusedIdx != -1) { //�������� ���� �� ���� 
				g_resizePoint = g_msc.isClosedAt(g_focusedIdx, g_mouse.getRelativeNewPos());

				if (g_resizePoint == 1 || g_resizePoint == 3) { //�»�ܸ� �����ش�.  
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZENWSE));
				}
				else if (g_resizePoint == 2 || g_resizePoint == 4) {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZENESW));
				}
				else if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx)) {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZEALL));
				}
				else {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));
				}


			}
		}

		break;
	}
	case WM_RBUTTONDOWN: {
		g_mouse.setPulled(true);
		g_mouse.setGrap(true);
		g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel());
		break;
	}
	case WM_RBUTTONUP: {
		g_mouse.setPulled(false);
		g_mouse.setGrap(false);
		break;
	}


	case WM_LBUTTONDOWN: {
		//1. 1���� ���� 100,100�̸� 2���� ���� 50,50�� ������ �Ǵϱ� zoomLevel�� ������ �־���� �ȴ�. 
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel());
		g_mouse.setGrap(true);

		printf("relative new x,y : %d, %d, relative old = %d, %d \n", g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY(), g_mouse.getRelativeOldX(), g_mouse.getRelativeOldY()); //�� ��. 
		if (g_focusedIdx != -1) { //������ ���õǾ� �ִٸ�..
			if (g_resizePoint != -1) { //�������� �����Ѵٴ� �ǹ�
				g_mouse.setTemPos1(g_msc.getUpLeftAt(g_focusedIdx));
				g_mouse.setTemPos2(g_msc.getDownRightAt(g_focusedIdx));
				g_orderFlag = Flag::RESIZE;
			}
			else {
				if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx)) {
					g_orderFlag = Flag::MOVE; //�����̶�� ��� ���� 
				}
			}


		}

		//. ���� ���� ����
		if (g_orderFlag == Flag::NOTHING) {
			g_focusedIdx = g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx); //printf("������ focused: %d \n", g_focusedIdx); //��� ������ ������� �Ǻ����ش� //���� �� ��.
																					 //printf("���� ���콺 �ȿ� �ִ� ����: %d \n", g_focusedIdx);
		}

		g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel());
		break;
	}

	case WM_LBUTTONUP: {
		g_mouse.setGrap(false);
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel());
		g_focusedIdx = g_msc.buMakeShapeAction(g_orderFlag, g_focusedIdx, g_mouse);

		g_orderFlag = Flag::NOTHING; //�����ϱ� ���ؼ� 
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		if (g_mouse.getGrapped()) {
			g_msc.paintShowZoomProgressAction(hdc, g_orderFlag, g_mouse, ORIGIN_POINT.getOriginPoint(), g_options.getZoomLevel());
		}
		g_options.setting(hdc);
		g_msc.showAllExcept_Zoom(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint(), g_options.getZoomLevel(), g_options.getFontSize());
		g_msc.showAt_zoom(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint(), g_options.getZoomLevel(), g_options.getFontSize());
		g_msc.showDotAt_relative(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint());

		ORIGIN_POINT.show(hdc);
		g_options.settingOlds(hdc);
		EndPaint(hwnd, &ps);
		break;
	}


				   //======================�������� �ܼ� �����ֱ�======================== 
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
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
		WINDOW_WIDTH, /* width */
		WINDOW_HEIGHT, /* height */
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