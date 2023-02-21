#include "GiboSGF.h"
#include <format>

inline TCHAR GiboSGF::Placement::n_to_ch(int n)
{
	return (n + 19) % 20 + 'a';
}

GiboSGF::Placement::operator std::wstring() const
{
	return std::format(_T(";{}[{}{}]"), color, n_to_ch(x), n_to_ch(y));
}

std::wostream& operator<<(std::wostream& wos, const GiboSGF::Placement& placement)
{
	wos << std::wstring(placement);
	return wos;
}

std::wostream& operator<<(std::wostream& wos, const GiboSGF& sgf)
{
	wos << "(";
	wos << ";AP[" << sgf.application << "]";
	wos << "SZ[" << sgf.size << "]";
	wos << "GN[" << sgf.game_name << "]";
	wos << "DT[" << sgf.date << "]";
	wos << "PB[" << sgf.black_name << "]";
	wos << "BR[" << sgf.black_rank << "]";
	wos << "PW[" << sgf.white_name << "]";
	wos << "WR[" << sgf.white_rank << "]";
	wos << "KM[" << sgf.komi << "]";
	wos << "HA[" << sgf.handicap << "]";
	wos << "RE[" << sgf.result << "]";
	wos << "US[" << sgf.user << "]";
	wos << std::endl;
	for (int i = 0; i < sgf.placements.size(); i++)
	{
		wos << sgf.placements[i];
		if (i % 14 == 13) wos << std::endl;
	}
	wos << ")";

	return wos;
}
