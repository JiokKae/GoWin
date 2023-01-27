#pragma once
#include <string>
#include <windows.h>

namespace GoWin 
{
	class OpenFileName 
	{
	public:
		struct result 
		{
			std::wstring file_path;
			bool success;
		};

		OpenFileName(HWND hWnd, const TCHAR* filter, DWORD flags);

		OpenFileName::result open();
	private:
		TCHAR file_path[MAX_PATH];
		OPENFILENAME ofn;
		const TCHAR* filter;
	};
}
