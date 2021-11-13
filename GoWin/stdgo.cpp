#include "stdgo.h"

void PlacementInfo::print( std::wostream& wos ) const
{
	wos << "[착수 정보]-----" << std::endl;
	wos << "수 : " << sequence << std::endl;
	wos << "플레이어 : " << Color2Char( player ) << std::endl;
	wos << "좌표 : (" << placement.x << ", " << placement.y << ")" << std::endl;
	wos << "----------------" << std::endl;
}

std::wstring PlacementInfo::to_sgf() const
{
	wchar_t player = Color2Char( this->player );
	wchar_t x = (placement.x + 19 ) % 20 + 'a';
	wchar_t y = (placement.y + 19 ) % 20 + 'a';

	return std::wstring( { ';', player, '[', x, y, ']' } );
}

Color Reverse(Color color)
{
	if (color == Color::White)
		return Color::Black;
	else
		return Color::White;
}

char Color2Char( Color color )
{
	return ( color == Color::White ) ? 'W' : 'B';
}
