#include "BoardGraphic.h"

BoardGraphic::BoardGraphic(HDC hdc)
{
	this->hdc = CreateCompatibleDC(hdc);
}

BoardGraphic::~BoardGraphic()
{
	DeleteDC(hdc);
}

void BoardGraphic::Draw(HDC destination, const Go& game, const Coord2d& mouse)
{
	if (bitmaps.contains("board") == true)
	{
		SelectObject(hdc, bitmaps["board"].handle());
		BitBlt(destination, m_left, m_top, Width(), Height(), hdc, 0, 0, SRCCOPY);
	}

	for (int x = 1; x < 20; x++)
	{
		for (int y = 1; y < 20; y++)
		{
			const Stone& stone = game.read_coord({ x, y });
			if (stone.state() != Stone::State::Stone)
			{
				continue;
			}
			DrawStone(destination, stone.x(), stone.y(), stone.color(), stone.sequence());
		}
	}

	if (IsPointInBoard(mouse.x, mouse.y) == true)
	{
		const Coord2d board_point{ PointToBoard(mouse.x), PointToBoard(mouse.y) };
		if (game.read_coord(board_point).state() == Stone::State::Null )
		{
			DrawStone(destination, board_point.x, board_point.y, game.current_placement_order(), 0, true);
		}
	}
}

void BoardGraphic::SetLeftTopPoint(int left, int top)
{
	m_left = left;
	m_top = top;
}

void BoardGraphic::SetSpaceSize(int space_size)
{
	m_space_size = space_size;
}

void BoardGraphic::SetBorderSize(int border_size)
{
	m_border_size = border_size;
}

void BoardGraphic::AddBitmap(const std::string& name, HINSTANCE hInst, int resource_id)
{
	bitmaps.emplace(name, GoWin::Graphic::Bitmap(hInst, resource_id));
}

void BoardGraphic::TogglePrintSequence()
{
	m_print_sequance = !m_print_sequance;
}

void BoardGraphic::DrawStone(HDC destination, int x, int y, Color color, int sequence, bool alpha)
{
	SelectStone(color);
	if (alpha == true)
	{
		GdiAlphaBlend(destination, m_space_size * (x - 1) + 6, m_space_size * (y - 1) + 6, 39, 39, hdc, 0, 0, 39, 39, ALPHA_BF);
		return;
	}

	BitBlt(destination, m_space_size * (x - 1) + 6, m_space_size * (y - 1) + 6, 39, 39, hdc, 0, 0, SRCCOPY);

	if (sequence > 0 && m_print_sequance == true)
	{
		SetBkMode(destination, TRANSPARENT);
		SetTextAlign(destination, TA_CENTER);
		if (color == Color::Black)
		{
			SetTextColor(destination, RGB(255, 255, 255));
		}
		else if (color == Color::White)
		{
			SetTextColor(destination, RGB(0, 0, 0));
		}
		TextOut(destination, m_space_size * (x - 1) + 25, m_space_size * (y - 1) + 18, std::to_wstring(sequence).c_str(), static_cast<int>(std::to_wstring(sequence).length()));
	}
}

bool BoardGraphic::IsPointInBoard(int x, int y) const
{
	if (x >= m_left + m_border_size && x <= m_left + Width() - m_border_size &&
		y >= m_top + m_border_size && y <= m_top + Height() - m_border_size)
	{
		return true;
	}

	return false;
}

int BoardGraphic::PointToBoard(int point) const
{
	const int boardPoint = (point - m_border_size) / m_space_size + 1;

	if (boardPoint < 1)
	{
		return 1;
	}

	if (boardPoint > 19)
	{
		return 19;
	}

	return boardPoint;
}

LONG BoardGraphic::Width() const
{
	return bitmaps.contains("board") == true ? bitmaps.at("board").width() : LONG{};
}

LONG BoardGraphic::Height() const
{
	return bitmaps.contains("board") == true ? bitmaps.at("board").height() : LONG{};
}

void BoardGraphic::SelectStone(Color color)
{
	switch (color)
	{
	case Color::Black:
		SelectObject(hdc, bitmaps["blackStone"].handle());
		break;
	case Color::White:
		SelectObject(hdc, bitmaps["whiteStone"].handle());
		break;
	default:
		break;
	}
}
