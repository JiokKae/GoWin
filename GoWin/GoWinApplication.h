#pragma once
#include <functional>
#include "framework.h"
#include "Go.h"
#include "MySocket.h"
#include "BoardGraphic.h"
#include "GoWin/Chatting.h"
#include "GoWin/Font.h"

class GoWinApplication
{
public:
	GoWinApplication();

	void set_main_window_handle(HWND);
	void set_hInstance(HINSTANCE);

	HINSTANCE hInstance() const;
	HWND main_window_handle() const;

	LRESULT CALLBACK main_procedure(HWND hWnd, UINT message, WPARAM, LPARAM);
	INT_PTR CALLBACK about_procedure(HWND hDlg, UINT message, WPARAM, LPARAM);
	INT_PTR CALLBACK netbox_procedure(HWND hDlg, UINT message, WPARAM, LPARAM);

private: // procedure callback
	std::map<UINT, std::function<void(HWND, WPARAM, LPARAM)>> procedure_callbacks;

	void on_create(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void on_mouse_move(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void on_lbutton_down(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void on_command(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void on_async(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void on_paint(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam);

private:
	std::map<Command_MSG::Command, std::function<void(HWND)>> command_message_callbacks;

private:
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

	Coord2d mouse;
	PAINTSTRUCT ps;
	GoWin::Font font_gungseo;

	HINSTANCE m_hInstance = nullptr;
	HWND m_main_window_handle = nullptr;
	HWND ip_input_box = nullptr;
	HBITMAP hBitmapMem = nullptr;
	HDC hdc = nullptr;
	HDC hdcMem = nullptr;
	HDC hdc_BackGround = nullptr;
	HWND hWCS = nullptr;
	HWND hBCS = nullptr;

private:	
	enum class StringID {
		INVALID_EXTENSION,
		FILE_OPEN_FAIL_TITLE,
		FILE_OPEN_FAIL,
		FILE_SAVE_FAIL_TITLE,
		FILE_SAVE_FAIL,
	};
	static const std::map<StringID, std::wstring> STRINGS;

	static const TCHAR* ERROR_MESSAGES[5];

	static constexpr int TOGGLE_SHOW_SEQUENCE = 3;
};
