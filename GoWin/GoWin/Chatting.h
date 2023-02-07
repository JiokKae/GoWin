#pragma once
#include <string>
#include <wtypes.h>

namespace GoWin
{
	class Chatting
	{
	public:
		Chatting();

		void set_output_handle(HWND);
		void set_input_handle(HWND);

		HWND input_handle() const;

		void print(LPCTSTR text);
		void system_print(LPCTSTR text);
		void input_clear();
		bool input_empty();
		std::wstring send();
	private:
		void append_text_to_editbox(HWND editbox, LPCTSTR text);

		HWND output_handle;
		HWND m_input_handle;
	};
}
