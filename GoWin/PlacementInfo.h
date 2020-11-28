#pragma once
#include "framework.h"

class PlacementInfo
{
	int sequence;
	Color m_player;
	Coord2d placment;

	void print();
	wstring to_wstr_sgf();
	wstring to_wstr_ngf();

};