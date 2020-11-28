// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//
#pragma once

// // SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
// 이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다.
#include <SDKDDKVer.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
using namespace std;

#include "MySocket.h"
#include "MyString.h"
#include "Mywmkey.h"
#include "GiboSGF.h"
#include "HDCManager.h"
#include "GoWinManager.h"

typedef POINT Coord2d;

enum class Color {
	Null,
	Black,
	White,
	Wall,
	Temp,
};

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

const wchar_t* errorMSG_wchar[5] = {
	_T(""),
	_T("바둑판 안에 착수해주세요"),
	_T("이미 바둑 돌이 있습니다"),
	_T("착수 금지점입니다"),
	_T("패 입니다"),
};

inline wstring int2str_ngf(int index)
{
	wchar_t first = ++index / 26 + 65;
	wchar_t second = index % 26 + 65;

	return wstring({ first,second });
}
