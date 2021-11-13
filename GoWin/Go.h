#pragma once

#include "framework.h"
#include "GiboNGF.h"
#include "mydatastructure.h"

#define ERR_NOTBOARDIN		1
#define ERR_NOTEMPTY		2
#define ERR_ILLEGALPOINT	3 
#define ERR_KO			4

class Board;
class Player;
class Stone;
class Go 
{
public:
	class Information 
	{
	public:
		Information(); 

		bool Init();
		void Release();

		// getter
		Player* white_player()		{ return m_white_player; }
		Player* black_player()		{ return m_black_player; }
		std::wstring game_type()	{ return m_game_type; }
		int board_size()		{ return m_board_size; }
		std::wstring link()		{ return m_link; }
		int go_type()			{ return m_go_type; }
		int gongje()			{ return m_gongje; }
		int compensation()		{ return m_compensation; }
		std::wstring date()		{ return m_date; }
		std::wstring base_time()	{ return m_base_time; }
		std::wstring game_result()	{ return m_game_result; }
		int sequence()			{ return m_sequence; }

		LinkedList placement() const	{ return m_placement; }

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
		void delete_placement() { m_placement.delete_back(); }
		void clear_placement(); 

	private:
		Player* m_white_player;	// 백 플레이어
		Player* m_black_player;	// 흑 플레이어
		LinkedList m_placement;	// 착수
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
	bool Backsies();					// 무르기
	bool Handicap(int num);					// 핸디캡 적용
	bool Init();						// 초기화
	bool Pass();						// 한수쉼
	int Placement( Coord2d board, Color color );		// 착수

	bool Load( GiboNGF& gibo );				// 기보 불러오기
	bool Save( LPWSTR address, std::wstring extension );	// 기보 저장하기

private:
	std::vector< Board* > boardLog;	// 보드의 상태를 저장하는 벡터
	std::vector< Board* > giboLog;
	Board* m_board;			// 보드
	Information m_info;		// 바둑의 정보
	std::string m_mode;		// 모드 : Single, Gibo
};
