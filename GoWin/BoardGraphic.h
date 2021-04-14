#pragma once
#include "stdgo.h"
#include "framework.h"

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
	

	// 마우스의 좌표가 보드 안에 들어가 있는지
	bool IsMouseInBoard(Coord2d mouse);

	// 마우스 좌표를 보드 좌표로 변경
	Coord2d MouseToBoard(int x, int y) {
		Coord2d board;
		board.x = (x - m_border_size) / m_space_size + 1;
		board.y = (y - m_border_size) / m_space_size + 1;

		if (board.x < 1)	board.x = 1;
		if (board.x > 19)	board.x = 19;
		if (board.y < 1)	board.y = 1;
		if (board.y > 19)	board.y = 19;

		return board;
	}

};