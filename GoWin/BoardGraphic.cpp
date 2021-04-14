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
			DrawStone(g_Game.Read({ x, y }), hdc);

			int sequence = g_Game.Read({ x, y }).sequence();
			if (sequence != 0 && m_print_sequance == true)
			{
				TextOut(hdc, m_space_size * (x - 1) + 25, m_space_size * (y - 1) + 18, std::to_wstring(sequence).c_str(), std::to_wstring(sequence).length());
			}
		}
	}

	if (IsMouseInBoard(g_mouse))
	{
		Coord2d board_point = MouseToBoard(g_mouse.x, g_mouse.y);
		if (g_Game.Read(board_point).color() == Color::Null)
		{
			BLENDFUNCTION bf;
			bf.AlphaFormat = AC_SRC_ALPHA;
			bf.BlendFlags = 0;
			bf.BlendOp = 0;
			bf.SourceConstantAlpha = 180;

			if (Stone::Sqnce2color(g_Game.info().sequence()) == Color::Black)
				GdiAlphaBlend(hdc, m_space_size * (board_point.x - 1) + 6, m_space_size * (board_point.y - 1) + 6, 39, 39, hdc_BlackStone, 0, 0, 39, 39, bf);
			else
				GdiAlphaBlend(hdc, m_space_size * (board_point.x - 1) + 6, m_space_size * (board_point.y - 1) + 6, 39, 39, hdc_WhiteStone, 0, 0, 39, 39, bf);
		}
	}
}

void BoardGraphic::DrawStone(Stone stone, HDC hdc)
{
	int x = stone.x();
	int y = stone.y();
	Color color = stone.color();

	if (color == Color::Black)
	{
		BitBlt(hdc, m_space_size * (x - 1) + 6, m_space_size * (y - 1) + 6, 39, 39, hdc_BlackStone, 0, 0, SRCCOPY);
		SetTextColor(hdc, RGB(255, 255, 255));
	}
	else if (color == Color::White)
	{
		BitBlt(hdc, m_space_size * (x - 1) + 6, m_space_size * (y - 1) + 6, 39, 39, hdc_WhiteStone, 0, 0, SRCCOPY);
		SetTextColor(hdc, RGB(0, 0, 0));
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
