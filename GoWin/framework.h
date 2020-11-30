#pragma once
#pragma warning (disable : 4996)

// // SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.
// 이전 Windows 플랫폼용 애플리케이션을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT를 지원하려는 플랫폼으로 설정합니다.
#include <SDKDDKVer.h>
// #define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <WinSock2.h>
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
// C++ 런타임 헤더 파일입니다.
#include <iostream>
using namespace std;

#include "Image.h"
#include "ImageManager.h"
#include "resource.h"
#include "MySocket.h"
#include "MyString.h"
#include "Mywmkey.h"
#include "mydatastructure.h"

#define SPACE_SIZE 42
#define SAFE_RELEASE(p)	{ if(p) p->release(), delete p, p = nullptr; }
typedef POINT Coord2d;

enum class Color {
	Null,
	Black,
	White,
	Wall,
	Temp,
	END
};

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern const wchar_t* errorMSG_wchar[5];

inline wstring int2str_ngf(int index)
{
	wchar_t first = ++index / 26 + 65;
	wchar_t second = index % 26 + 65;

	return wstring({ first,second });
}
