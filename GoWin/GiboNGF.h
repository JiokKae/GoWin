#pragma once
#include <string>
#include <fstream>
#include "stdgo.h"
using namespace std;

static wstring int2str_ngf(int index)
{
	wchar_t first = ++index / 26 + 65;
	wchar_t second = index % 26 + 65;

	return wstring({ first,second });
}

static wstring data2str_ngf(PlacementInfo data)
{
	wchar_t player = Stone::Color2Char(data.player);
	wchar_t x = data.placment.x + 65;
	wchar_t y = data.placment.y + 65;

	return wstring({ player, x, y, y, x });
}

class Player;
class GiboNGF {
	wstring m_battle_type;	// 레이팅, 친선
	int m_board_size;	// 바둑판 줄개수
	Player* m_white;
	Player* m_black;
	wstring m_link;		// 인터넷 주소
	int m_go_type;		// 0.호선 1.정선 2~9.점 접바둑
	int m_gongje;		// 0.흑공제 else.백공제
	int m_compensation;	// 덤
	wstring m_date;		// 날짜 및 시간
	wstring m_base_time;	// 기본 시간
	wstring m_game_result;	// 게임 결과
	int m_sequence;		// 수
	wstring* m_placement;	// 착수 내역

public:
	GiboNGF(wchar_t* address) { loadGibo(address); };
	~GiboNGF() { delete[] m_placement; };

	wstring battle_type()	{ return m_battle_type; }
	int board_size()	{ return m_board_size; }
	Player* white()		{ return m_white; }
	Player* black()		{ return m_black; }
	wstring link()		{ return m_link; }
	int go_type()		{ return m_go_type; }
	int gongje()		{ return m_gongje; }
	int compensation()	{ return m_compensation; }
	wstring date()		{ return m_date; }
	wstring base_time()	{ return m_base_time; }
	wstring game_result()	{ return m_game_result; }
	int sequence()		{ return m_sequence; }

	Coord2d getPlacement(int sequence);

	bool set_board_size(wstring lineNum);
	bool set_go_type(wstring goType);
	bool set_gongje(wstring gongje);
	bool set_compensation(wstring compensation);
	bool set_sequence(wstring sequence);

	bool loadGibo(wchar_t* address);
	//bool SaveGibo(char* address);

};
