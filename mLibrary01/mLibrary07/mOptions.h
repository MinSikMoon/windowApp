#pragma once
#include <Windows.h>
class mOptions {
private: //줌레벨, 줌관련 xform, 폰트크기, 
		 //0. 초기 윈도우 크기

		 //1. 줌
	double zoomLevel;
	double zoomInc;
	double zoomMin;
	double zoomMax;
	XFORM zoomXform;


	//2. 폰트
	HFONT hFont;
	double fontSize;
	TCHAR fontName[100];

	//3. 선
	HPEN hPen;
	double penWidth;


public:
	//1. constructor
	mOptions() {
		zoomLevel = 1.0;
		zoomInc = 0.1;
		zoomXform.eM11 = zoomLevel;
		zoomXform.eM22 = zoomLevel;
		zoomMin = 1.0;
		zoomMax = 20.0;

		fontSize = 16;
		_tcscpy_s(fontName, 100, TEXT("궁서"));
		hFont = CreateFont(fontSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, fontName);

		penWidth = 1;
		hPen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 0));
	}
	

	//2. zoom 관련
	void setZoomLevel(double newZoomLevel) {
		zoomLevel = newZoomLevel;
	}
	void setZoomInc(double newZoomInc) {
		zoomInc = newZoomInc;
	}
	void setZoomMin(double newZoomMin) {
		zoomMin = newZoomMin;
	}
	void setZoomMax(double newZoomMax) {
		zoomMax = newZoomMax;
	}
	void zoomIn() {
		zoomLevel += zoomInc;

		if (zoomLevel > zoomMax) {
			zoomLevel = zoomMax;
		}
	}
	void zoomOut() {
		zoomLevel -= zoomInc;

		if (zoomLevel < zoomMin) {
			zoomLevel = zoomMin;
		}
	}
	double getZoomLevel() {
		return zoomLevel;
	}
	

	//3. font 관련
	HFONT getFont() {
		return CreateFont(fontSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, fontName);
	}
	void setFontSize(double size) {
		fontSize = size;
	}
	void setFontName(TCHAR* newFontName) {
		_tcscpy_s(fontName, 100, newFontName);
	}

	//4. pen 관련
	HPEN getPen() {
		return CreatePen(PS_SOLID, penWidth, RGB(0, 0, 0));
	}
	void setPenWidth(double newPenWidth) {
		penWidth = newPenWidth;
	}

	//5. paint
	void setting(HDC hdc) {
		hFont = getFont();
		hPen = getPen();
		SelectObject(hdc, hFont);
		SelectObject(hdc, hPen);
		zoomXform.eM11 = zoomLevel;
		zoomXform.eM22 = zoomLevel;
		SetGraphicsMode(hdc, GM_ADVANCED);
		SetWorldTransform(hdc, &zoomXform);
	}

};
