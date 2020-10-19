#include "GoWinManager.h"

Go GoWinManager::GetGame()
{
	return game;
}

bool GoWinManager::GetPrintSequenceSwitch()
{
	return printSequanceSwitch;
}

void GoWinManager::SetPrintSequenceSwitch(bool b)
{
	printSequanceSwitch = b;
}

void GoWinManager::FileSave()
{
	tagOFNW SFN;
	WCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&SFN, 0, sizeof(tagOFNW));
	SFN.lStructSize = sizeof(tagOFNW);
	SFN.hwndOwner = hWnd;
	SFN.lpstrFilter = _T("기보 파일(*.ngf)\0*.ngf\0기보 파일(*.sgf)\0*.sgf\0모든 파일(*.*)\0*.*\0");
	SFN.lpstrDefExt = _T("ngf");
	SFN.lpstrFile = lpstrFile;
	SFN.nMaxFile = 256;
	SFN.Flags = OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&SFN) != 0)
	{
		wstring extension = lpstrFile;		//확장자 추출하기
		extension = extension.substr(extension.length() - 3, 3);
		Game.Save(lpstrFile, extension);
	}
}
