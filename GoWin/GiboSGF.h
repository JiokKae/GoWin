#pragma once
#include <ostream>
#include <vector>
#include <tchar.h>

struct GiboSGF {
	struct Placement {
		int x;
		int y;
		TCHAR color;

		inline static TCHAR n_to_ch(int n);
		operator std::wstring() const;
		friend std::wostream& operator<<(std::wostream&, const GiboSGF::Placement&);
	};

	std::vector<GiboSGF::Placement> placements;
	std::wstring application;
	std::wstring game_name;
	std::wstring date;
	std::wstring black_name;
	std::wstring black_rank;
	std::wstring white_name;
	std::wstring white_rank;
	std::wstring komi;
	std::wstring result;
	std::wstring user;
	int size;
	int handicap;

	friend std::wostream& operator<<(std::wostream&, const GiboSGF&);
};
