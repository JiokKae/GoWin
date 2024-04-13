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
	void set_killer( bool killer )	{ m_killer = killer; }

	virtual operator std::string() const override;
private:
	virtual value_array values() const override;

private:
	int m_sequence = 0;
	Color m_color = Color::None;
	bool m_killer = false;
};
