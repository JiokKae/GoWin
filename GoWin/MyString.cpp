#include "MyString.h"
#include <iostream>

using namespace std;

bool isStoi(string num)
{
	if (num.size() == 0)
		return false;
	for (int i = 0; i < (signed)num.size(); i++)
	{
		char a = num.at(i);
		if (a < 48 || a > 57)
			return false;
	}
	return true;
}

bool isWstoi(wstring num) 
{
	if (num.size() == 0)
		return false;
	for (int i = 0; i < (signed)num.size(); i++)
	{
		WCHAR a = num.at(i);
		printf("%c<%d> |",a , a);
		if (a < 48 || a > 57)
			return false;
	}
	return true;
}

wchar_t* CharToWChar(char* pstrSrc)
{
	if (pstrSrc == nullptr)
		return nullptr;
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t) * nLen);
	size_t ConvertedChars = 0;
	mbstowcs_s(&ConvertedChars, pwstr, nLen, pstrSrc, nLen);

	return pwstr;
}

char* WCharToChar(const wchar_t* pwstrSrc)
{
	int nLen = wcslen(pwstrSrc);
	char* pstr = (char*)malloc(sizeof(char) * nLen + 1);
	size_t ConvertedChars = 0;
	wcstombs_s(&ConvertedChars, pstr, nLen + 1, pwstrSrc, nLen + 1);

	return pstr;
}
