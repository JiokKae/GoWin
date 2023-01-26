#include "Stone.h"

Stone::Stone( int x, int y, int sequence, Color color )
	: m_x( x )
	, m_y( y )
	, m_sequence( sequence )
	, m_color( color )
	, m_backStone( nullptr )
	, m_nextStone( nullptr )
	, m_killer( false )
	, m_state( State::Normal )
{
}

Stone::Stone( int x, int y, int sequence, std::string temp ) 
	: m_x( x )
	, m_y( y )
	, m_sequence( sequence )
	, m_backStone( nullptr )
	, m_nextStone( nullptr )
	, m_killer( false )
	, m_state( State::Temp )
{
}

Stone::Stone()
	: m_x( 1 )
	, m_y( 1 )
	, m_sequence( 0 )
	, m_backStone( nullptr )
	, m_nextStone( nullptr )
	, m_killer( false )
	, m_state( State::Null )
{
}

Stone::Stone( std::string Wall )
	: m_x( 0 )
	, m_y( 0 )
	, m_sequence( 0 )
	, m_backStone( nullptr )
	, m_nextStone( nullptr )
	, m_killer( false )
	, m_state( State::Wall )
{
}

void Stone::Print(std::ostream& os) const
{
	os << "Stone ----------------" << std::endl;
	os << "    x: " << m_x << std::endl;
	os << "    y: " << m_y << std::endl;
	os << "    sequence: " << m_sequence << std::endl;
	os << "    backStone: " << m_backStone << std::endl;
	os << "    nextStone: " << m_nextStone << std::endl;
	os << "    color: " << (int)m_color << std::endl;
	os << "    state: " << (int)m_state << std::endl;
	os << "    killer: " << m_killer << std::endl;
	os << "----------------------" << std::endl;
}
