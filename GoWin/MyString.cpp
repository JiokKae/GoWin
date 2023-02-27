#include "MyString.h"
#include <windows.h>

wchar_t* CharToWChar(const char* pstrSrc)
{
	if (pstrSrc == nullptr)
		return nullptr;
	int nLen = static_cast<int>( strlen(pstrSrc) + 1 );
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t) * nLen);
	size_t ConvertedChars = 0;
	mbstowcs_s(&ConvertedChars, pwstr, nLen, pstrSrc, nLen);

	return pwstr;
}

char* WCharToChar(const wchar_t* pwstrSrc)
{
	size_t nLen = wcslen( pwstrSrc );
	char* pstr = (char*)malloc(sizeof(char) * nLen + 1);
	size_t ConvertedChars = 0;
	wcstombs_s(&ConvertedChars, pstr, nLen + 1, pwstrSrc, nLen + 1);

	return pstr;
}
