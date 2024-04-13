#pragma once

#include "../stdgo.h"
#include "../Grid/GridItem.h"

class Stone : public GridItem
{
public:
	Stone( int x, int y, int sequence, Color color );
	Stone( int x, int y, int sequence, std::string temp );
	Stone(); 			// ºó µ¹
	Stone( std::string Wall );	// º®

	Color color() const	{ return m_color; }
	int sequence() const	{ return m_sequence; }
	bool is_killer() const	{ return m_killer; }

	Stone* backStone()	{ return m_backStone; }
	Stone* nextStone()	{ return m_nextStone; }

	void set_backStone( Stone* s )	{ m_backStone = s; }
	void set_nextStone( Stone* s )	{ m_nextStone = s; }
	void set_killer( bool killer )	{ m_killer = killer; }

	virtual operator std::string() const override;
private:
	virtual value_array values() const override;

private:
	int m_sequence = 0;
	Stone* m_backStone = nullptr;
	Stone* m_nextStone = nullptr;
	Color m_color = Color::Black;
	bool m_killer = false;
};
