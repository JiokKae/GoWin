#pragma once
#include <Windows.h>
#include "stdgo.h"

#define KEY_ENTER	0x0D
#define KEY_LEFT	0x25
#define KEY_RIGHT	0x27

// ������ �޼����� ���ڿ��� �ٲ��ִ� �Լ�
char* ReadMessage(UINT message, char* buffer);

// ������ ���콺�� ��ǥ�� �����ϴ� �Լ�
void GetMouseCoord(Coord2d & coord, LPARAM lParam);
