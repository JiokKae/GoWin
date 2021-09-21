#pragma once
#include <iostream>
#include <string>
#include <tchar.h>
#include "Stone/Stone.h"

enum class Color
{
	Black,
	White,
};

static Color Reverse( Color color )
{
	if (color == Color::White)
		return Color::Black;
	else
		return Color::White;
}
static char Color2Char( Color color ) { return ( color == Color::White ) ? 'W' : 'B'; }

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
	std::cout << "[착수 정보]-----" << std::endl;
	std::cout << "수 : " << data.sequence << std::endl;
	std::cout << "플레이어 : " << Color2Char(data.player) << std::endl;
	std::cout << "좌표 : (" << data.placment.x << ", " << data.placment.y << ")" << std::endl;
	std::cout << "----------------" << std::endl;
}

static std::wstring data2str_sgf(PlacementInfo data)
{
	wchar_t player = Color2Char(data.player);
	wchar_t x = (data.placment.x + 19) % 20 + 97;
	wchar_t y = (data.placment.y + 19) % 20 + 97;

	return std::wstring({ ';', player, '[', x, y, ']' });
}
