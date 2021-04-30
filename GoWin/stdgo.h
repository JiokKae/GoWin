#pragma once
#include <iostream>
#include <string>
#include <tchar.h>
#include "Stone.h"
using namespace std;

struct Coord2d {
	int x;
	int y;
};

struct PlacementInfo
{
	int sequence;
	Color player;
	Coord2d placment;
};

static void print_data(PlacementInfo data)
{
	cout << "[착수 정보]-----" << endl;
	cout << "수 : " << data.sequence << endl;
	cout << "플레이어 : " << Stone::Color2Char(data.player) << endl;
	cout << "좌표 : (" << data.placment.x << ", " << data.placment.y << ")" << endl;
	cout << "----------------" << endl;
}

static wstring data2str_sgf(PlacementInfo data)
{
	wchar_t player = Stone::Color2Char(data.player);
	wchar_t x = (data.placment.x + 19) % 20 + 97;
	wchar_t y = (data.placment.y + 19) % 20 + 97;

	return wstring({ ';', player, '[', x, y, ']' });
}
