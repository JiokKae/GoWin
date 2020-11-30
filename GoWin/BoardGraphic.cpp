#include "BoardGraphic.h"

HRESULT BoardGraphic::init(Coord2d left_top_point, int width, int height, int space_size, int border_size)
{
	m_left_top_point = left_top_point;
	m_width = width;
	m_height = height;
	m_space_size = space_size;
	m_border_size = border_size;

	return S_OK;
}

void BoardGraphic::release()
{
}

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