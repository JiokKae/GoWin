#include "BoardGraphic.h"

bool BoardGraphic::IsMouseInBoard(Coord2d mouse)
{
	if (mouse.x >= m_left_top_point.x + m_border_size &&
		mouse.x <= m_left_top_point.x + m_width - m_border_size &&
		mouse.y >= m_left_top_point.y + m_border_size &&
		mouse.y <= m_left_top_point.y + m_height - m_border_size)
	{
		return true;
	}
	else
	{
		return false;
	}
}