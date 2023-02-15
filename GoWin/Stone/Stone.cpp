#include "Stone.h"
#include <format>

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

Stone::operator std::string() const
{
	return std::format("Stone({}, {})", m_x, m_y);
}

std::ostream& operator<<(std::ostream& os, const Stone& s)
{
	os << std::string(s) << "----------------" << std::endl;
	os << "    sequence: " << s.m_sequence << std::endl;
	os << "    backStone: " << (s.m_backStone ? std::string(*s.m_backStone) : "nullptr") << std::endl;
	os << "    nextStone: " << (s.m_nextStone ? std::string(*s.m_nextStone) : "nullptr") << std::endl;
	os << "    color: " << int(s.m_color) << std::endl;
	os << "    state: " << int(s.m_state) << std::endl;
	os << "    killer: " << s.m_killer << std::endl;
	os << "----------------------" << std::endl;
	return os;
}
