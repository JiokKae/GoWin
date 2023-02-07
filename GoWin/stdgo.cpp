#include "stdgo.h"
#include <tchar.h>
#include <format>

std::wstring PlacementInfo::to_sgf() const
{
	TCHAR x = (this->x + 19) % 20 + 'a';
	TCHAR y = (this->y + 19) % 20 + 'a';

	return std::format(_T(";{}[{}{}]", Color2Char(this->player), x, y));
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

std::wostream& operator<<(std::wostream& wos, const PlacementInfo& p)
{
	wos << "[착수 정보]-----" << std::endl;
	wos << "수 : " << p.sequence << std::endl;
	wos << "플레이어 : " << Color2Char(p.player) << std::endl;
	wos << "좌표 : (" << p.x << ", " << p.y << ")" << std::endl;
	wos << "----------------" << std::endl;
	return wos;
}
