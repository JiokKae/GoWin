#pragma once
#include <string>
#include <vector>

class GiboNGF {
public:
	class Player {
	public:
		std::wstring name;
		std::wstring kyu;

		void Set(const std::wstring& ngfPlayerString);
	};

	class Placement {
	public:
		Placement(int sequence, int x, int y, wchar_t color);
		Placement(const std::wstring& ngfString);

		int sequence() const;
		int x() const;
		int y() const;
		wchar_t color() const;

		std::wstring ToString() const;

	private:
		int m_sequence;
		int m_x;
		int m_y;
		wchar_t m_color;

		int StringToSequence(const std::wstring& sequence) const;
		std::wstring SequenceToString(int sequence) const;
	};

private:
	std::wstring m_battle_type;	// 레이팅, 친선
	int m_board_size;	// 바둑판 줄개수
	GiboNGF::Player m_white;
	GiboNGF::Player m_black;
	std::wstring m_link;		// 인터넷 주소
	int m_go_type;		// 0.호선 1.정선 2~9.점 접바둑
	int m_gongje;		// 0.흑공제 else.백공제
	int m_compensation;	// 덤
	std::wstring m_date;		// 날짜 및 시간
	std::wstring m_base_time;	// 기본 시간
	std::wstring m_game_result;	// 게임 결과
	int m_sequence;		// 수
	std::vector<GiboNGF::Placement> m_placements;	// 착수 내역

public:
	GiboNGF(wchar_t* address);
	~GiboNGF(); 

	std::wstring battle_type()	{ return m_battle_type; }
	int board_size()		{ return m_board_size; }
	GiboNGF::Player white()		{ return m_white; }
	GiboNGF::Player black()		{ return m_black; }
	std::wstring link()		{ return m_link; }
	int go_type()			{ return m_go_type; }
	int gongje()			{ return m_gongje; }
	int compensation()		{ return m_compensation; }
	std::wstring date()		{ return m_date; }
	std::wstring base_time()	{ return m_base_time; }
	std::wstring game_result()	{ return m_game_result; }
	int sequence()			{ return m_sequence; }

	const GiboNGF::Placement& getPlacement(int sequence) const;

	bool set_board_size(std::wstring lineNum);
	bool set_go_type(std::wstring goType);
	bool set_gongje(std::wstring gongje);
	bool set_compensation(std::wstring compensation);
	bool set_sequence(std::wstring sequence);

	bool loadGibo(wchar_t* address);
	//bool SaveGibo(char* address);

};
