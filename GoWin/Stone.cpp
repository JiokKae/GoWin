#include "Stone.h"

HRESULT Stone::init()
{
	m_sequence = 0;
	m_color = Color::Null;
	m_backStone = nullptr;
	m_nextStone = nullptr;
	m_killer = false;

	image[int(Color::White)] = ImageManager::GetSingleton()->FindImage("WhiteStone");
	image[int(Color::Black)] = ImageManager::GetSingleton()->FindImage("BlackStone");

	return S_OK;
}

void Stone::release()
{

}

void Stone::render(HDC hdc)
{
	if (image[int(m_color)])
	{
		image[int(m_color)]->Render(hdc, SPACE_SIZE * (m_x - 1) + 6, SPACE_SIZE * (m_y - 1) + 6);

		if (m_color == Color::Black)
		{
			SetTextColor(hdc, RGB(255, 255, 255));
		}
		else if (m_color == Color::White)
		{
			SetTextColor(hdc, RGB(0, 0, 0));
		}
	}
		
}

void Stone::place(int x, int y, int sequence)
{
	m_x = x;
	m_y = y;
	m_sequence = sequence;
	m_color = Sqnce2color(sequence);
	m_backStone = nullptr;
	m_nextStone = nullptr;
	m_killer = false;
}

void Stone::place_temp(int x, int y, int sequence, bool is_temp)
{
	m_x = x;
	m_y = y;
	m_sequence = sequence;
	m_color = Color::Temp;
	m_backStone = nullptr;
	m_nextStone = nullptr;
	m_killer = false;
	m_is_temp = true;
}

Stone::Stone() : 
	m_x(1),	m_y(1),	m_sequence(0), m_color(Color::Null),
	m_backStone(nullptr), m_nextStone(nullptr),
	m_killer(false)
{

}