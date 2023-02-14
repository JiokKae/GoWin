#pragma once
#include <Windows.h>
#include "stdgo.h"
#include <string>

#define KEY_ENTER	0x0D
#define KEY_LEFT	0x25
#define KEY_RIGHT	0x27

// 윈도우 메세지를 문자열로 바꿔주는 함수
std::string ReadMessage(UINT message);

// 윈도우 마우스의 좌표를 저장하는 함수
void GetMouseCoord(Coord2d & coord, LPARAM lParam);
