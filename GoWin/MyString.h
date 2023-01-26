#pragma once

#include <string>
#include <tchar.h>

// string to int 형변환이 가능한지 검사하는 함수
bool isStoi( const std::string& num );
// wstring to int 형변환이 가능한지 검사하는 함수
bool isWstoi( const std::wstring& num ) ;
// char* to WCHAR* 형변환 함수
wchar_t* CharToWChar( const char* pstrSrc );
// WCHAR* to char* 형변환 함수
char* WCharToChar( const wchar_t* pwstrSrc );