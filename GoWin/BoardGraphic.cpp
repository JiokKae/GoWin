#include "BoardGraphic.h"
#include "Go.h"
#include "Stone/Stone.h"

bool BoardGraphic::IsMouseInBoard(const Coord2d& mouseCoord) const
{
	if (mouseCoord.x >= m_left_top_point.x + m_border_size &&
		mouseCoord.x <= m_left_top_point.x + m_width - m_border_size &&
		mouseCoord.y >= m_left_top_point.y + m_border_size &&
		mouseCoord.y <= m_left_top_point.y + m_height - m_border_size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Coord2d BoardGraphic::MouseToBoard(int x, int y)
{
	Coord2d boardCoord;
	boardCoord.x = (x - m_border_size) / m_space_size + 1;
	boardCoord.y = (y - m_border_size) / m_space_size + 1;

	if (boardCoord.x < 1)	boardCoord.x = 1;
	if (boardCoord.x > 19)	boardCoord.x = 19;
	if (boardCoord.y < 1)	boardCoord.y = 1;
	if (boardCoord.y > 19)	boardCoord.y = 19;

	return boardCoord;
}

void BoardGraphic::Init(HDC hdc, HINSTANCE hInst)
{
	hdc_BlackStone = CreateCompatibleDC(hdc);
	hdc_WhiteStone = CreateCompatibleDC(hdc);
	hdc_Board = CreateCompatibleDC(hdc);

	HBITMAP bitBlackStone;
	HBITMAP bitWhiteStone;

	HBITMAP bitBoard;

	bitBlackStone = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLACKSTONE));
	bitWhiteStone = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WHITESTONE));
	bitBoard = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOARD));

	SelectObject(hdc_BlackStone, bitBlackStone);
	SelectObject(hdc_WhiteStone, bitWhiteStone);
	SelectObject(hdc_Board, bitBoard);

	DeleteObject(bitBlackStone);
	DeleteObject(bitWhiteStone);
	DeleteObject(bitBoard);

	alphaBF.AlphaFormat = AC_SRC_ALPHA;
	alphaBF.BlendFlags = 0;
	alphaBF.BlendOp = 0;
	alphaBF.SourceConstantAlpha = 180;
}

void BoardGraphic::Release()
{
	DeleteDC(hdc_BlackStone);
	DeleteDC(hdc_WhiteStone);
	DeleteDC(hdc_Board);
}

void BoardGraphic::Draw(HDC hdc)
{
	BitBlt(hdc, 0, 0, 806, 806, hdc_Board, 0, 0, SRCCOPY);

	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, TRANSPARENT);
	for (int x = 1; x < 20; x++)
	{
		for (int y = 1; y < 20; y++)
		{
			const Stone& stone = g_Game.ReadCoord({ x, y });
			if (stone.state() != Stone::State::Normal)
			{
				continue;
			}
			DrawStone(stone, hdc);

			int sequence = stone.sequence();
			if (sequence != 0 && m_print_sequance == true)
			{
				if (stone.color() == Color::Black)
				{
					SetTextColor(hdc, RGB(255, 255, 255));
				}
				else if (stone.color() == Color::White)
				{
					SetTextColor(hdc, RGB(0, 0, 0));
				}
				TextOut(hdc, m_space_size * (x - 1) + 25, m_space_size * (y - 1) + 18, std::to_wstring(sequence).c_str(), static_cast<int>(std::to_wstring(sequence).length()));
			}
		}
	}

	if (IsMouseInBoard(g_mouse))
	{
		Coord2d board_point = MouseToBoard(g_mouse.x, g_mouse.y);
		if ( g_Game.ReadCoord(board_point).state() == Stone::State::Null )
		{
			if (g_Game.info().sequence() % 2 == 1 )// Stone::Sqnce2color() == Color::Black)
				GdiAlphaBlend(hdc, m_space_size * (board_point.x - 1) + 6, m_space_size * (board_point.y - 1) + 6, 39, 39, hdc_BlackStone, 0, 0, 39, 39, alphaBF);
			else
				GdiAlphaBlend(hdc, m_space_size * (board_point.x - 1) + 6, m_space_size * (board_point.y - 1) + 6, 39, 39, hdc_WhiteStone, 0, 0, 39, 39, alphaBF);
		}
	}
}

void BoardGraphic::DrawStone(Stone stone, HDC hdc)
{
	int x = stone.x();
	int y = stone.y();
	Color color = stone.color();

	const HDC& hdc_stone = GetHDCStoneByColor(color);
	BitBlt(hdc, m_space_size * (x - 1) + 6, m_space_size * (y - 1) + 6, 39, 39, hdc_stone, 0, 0, SRCCOPY);
}

const HDC& BoardGraphic::GetHDCStoneByColor(Color color) const
{
	if (color == Color::Black)
	{
		return hdc_BlackStone;
	}
	else if (color == Color::White)
	{
		return hdc_WhiteStone;
	}
}

void BoardGraphic::SetSpaceSize(int space_size)
{
	m_space_size = space_size;
}

void BoardGraphic::SetBorderSize(int border_size)
{
	m_border_size = border_size;
}

void BoardGraphic::SetPrintSeqeance(bool b)
{
	m_print_sequance = b;
}

void BoardGraphic::TogglePrintSequence()
{
	m_print_sequance = !m_print_sequance;
}
