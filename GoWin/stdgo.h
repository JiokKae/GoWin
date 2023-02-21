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
	Coord2d(int x = 0, int y = 0) 
		: x(x)
		, y(y)
	{
	}

	int x;
	int y;
};

struct PlacementInfo
{
	PlacementInfo(int x, int y, int sequence, Color player)
		: x(x)
		, y(y)
		, sequence(sequence)
		, player(player)
	{
	}

	int x, y;
	int sequence;
	Color player;

	friend std::wostream& operator<<(std::wostream&, const PlacementInfo&);
};
