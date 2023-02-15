#pragma once

#include "../stdgo.h"

class Stone 
{
public:
	enum class State 
	{
		Null,
		Normal,
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
	State state() const	{ return m_state; }
	int sequence() const	{ return m_sequence; }
	bool is_killer() const	{ return m_killer; }

	Stone* backStone()	{ return m_backStone; }
	Stone* nextStone()	{ return m_nextStone; }

	void set_backStone( Stone* s )	{ m_backStone = s; }
	void set_nextStone( Stone* s )	{ m_nextStone = s; }
	void set_killer( bool killer )	{ m_killer = killer; }

	operator std::string() const;
	friend std::ostream& operator<<(std::ostream&, const Stone&);
private:
	int m_x;
	int m_y;
	int m_sequence;
	Stone* m_backStone;
	Stone* m_nextStone;
	Color m_color;
	State m_state;
	bool m_killer;
};
