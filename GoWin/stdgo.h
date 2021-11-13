#pragma once
#include <ostream>
#include <string>

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
		: placement(x, y)
		, sequence(sequence)
		, player(player)
	{
	}

	PlacementInfo(Coord2d placement, int sequence, Color player)
		: placement(placement)
		, sequence(sequence)
		, player(player)
	{
	}

	Coord2d placement;
	int sequence;
	Color player;
	
	void print( std::wostream& wos ) const;
	std::wstring to_sgf() const;
};
