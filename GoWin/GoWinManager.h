#pragma once
#include "framework.h"

enum EBitmapName
{
	BlackStone,
	WhiteStone,
	BackGround,
	Board,
};

class GoWinManager
{
	Go game;
	HDCManager bitmaps;
	HDC hdc, hdcMem;
	HBITMAP hbmMem, hbmMemOld;
	HWND hChatInputBox, hChatBox;
	HWND hWCS, hBCS;
	HFONT hFont;
	POINT         mouse;
	BoardGraphic    boardInfo({ 0, 0 }, 806, 806, SPACE_SIZE, 6);
	MySocket        mysocket;

	bool printSequenceSwitch;
public:

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);

	Go& GetGame();
	bool GetPrintSequenceSwitch();

	void SetPrintSequenceSwitch(bool b);

	void FileOpen();
	void FileSave();

	void DrawStone(HDC hdc, Stone stone);
	void DrawBoard(HDC hdc);
	
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	

};

