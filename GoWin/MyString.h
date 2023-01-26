#pragma once

#include <string>
#include <tchar.h>

// string to int ����ȯ�� �������� �˻��ϴ� �Լ�
bool isStoi( const std::string& num );
// wstring to int ����ȯ�� �������� �˻��ϴ� �Լ�
bool isWstoi( const std::wstring& num ) ;
// char* to WCHAR* ����ȯ �Լ�
wchar_t* CharToWChar( const char* pstrSrc );
// WCHAR* to char* ����ȯ �Լ�
char* WCharToChar( const wchar_t* pwstrSrc );