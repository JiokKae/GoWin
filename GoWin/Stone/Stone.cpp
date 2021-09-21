#include "Stone.h"

Stone::Stone(int x, int y, int sequence) {
	m_x = x;
	m_y = y;
	m_sequence = sequence;
	m_color = Sqnce2color(sequence);
	m_backStone = nullptr;
	m_nextStone = nullptr;
	m_killer = false;
}
Stone::Stone(int x, int y, int sequence, string temp) {
	m_x = x;
	m_y = y;
	m_sequence = sequence;
	m_color = Color::Temp;
	m_backStone = nullptr;
	m_nextStone = nullptr;
	m_killer = false;
}