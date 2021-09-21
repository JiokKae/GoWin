#pragma once
#include <ostream>
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

	void print( std::wostream& wos );
	std::wstring to_sgf();
};
