#pragma once

#include "framework.h"
#include "GiboNGF.h"
#include "mydatastructure.h"
#include "Player/player.h"

#define ERR_NOTBOARDIN		1
#define ERR_NOTEMPTY		2
#define ERR_ILLEGALPOINT	3 
#define ERR_KO			4

class Board;
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
		LinkedList placement() const;

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
		std::map<Color, Player> players;

		LinkedList m_placement;	// ����
		std::wstring m_game_type;	// ������, ģ��
		int m_board_size;	// �ٵ��� ũ��(9, 13, 19 ...)
		std::wstring m_link;		// ���ͳ� �ּ�
		int m_go_type;		// 0.ȣ�� 1.���� 2~9.�� ���ٵ�
		int m_gongje;		// 0.����� else.�����
		int m_compensation;	// ��
		std::wstring m_date;		// ��¥ �� �ð�
		std::wstring m_base_time;	// �⺻ �ð�
		std::wstring m_game_result;	// ���� ���
		int m_sequence;		// ��
	};

public:
	Go(); 
	~Go();

	// getter
	Information info()	{ return m_info; }	// ���� 
	std::string mode()	{ return m_mode; }	// ��� ��ȯ
	const Stone& ReadCoord( Coord2d coord );	// �б�
	const PlacementInfo& getLastPlacementInfo() const;

	// function
	bool Backsies();					// ������
	bool Handicap(int num);					// �ڵ�ĸ ����
	bool Init();						// �ʱ�ȭ
	bool Pass();						// �Ѽ���
	int Placement( Coord2d board, Color color );		// ����

	bool Load( GiboNGF& gibo );				// �⺸ �ҷ�����
	bool Save( LPWSTR address, std::wstring extension );	// �⺸ �����ϱ�

	Color get_current_placement_order() const;
	void set_placement_order_next();

private:
	std::vector< Board* > boardLog;	// ������ ���¸� �����ϴ� ����
	std::vector< Board* > giboLog;
	
	Board* m_board;			// ����
	Information m_info;		// �ٵ��� ����
	std::string m_mode;		// ��� : Single, Gibo
	
	std::vector<Color> placementOrders;
	int currentPlacementOrderIndex;
};
