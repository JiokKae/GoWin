#pragma once
#include <vector>
#include "Board.h"
#include "stdgo.h"
#include "mydatastructure.h"
#include "GiboNGF.h"
#include <windows.h>

#define ERR_NOTBOARDIN		1
#define ERR_NOTEMPTY		2
#define ERR_ILLEGALPOINT	3 
#define ERR_KO			4

class Go {
public:
	class Information {
		Player m_white_player;	// 백 플레이어
		Player m_black_player;	// 흑 플레이어
		LinkedList m_placement;	// 착수
		wstring m_game_type;	// 레이팅, 친선
		int m_board_size;	// 바둑판 크기(9, 13, 19 ...)
		wstring m_link;		// 인터넷 주소
		int m_go_type;		// 0.호선 1.정선 2~9.점 접바둑
		int m_gongje;		// 0.흑공제 else.백공제
		int m_compensation;	// 덤
		wstring m_date;		// 날짜 및 시간
		wstring m_base_time;	// 기본 시간
		wstring m_game_result;	// 게임 결과
		int m_sequence;		// 수

	public:
		Information() {
			m_game_type = _T("친선대국");
			m_board_size = 19;
			m_compensation = 6;
			m_base_time = _T("0");
			m_game_result = _T("결과 없음");
		}

		// getter
		Player white_player()	{ return m_white_player; }
		Player black_player()	{ return m_black_player; }
		wstring game_type()	{ return m_game_type; }
		int board_size()	{ return m_board_size; }
		wstring link()		{ return m_link; }
		int go_type()		{ return m_go_type; }
		int gongje()		{ return m_gongje; }
		int compensation()	{ return m_compensation; }
		wstring date()		{ return m_date; }
		wstring base_time()	{ return m_base_time; }
		wstring game_result()	{ return m_game_result; }
		int sequence()		{ return m_sequence; }

		LinkedList placement() { return m_placement; }

		// setter
		void set_white_player(Player white_player)	{ m_white_player = white_player; }
		void set_black_player(Player black_player)	{ m_black_player = black_player; }
		void set_game_type(wstring game_type)		{ m_game_type = game_type; }
		void set_board_size(int board_size)		{ m_board_size = board_size; }
		void set_link(wstring link)			{ m_link = link; }
		void set_go_type(int go_type)			{ m_go_type = go_type; }
		void set_gongje(int gongje)			{ m_gongje = gongje; }
		void set_compensation(int compensation)		{ m_compensation = compensation; }
		void set_date(wstring date)			{ m_date = date; }
		void set_base_time(wstring base_time)		{ m_base_time = base_time; }
		void set_game_result(wstring game_result)	{ m_game_result = game_result; }
		void set_sequence(int sequence)			{ m_sequence = sequence; }

		void add_sequence(int num)			{ m_sequence += num; }
		void add_captured_stone(Color color, int captured_stone) {
			if (color == Color::Black)
				m_black_player.add_captured_stone(captured_stone);
			else if (color == Color::White)
				m_white_player.add_captured_stone(captured_stone);
		}

		// function
		void add_placement(PlacementInfo placement) { m_placement.push_back(new Node(placement)); }
		void delete_placement() { m_placement.delete_back(); }
		void clear_placement() {
			while (m_placement.size() != 0)
			{
				m_placement.delete_back();
			}

		}
	};

private:
	vector<Board> boardLog;	// 보드의 상태를 저장하는 벡터
	vector<Board> giboLog;
	Board m_board;		// 보드
	Information m_info;		// 바둑의 정보
	string m_mode;			// 모드 : Single, Gibo

public:
	Go() { 
		m_info.set_sequence(1);
		m_board.init();
		m_mode = "Single";
	}
	~Go() {}

	// getter
	Information info()	{ return m_info; }	// 정보 
	string mode()		{ return m_mode; }	// 모드 반환
	Stone Read(Coord2d coord_read);			// 읽기
	PlacementInfo getLastPlacementInfo() { return m_info.placement().getLastNode().data(); }

	// function
	bool Backsies();		// 무르기
	bool Handicap(int num);		// 핸디캡 적용
	bool Init();			// 초기화
	bool Pass();			// 한수쉼
	int Placement(Coord2d board);	// 착수

	bool Load(GiboNGF& gibo);			// 기보 불러오기
	bool Save(LPWSTR address, wstring extension);	// 기보 저장하기
};
