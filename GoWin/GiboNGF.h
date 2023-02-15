#pragma once
#include <string>
#include <vector>

class GiboNGF {
public:
	class Player {
	public:
		const std::wstring& name() const;
		const std::wstring& kyu() const;

		void set(const std::wstring& ngfPlayerString);
		void set(const std::wstring& name, const std::wstring& kyu);
		std::wstring to_string() const;

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

		std::wstring to_string() const;

	private:
		int m_sequence;
		int m_x;
		int m_y;
		wchar_t m_color;

		int string_to_sequence(const std::wstring& sequence) const;
		std::wstring sequence_to_string(int sequence) const;
	};

public:
	GiboNGF();
	GiboNGF(std::wistream&);
	~GiboNGF(); 

	const std::vector<GiboNGF::Placement>& getPlacements() const;
	const GiboNGF::Player& white() const; 
	const GiboNGF::Player& black() const; 
	const std::wstring& url() const;
	const std::wstring& date() const;
	const std::wstring& base_time()	const; 
	const std::wstring& battle_type() const;
	const std::wstring& game_result() const;
	int gongje() const; 
	int go_type() const; 
	int sequence() const;
	int board_size() const;
	int compensation() const;

	void add_placement(GiboNGF::Placement&&);
	void set_white_player(const std::wstring& name, const std::wstring& kyu);
	void set_black_player(const std::wstring& name, const std::wstring& kyu);
	void set_url(const std::wstring&);
	void set_date(const std::wstring&);
	void set_base_time(const std::wstring&);
	void set_game_result(const std::wstring&);
	void set_battle_type(const std::wstring&);
	void set_gongje(int);
	void set_go_type(int);
	void set_board_size(int);
	void set_compensation(int);
	
	void load(std::wistream&);
	void save(std::wostream&);

private:
	std::vector<GiboNGF::Placement> m_placements;	// 착수 내역
	GiboNGF::Player m_white;
	GiboNGF::Player m_black;
	std::wstring m_battle_type;	// 레이팅, 친선
	std::wstring m_url;		// 인터넷 주소
	std::wstring m_date;		// 날짜 및 시간
	std::wstring m_base_time;	// 기본 시간
	std::wstring m_game_result;	// 게임 결과
	int m_board_size;		// 바둑판 줄개수
	int m_go_type;			// 0.호선 1.정선 2~9.점 접바둑
	int m_gongje;			// 0.흑공제 else.백공제
	int m_compensation;		// 덤
};
