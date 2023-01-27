#include "OpenFileName.h"

GoWin::OpenFileName::OpenFileName(HWND hWnd, const TCHAR* filter, DWORD flags) : filter(filter)
{
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = this->file_path;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = flags;
}

GoWin::OpenFileName::result GoWin::OpenFileName::open()
{
	memset(file_path, 0, MAX_PATH);
	bool success = GetOpenFileName(&ofn) != 0;
	return OpenFileName::result(std::wstring(file_path), success);
}
