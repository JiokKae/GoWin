#pragma once
#include <functional>
#include "framework.h"
#include "Go.h"
#include "MySocket.h"
#include "BoardGraphic.h"
#include "GoWin/Chatting.h"

class GoWinApplication
{
public:
	GoWinApplication();

	void set_main_window_handle(HWND);
	void set_hInstance(HINSTANCE);

	HINSTANCE hInstance();
	HWND main_window_handle();

	LRESULT CALLBACK main_procedure(HWND hWnd, UINT message, WPARAM, LPARAM);
	INT_PTR CALLBACK about_procedure(HWND hDlg, UINT message, WPARAM, LPARAM);
	INT_PTR CALLBACK netbox_procedure(HWND hDlg, UINT message, WPARAM, LPARAM);

private:
	const TCHAR* ERROR_MESSAGES[5] = {
		_T(""),
		_T("바둑판 안에 착수해주세요"),
		_T("이미 바둑 돌이 있습니다"),
		_T("착수 금지점입니다"),
		_T("패 입니다"),
	};
	enum class string_id {
		INVALID_EXTENSION,
		FILE_OPEN_FAIL_TITLE,
		FILE_OPEN_FAIL,
		FILE_SAVE_FAIL_TITLE,
		FILE_SAVE_FAIL,
	};
	void create(HWND);
	void file_open(HWND);
	void file_save(HWND);
	void backsies(HWND);
	void init(HWND);
	void pass(HWND);
	void set_mouse_coord(LPARAM);
	std::wstring get_extension(const std::wstring& path);
	void ok_message_box(HWND, const std::wstring& title, const std::wstring& content);
	
	Go go;
	BoardGraphic board_graphic;
	MySocket my_socket;
	GoWin::Chatting chatting;

	using callback = std::function<void(HWND)>;
	std::map<Command_MSG::Command, callback> command_message_callbacks;
	std::map<string_id, std::wstring> strings;

	Coord2d mouse;
	HINSTANCE m_hInstance;
	PAINTSTRUCT ps;
	HWND m_main_window_handle;
	HWND ip_input_box; 
	HBITMAP hBitmapMem;
	HDC hdc, hdcMem;
	HDC hdc_BackGround;
	HWND hWCS, hBCS;
	HFONT hFont;
};