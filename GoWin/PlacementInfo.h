#pragma once
#include "framework.h"

class PlacementInfo
{
	int m_sequence;
	Color m_player;
	Coord2d m_coord;

public:
	void set_sequence(int sequence) { this->m_sequence = sequence; }
	void set_player(Color player)	{ this->m_player = player; }
	void set_coord(Coord2d coord)	{ this->m_coord = coord; }

	int sequence()		{ return m_sequence; }
	Color player()		{ return m_player; }
	Coord2d coord()		{ return m_coord; }

	void print();
	wstring to_wstr_sgf();
	wstring to_wstr_ngf();

};