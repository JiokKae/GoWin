#include "stdgo.h"

void PlacementInfo::print( std::wostream& wos )
{
	wos << "[착수 정보]-----" << std::endl;
	wos << "수 : " << sequence << std::endl;
	wos << "플레이어 : " << Color2Char( player ) << std::endl;
	wos << "좌표 : (" << placment.x << ", " << placment.y << ")" << std::endl;
	wos << "----------------" << std::endl;
}

std::wstring PlacementInfo::to_sgf()
{
	wchar_t player = Color2Char( this->player );
	wchar_t x = ( placment.x + 19 ) % 20 + 'a';
	wchar_t y = ( placment.y + 19 ) % 20 + 'a';

	return std::wstring( { ';', player, '[', x, y, ']' } );
}

