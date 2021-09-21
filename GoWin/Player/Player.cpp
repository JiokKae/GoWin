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

std::wstring Player::to_ngf() const
{
	std::wstring name = _T("           ");
	name.replace( 0, m_name.length(), m_name );

	return name + m_kyu;
}

void Player::print( std::wostream& wos ) const
{
	wos << _T( "[플레이어 정보]---------" ) << std::endl;
	wos << _T( "이름 : " ) << m_name << std::endl;
	wos << _T( "급 : " ) << m_kyu << std::endl;
	wos << _T( "따낸 돌 : " ) << m_captured_stone << std::endl;
	wos << _T( "------------------------" ) << std::endl;
}
