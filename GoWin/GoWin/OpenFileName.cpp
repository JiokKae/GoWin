#include "OpenFileName.h"

GoWin::OpenFileName::OpenFileName(HWND hWnd, const TCHAR* filter, DWORD flags)
	: file_path()
	, ofn{ .lStructSize = sizeof(OPENFILENAME),
		.hwndOwner = hWnd,
		.lpstrFilter = filter,
		.lpstrFile = file_path,
		.nMaxFile = MAX_PATH,
		.Flags = flags }
{
}

GoWin::OpenFileName::result GoWin::OpenFileName::open()
{
	memset(file_path, 0, MAX_PATH);
	bool success(GetOpenFileName(&ofn) != 0);
	return OpenFileName::result{ file_path, success };
}

GoWin::OpenFileName::result GoWin::OpenFileName::save(const TCHAR* defExt)
{
	ofn.lpstrDefExt = defExt;
	memset(file_path, 0, MAX_PATH);
	bool success(GetSaveFileName(&ofn) != 0);
	return OpenFileName::result{ file_path, success };
}
