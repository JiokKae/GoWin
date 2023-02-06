#pragma once
#include "framework.h"
#include "Go.h"
#include "stdgo.h"

class BoardGraphic {
public:
	class Bitmap {
	public:
		Bitmap();
		Bitmap(HINSTANCE hInst, int resource_id);
		Bitmap(Bitmap&&) noexcept;
		~Bitmap();

		HANDLE handle() const;
	private:
		HANDLE m_handle;
	};

	void Init(HDC hdc, HINSTANCE hInst);
	void Release();
	void Draw(HDC destination, const Go& game, const Coord2d& mouse);
	
	void SetWidth(int width); 
	void SetHeight(int height); 
	void SetLeftTopPoint(Coord2d left_top_point);
	void SetSpaceSize(int space_size);
	void SetBorderSize(int border_size);
	void TogglePrintSequence();

	bool IsMouseInBoard(const Coord2d& mouseCoord) const;

	// 마우스 좌표를 보드 좌표로 변경
	Coord2d MouseToBoard(int x, int y);

private:
	void SelectStone(Color color);
	void DrawStone(HDC destination, int x, int y, Color color, int sequence, bool alpha = false);

	std::map<std::string, BoardGraphic::Bitmap> bitmaps;
	Coord2d m_left_top_point;
	int m_width;
	int m_height;
	int m_space_size;
	int m_border_size;
	HDC hdc;
	BLENDFUNCTION alphaBF;
	bool m_print_sequance;
};
