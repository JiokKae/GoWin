#pragma once
#include "framework.h"
#include "stdgo.h"

class Stone;
class BoardGraphic {
private:
	Coord2d m_left_top_point;
	int m_width;
	int m_height;
	int m_space_size;
	int m_border_size;

	HDC hdc_BlackStone;
	HDC hdc_WhiteStone;
	HDC hdc_Board;

	bool m_print_sequance;
public:
	void Init(HDC hdc, HINSTANCE hInst);
	void Release();
	void Draw(HDC hdc);
	void DrawStone(Stone stone, HDC hdc);
	const HDC& GetHDCStoneByColor(Color color) const;

	// Setter
	void SetWidth(int width)			{ m_width = width; }
	void SetHeight(int height)			{ m_height = height; }
	void SetLeftTopPoint(Coord2d left_top_point)	{ m_left_top_point = left_top_point; }
	void SetSpaceSize(int space_size);
	void SetBorderSize(int border_size);
	void SetPrintSeqeance(bool);
	void TogglePrintSequence();

	// 접근자
	int width() const		{ return m_width; }
	int height() const		{ return m_height; }
	int space_size() const		{ return m_space_size; }
	int border_size() const		{ return m_border_size; }
	Coord2d left_top_point() const	{ return m_left_top_point; }
	
	bool IsMouseInBoard(const Coord2d& mouseCoord) const;

	// 마우스 좌표를 보드 좌표로 변경
	Coord2d MouseToBoard(int x, int y);
};
