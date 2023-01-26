#include "Player.h"

Player::Player( Color color )
	: m_name( _T( "player" ) )
	, m_kyu( _T( "18급" ) )
	, m_color( color )
	, m_captured_stone( 0 )
{
}


bool Player::init( Color color )
{
	m_name = _T( "player" );
	m_kyu = _T( "18급" ); 
	m_color = color;
	m_captured_stone = 0;

	return true;
}

std::wstring Player::to_wstring() const
{
	return _T( "이름 : " ) + m_name + _T( " 급 : " ) + m_kyu + _T( " 따낸 돌 : " ) + std::to_wstring( m_captured_stone );
}

void Player::print( std::wostream& wos ) const
{
	wos << _T( "[Player]---------" ) << std::endl;
	wos << _T( "이름 : " ) << m_name << std::endl;
	wos << _T( "급 : " ) << m_kyu << std::endl;
	wos << _T( "따낸 돌 : " ) << m_captured_stone << std::endl;
	wos << _T( "------------------------" ) << std::endl;
}
