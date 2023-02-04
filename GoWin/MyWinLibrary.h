#pragma once
#include <Windows.h>
#include "stdgo.h"

#define KEY_ENTER	0x0D
#define KEY_LEFT	0x25
#define KEY_RIGHT	0x27

// 윈도우 메세지를 문자열로 바꿔주는 함수
char* ReadMessage(UINT message, char* buffer);

// 윈도우 마우스의 좌표를 저장하는 함수
void GetMouseCoord(Coord2d & coord, LPARAM lParam);
