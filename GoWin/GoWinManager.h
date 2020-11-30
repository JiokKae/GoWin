#pragma once
#include "framework.h"

class Go;
class Stone;
class HDCManager;
class BoardGraphic;
class GoWinManager
{
	Go* m_game;

	HDC hdc, hdcMem;
	HBITMAP hbmMem, hbmMemOld;
	HWND hChatInputBox, hChatBox;
	HWND hWCS, hBCS;
	HFONT hFont;
	POINT mouse;
	BoardGraphic* boardInfo;
	MySocket mysocket;

	Image* board;
	Image* background;

	bool printSequenceSwitch;
	bool is_init;

public:
	HRESULT init(HINSTANCE hInstance, HWND hWnd);
	void release();

	Go* game() { return m_game; }
	bool GetPrintSequenceSwitch();

	void SetPrintSequenceSwitch(bool b);

	void FileOpen();
	void FileSave();

	void DrawBoard(HDC hdc);
	
	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	
	GoWinManager();
	~GoWinManager();
};
