#pragma once
#include <string>
#include <vector>

class GiboNGF {
public:
	class Player {
	public:
		const std::wstring& name() const;
		const std::wstring& kyu() const;

		void Set(const std::wstring& ngfPlayerString);

	private:
		std::wstring m_name;
		std::wstring m_kyu;
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

public:
	GiboNGF(wchar_t* address);
	~GiboNGF(); 

	const std::vector<GiboNGF::Placement>& getPlacements() const;
	const GiboNGF::Player& white()	const; 
	const GiboNGF::Player& black()	const; 
	const std::wstring& link() const;
	const std::wstring& date() const;
	const std::wstring& base_time()	const; 
	const std::wstring& battle_type() const;
	const std::wstring& game_result() const;
	int gongje() const; 
	int go_type() const; 
	int sequence() const;
	int board_size() const;
	int compensation() const;

	bool set_gongje(const std::wstring& gongje);
	bool set_go_type(const std::wstring& goType);
	bool set_sequence(const std::wstring& sequence);
	bool set_board_size(const std::wstring& lineNum);
	bool set_compensation(const std::wstring& compensation);

	bool loadGibo(wchar_t* address);
	//bool SaveGibo(char* address);

private:
	std::vector<GiboNGF::Placement> m_placements;	// 착수 내역
	GiboNGF::Player m_white;
	GiboNGF::Player m_black;
	std::wstring m_battle_type;	// 레이팅, 친선
	std::wstring m_link;		// 인터넷 주소
	std::wstring m_date;		// 날짜 및 시간
	std::wstring m_base_time;	// 기본 시간
	std::wstring m_game_result;	// 게임 결과
	int m_board_size;		// 바둑판 줄개수
	int m_go_type;			// 0.호선 1.정선 2~9.점 접바둑
	int m_gongje;			// 0.흑공제 else.백공제
	int m_compensation;		// 덤
	int m_sequence;			// 수
};
