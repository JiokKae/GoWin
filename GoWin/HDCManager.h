#pragma once
#include <Windows.h>
#include <array>

class HDCManager {
	HDC *hdcs;

public:
	HDCManager(int num);
	~HDCManager();
	HDC operator[](int index) {
		return hdcs[index];
	}

	void AddBitmap(HDC hdc, HWND hWnd, int index, HBITMAP bitmap)
	{
		hdc = GetDC(hWnd);

		//HDC hdc_BlackStone, hdc_WhiteStone, hdc_BackGround, hdc_Board;
		
		hdcs[index] = CreateCompatibleDC(hdc);

		ReleaseDC(hWnd, hdc);

		SelectObject(hdcs[index], bitmap);

		DeleteObject(bitmap);

	}
};