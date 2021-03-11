#pragma once
#include <string>
#include <fstream>
#include "stdgo.h"
using namespace std;

class GiboNGF {
	wstring m_battle_type;	// ������, ģ��
	int m_board_size;	// �ٵ��� �ٰ���
	Player m_white;
	Player m_black;
	wstring m_link;		// ���ͳ� �ּ�
	int m_go_type;		// 0.ȣ�� 1.���� 2~9.�� ���ٵ�
	int m_gongje;		// 0.����� else.�����
	int m_compensation;	// ��
	wstring m_date;		// ��¥ �� �ð�
	wstring m_base_time;	// �⺻ �ð�
	wstring m_game_result;	// ���� ���
	int m_sequence;		// ��
	wstring* m_placement;	// ���� ����

public:
	GiboNGF(wchar_t* address) { loadGibo(address); };
	~GiboNGF() { delete[] m_placement; };

	wstring battle_type()	{ return m_battle_type; }
	int board_size()	{ return m_board_size; }
	Player white()		{ return m_white; }
	Player black()		{ return m_black; }
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
