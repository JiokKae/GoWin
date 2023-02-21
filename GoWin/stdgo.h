#pragma once
#include <ostream>

enum class Color
{
	Black,
	White,
};
Color Reverse( Color color );
char Color2Char( Color color ); 

struct Coord2d 
{
	int x;
	int y;
};

struct PlacementInfo
{
	int x, y;
	int sequence;
	Color player;

	friend std::wostream& operator<<(std::wostream&, const PlacementInfo&);
};
