#pragma once
#include "stdgo.h"

class BoardGraphic {
	Coord2d m_left_top_point;
	int m_width;
	int m_height;
	int m_space_size;
	int m_border_size;
public:
	BoardGraphic(Coord2d left_top_point, int width, int height, int space_size, int border_size)
	{
		m_left_top_point = left_top_point;
		m_width = width;
		m_height = height; 
		m_space_size = space_size;
		m_border_size = border_size;
	}

	// ������
	void SetWidth(int width)						{ m_width = width; }
	void SetHeight(int height)						{ m_height = height; }
	void SetLeftTopPoint(Coord2d left_top_point)	{ m_left_top_point = left_top_point; }

	// ������
	int width() const				{ return m_width; }
	int height() const				{ return m_height; }
	int space_size() const			{ return m_space_size; }
	int border_size() const			{ return m_border_size; }
	Coord2d left_top_point() const	{ return m_left_top_point; }

	// ���콺�� ��ǥ�� ���� �ȿ� �� �ִ���
	bool IsMouseInBoard(Coord2d mouse);

	// ���콺 ��ǥ�� ���� ��ǥ�� ����
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