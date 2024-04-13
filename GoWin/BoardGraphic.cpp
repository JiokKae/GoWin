#include "BoardGraphic.h"
#include "resource.h"

BoardGraphic::Bitmap::Bitmap()
	: m_handle(nullptr)
{
}

BoardGraphic::Bitmap::Bitmap(HINSTANCE hInst, int resource_id)
	: m_handle(LoadImage(hInst, MAKEINTRESOURCE(resource_id), IMAGE_BITMAP, 0, 0, NULL))
{
}

BoardGraphic::Bitmap::Bitmap(Bitmap&& bitmap) noexcept
	: m_handle(std::exchange(bitmap.m_handle, nullptr))
{
}

BoardGraphic::Bitmap::~Bitmap()
{
	if (m_handle != nullptr) DeleteObject(m_handle);
}

HANDLE BoardGraphic::Bitmap::handle() const
{
	return m_handle;
}

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
	this->hdc = CreateCompatibleDC(hdc);
	bitmaps.emplace("blackStone", BoardGraphic::Bitmap(hInst, IDB_BLACKSTONE));
	bitmaps.emplace("whiteStone", BoardGraphic::Bitmap(hInst, IDB_WHITESTONE));
	bitmaps.emplace("board", BoardGraphic::Bitmap(hInst, IDB_BOARD));

	alphaBF = { .BlendOp = 0, .BlendFlags = 0, .SourceConstantAlpha = 180, .AlphaFormat = AC_SRC_ALPHA };
}

void BoardGraphic::Release()
{
	DeleteDC(hdc);
}

void BoardGraphic::Draw(HDC destination, const Go& game, const Coord2d& mouse)
{
	SelectObject(hdc, bitmaps["board"].handle());
	BitBlt(destination, 0, 0, 806, 806, hdc, 0, 0, SRCCOPY);

	SetTextAlign(destination, TA_CENTER);
	SetBkMode(destination, TRANSPARENT);
	for (int x = 1; x < 20; x++)
	{
		for (int y = 1; y < 20; y++)
		{
			const Stone& stone = game.ReadCoord({ x, y });
			if (stone.state() != Stone::State::Stone)
			{
				continue;
			}
			DrawStone(destination, stone.x(), stone.y(), stone.color(), stone.sequence());
		}
	}

	if (IsMouseInBoard(mouse) == true)
	{
		Coord2d board_point = MouseToBoard(mouse.x, mouse.y);
		if (game.ReadCoord(board_point).state() == Stone::State::Null )
		{
			DrawStone(destination, board_point.x, board_point.y, game.get_current_placement_order(), 0, true);
		}
	}
}

void BoardGraphic::DrawStone(HDC destination, int x, int y, Color color, int sequence, bool alpha)
{
	SelectStone(color);
	if (alpha == true)
	{
		GdiAlphaBlend(destination, m_space_size * (x - 1) + 6, m_space_size * (y - 1) + 6, 39, 39, hdc, 0, 0, 39, 39, alphaBF);
		return;
	}

	BitBlt(destination, m_space_size * (x - 1) + 6, m_space_size * (y - 1) + 6, 39, 39, hdc, 0, 0, SRCCOPY);

	if (sequence != 0 && m_print_sequance == true)
	{
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

void BoardGraphic::SetWidth(int width)
{
	m_width = width;
}

void BoardGraphic::SetHeight(int height)
{
	m_height = height;
}

void BoardGraphic::SetLeftTopPoint(Coord2d left_top_point)
{
	m_left_top_point = left_top_point;
}

void BoardGraphic::SetSpaceSize(int space_size)
{
	m_space_size = space_size;
}

void BoardGraphic::SetBorderSize(int border_size)
{
	m_border_size = border_size;
}

void BoardGraphic::TogglePrintSequence()
{
	m_print_sequance = !m_print_sequance;
}
