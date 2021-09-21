#include "Stone.h"

Stone::Stone( int x, int y, int sequence, Color color )
	: m_x( x )
	, m_y( y )
	, m_sequence( sequence )
	, m_color( color )
	, m_backStone( nullptr )
	, m_nextStone( nullptr )
	, m_killer( false )
{
}

Stone::Stone( int x, int y, int sequence, std::string temp ) 
	: m_x( x )
	, m_y( y )
	, m_sequence( sequence )
	, m_color( Color::Temp )
	, m_backStone( nullptr )
	, m_nextStone( nullptr )
	, m_killer( false )
{
}

Stone::Stone()
	: m_x( 1 )
	, m_y( 1 )
	, m_sequence( 0 )
	, m_color( Color::Null )
	, m_backStone( nullptr )
	, m_nextStone( nullptr )
	, m_killer( false )
{
}

Stone::Stone( std::string Wall )
	: m_x( 0 )
	, m_y( 0 )
	, m_sequence( 0 )
	, m_color( Color::Wall )
	, m_backStone( nullptr )
	, m_nextStone( nullptr )
	, m_killer( false )
{
}
