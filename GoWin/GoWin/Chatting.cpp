#include "Chatting.h"
#include <format>
#include <tchar.h>

GoWin::Chatting::Chatting()
	: output_handle(nullptr)
	, m_input_handle(nullptr)
{
}

void GoWin::Chatting::set_output_handle(HWND handle)
{
	output_handle = handle;
}

void GoWin::Chatting::set_input_handle(HWND handle)
{
	m_input_handle = handle;
}

HWND GoWin::Chatting::input_handle() const
{
	return m_input_handle;
}

void GoWin::Chatting::print(LPCTSTR text)
{
	append_text_to_editbox(output_handle, std::format(_T("{}\r\n"), text).c_str());
}

void GoWin::Chatting::system_print(LPCTSTR text)
{
	append_text_to_editbox(output_handle, std::format(_T("[System] {}\r\n"), text).c_str());
}

void GoWin::Chatting::input_clear()
{
	SetWindowText(m_input_handle, _T(""));
}

bool GoWin::Chatting::input_empty()
{
	return GetWindowTextLength(m_input_handle) == 0;
}

std::wstring GoWin::Chatting::send()
{
	if (input_empty() == true)
	{
		return std::wstring();
	}

	TCHAR buffer[252];
	GetWindowText(m_input_handle, buffer, 252);
	print(std::format(_T("´ç½Å: {}"), buffer).c_str());
	input_clear();

	return std::wstring(buffer);
}

void GoWin::Chatting::append_text_to_editbox(HWND editbox, LPCTSTR text)
{
	SendMessage(editbox, EM_SETSEL, 0, -1);
	SendMessage(editbox, EM_SETSEL, -1, -1);
	SendMessage(editbox, EM_REPLACESEL, 0, LPARAM(text));
}