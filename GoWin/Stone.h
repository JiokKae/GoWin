#pragma once
#include <iostream>
#include <string>
using namespace std;

enum class Color {
	Null, 
	Black,
	White,
	Wall,
	Temp,
};

class Stone {
	int m_x;
	int m_y;
	int m_sequence;
	Stone* m_backStone;
	Stone* m_nextStone;
	bool m_killer;
	Color m_color;

public:
	Stone(int x, int y, int num);
	Stone(int x, int y, int sequence, string temp);
	Stone()				{ m_x = 1, m_y = 1, m_sequence = 0; m_color = Color::Null; m_backStone = nullptr; m_nextStone = nullptr; m_killer = false; }	// 빈 돌
	Stone(string Wall)	{ m_x = 0, m_y = 0, m_sequence = 0; m_color = Color::Wall; m_backStone = nullptr; m_nextStone = nullptr; m_killer = false; }	// 벽을 만들때 사용

	int x() const { return m_x; }
	int y() const { return m_y; }
	Stone* getRef() { return this; }
	Color color() const { return m_color; }
	int sequence() const { return m_sequence; }

	void set_backStone(Stone *s) { m_backStone = s; }
	void set_nextStone(Stone *s) { m_nextStone = s; }
	Stone* backStone() { return m_backStone; }
	Stone* nextStone() { return m_nextStone; }
	void set_killer(bool killer) { m_killer = killer; }
	bool is_killer() { return m_killer; }

	static Color Sqnce2color(int sequence)	{ return (sequence % 2) ? Color::Black : Color::White; }
	static Color Reverse(Color color) { 
		if (color == Color::White)
			return Color::Black;
		else if (color == Color::Black)
			return Color::White;
		else
			return color;
	}
	static char Color2Char(Color color) { return (color == Color::White) ? 'W' : 'B'; }
};