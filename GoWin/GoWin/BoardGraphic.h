#pragma once
#include <map>
#include <string>
#include <wtypes.h>

#include "../Go.h"
#include "Graphic.h"

class BoardGraphic 
{
public:
	BoardGraphic(HDC hdc);
	~BoardGraphic();

	void Draw(HDC destination, const Go& game, const Coord2d& mouse);
	
	void SetLeftTopPoint(int left, int top);
	void SetSpaceSize(int space_size);
	void SetBorderSize(int border_size);
	void AddBitmap(const std::string& name, HINSTANCE hInst, int resource_id);
	void TogglePrintSequence();

	bool IsPointInBoard(int x, int y) const;
	int PointToBoard(int point) const;

private:
	LONG Width() const;
	LONG Height() const;
	void SelectStone(Color color);
	void DrawStone(HDC destination, int x, int y, Color color, int sequence, bool alpha = false);

	std::map<std::string, GoWin::Graphic::Bitmap> bitmaps;
	int m_left;
	int m_top;
	int m_space_size;
	int m_border_size;
	HDC hdc;
	bool m_print_sequance;
	static constexpr BLENDFUNCTION ALPHA_BF = { .BlendOp = 0, .BlendFlags = 0, .SourceConstantAlpha = 180, .AlphaFormat = AC_SRC_ALPHA };
};
