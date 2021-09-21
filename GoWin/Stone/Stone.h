#pragma once
#include <string>

class Stone 
{
public:
	enum class Color 
	{
		Null,
		Black,
		White,
		Wall,
		Temp,
	};

public:
	Stone( int x, int y, int sequence, Color color );
	Stone( int x, int y, int sequence, std::string temp );
	Stone(); 			// ºó µ¹
	Stone( std::string Wall );	// º®

	int x() const		{ return m_x; }
	int y() const		{ return m_y; }
	Color color() const	{ return m_color; }
	int sequence() const	{ return m_sequence; }
	bool is_killer() const	{ return m_killer; }

	const Stone* backStone()	{ return m_backStone; }
	const Stone* nextStone()	{ return m_nextStone; }

	void set_backStone( Stone* s )	{ m_backStone = s; }
	void set_nextStone( Stone* s )	{ m_nextStone = s; }
	void set_killer( bool killer )	{ m_killer = killer; }

	static Color Reverse( Color color )
	{ 
		if ( color == Color::White )
			return Color::Black;
		else if ( color == Color::Black )
			return Color::White;
		else
			return Color::Null;
	}
	static char Color2Char( Color color ) { return ( color == Color::White ) ? 'W' : 'B'; }

private:
	int m_x;
	int m_y;
	int m_sequence;
	Stone* m_backStone;
	Stone* m_nextStone;
	bool m_killer;
	Color m_color;
};
