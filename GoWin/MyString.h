#pragma once

#include <string>
#include <windows.h>
#include <tchar.h>
using namespace std;

// string to int ����ȯ�� �������� �˻��ϴ� �Լ�
bool isStoi(string num);
// wstring to int ����ȯ�� �������� �˻��ϴ� �Լ�
bool isWstoi(wstring num);
// char* to WCHAR* ����ȯ �Լ�
wchar_t* CharToWChar(char* pstrSrc);
// WCHAR* to char* ����ȯ �Լ�
char* WCharToChar(const wchar_t* pwstrSrc);
