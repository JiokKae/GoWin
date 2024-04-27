#pragma once

#include "GiboNGF.h"
#include "mydatastructure.h"
#include "Player/player.h"
#include "Board.h"
#include "stdgo.h"
#include <map>
#include <vector>

class Stone;

class Go 
{
public:
	enum class PlacementError
	{
		NONE,
		NOT_BOARD_IN,
		NOT_EMPTY,
		ILLEGAL_POINT,
		KO,
	};

	class Information 
	{
	public:
		Information(); 

		void init();

		const Player& get_player(Color color) const;

		void add_captured_stone(Color color, int captured_stone);
		void add_placement(PlacementInfo&&);
		void delete_placement();

	public:
		std::map<Color, Player> m_players;
		std::vector<Node<PlacementInfo>> m_placements;	// 착수
		std::wstring m_game_type;	// 레이팅, 친선
		std::wstring m_link;		// 인터넷 주소
		std::wstring m_date;		// 날짜 및 시간
		std::wstring m_base_time;	// 기본 시간
		std::wstring m_game_result;	// 게임 결과
		int m_board_size;		// 바둑판 크기(9, 13, 19 ...)
		int m_go_type;			// 0: 호선, 1: 정선, 2 ~ 9: N점 접바둑
		int m_gongje;			// 0: 흑 공제, N: 백 공제
		int m_compensation;		// 덤
		int m_sequence;			// 수
	};

public:
	Go();

	const Information& info() const;
	const Stone& read_coord(Coord2d coord) const;
	const PlacementInfo& getLastPlacementInfo() const;
	Color current_placement_order() const;

	bool backsies();						// 무르기
	bool handicap(int num);						// 핸디캡 적용
	bool init();							// 초기화
	bool pass();							// 한수쉼
	PlacementError placement( Coord2d board );			// 착수

	bool load(const GiboNGF&);					// 기보 불러오기
	bool save(const std::wstring& directory, const std::wstring& gibo_extension);	// 기보 저장하기

	void set_placement_order_previous();
	void set_placement_order_next();

private:
	std::vector<Board> m_board_log;	// 보드 기록을 저장
	
	Board m_board;
	Information m_info;
	
	std::vector<Color> m_placement_orders;
	size_t m_current_placement_order_index;
};
