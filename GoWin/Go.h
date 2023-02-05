#pragma once

#include "framework.h"
#include "GiboNGF.h"
#include "mydatastructure.h"
#include "Player/player.h"
#include "Board.h"

#define ERR_NOTBOARDIN		1
#define ERR_NOTEMPTY		2
#define ERR_ILLEGALPOINT	3 
#define ERR_KO			4

class Stone;
class Go 
{
public:
	class Information 
	{
	public:
		Information(); 

		void Init();

		// getter
		const std::map<Color, Player>& get_players() const;
		const Player& get_player(Color color) const;
		
		const std::wstring& game_type() const;		
		const std::wstring& link() const;
		const std::wstring& date() const;
		const std::wstring& base_time() const;
		const std::wstring& game_result() const;
		int board_size() const;
		int go_type() const;
		int gongje() const;
		int compensation() const;
		int sequence() const;
		const std::vector<Node>& placements() const;

		// setter
		void set_game_type(std::wstring game_type)	{ m_game_type = game_type; }
		void set_board_size(int board_size)		{ m_board_size = board_size; }
		void set_link(std::wstring link)		{ m_link = link; }
		void set_go_type(int go_type)			{ m_go_type = go_type; }
		void set_gongje(int gongje)			{ m_gongje = gongje; }
		void set_compensation(int compensation)		{ m_compensation = compensation; }
		void set_date(std::wstring date)		{ m_date = date; }
		void set_base_time(std::wstring base_time)	{ m_base_time = base_time; }
		void set_game_result(std::wstring game_result)	{ m_game_result = game_result; }
		void set_sequence(int sequence)			{ m_sequence = sequence; }

		void add_sequence(int num)			{ m_sequence += num; }
		void add_captured_stone( Color color, int captured_stone );

		// function
		void add_placement( PlacementInfo placement ); 
		void delete_placement() { m_placements.pop_back(); }

	private:
		std::map<Color, Player> players;

		std::vector<Node> m_placements;	// 착수
		std::wstring m_game_type;	// 레이팅, 친선
		int m_board_size;	// 바둑판 크기(9, 13, 19 ...)
		std::wstring m_link;		// 인터넷 주소
		int m_go_type;		// 0.호선 1.정선 2~9.점 접바둑
		int m_gongje;		// 0.흑공제 else.백공제
		int m_compensation;	// 덤
		std::wstring m_date;		// 날짜 및 시간
		std::wstring m_base_time;	// 기본 시간
		std::wstring m_game_result;	// 게임 결과
		int m_sequence;		// 수
	};

public:
	Go(); 
	~Go();

	// getter
	Information info()	{ return m_info; }	// 정보 
	std::string mode()	{ return m_mode; }	// 모드 반환
	const Stone& ReadCoord( Coord2d coord );	// 읽기
	const PlacementInfo& getLastPlacementInfo() const;

	// function
	bool Backsies();						// 무르기
	bool Handicap(int num);						// 핸디캡 적용
	bool Init();							// 초기화
	bool Pass();							// 한수쉼
	int Placement( Coord2d board );					// 착수

	bool Load( GiboNGF& gibo );					// 기보 불러오기
	bool Save( LPWSTR address, std::wstring giboExtension );	// 기보 저장하기

	Color get_current_placement_order() const;
	void set_placement_order_previous();
	void set_placement_order_next();

private:
	std::vector< Board > boardLog;	// 보드의 상태를 저장하는 벡터
	std::vector< Board > giboLog;
	
	Board m_board;			// 보드
	Information m_info;		// 바둑의 정보
	std::string m_mode;		// 모드 : Single, Gibo
	
	std::vector<Color> placementOrders;
	int currentPlacementOrderIndex;
};
