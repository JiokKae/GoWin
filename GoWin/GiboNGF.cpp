#include "GIboNGF.h"
#include <fstream>
#include <format>
#include <tchar.h>

const std::wstring& GiboNGF::Player::name() const
{
	return m_name;
}

const std::wstring& GiboNGF::Player::kyu() const
{
	return m_kyu;
}

void GiboNGF::Player::set(const std::wstring& ngfPlayerString)
{
	m_name = ngfPlayerString.substr(0, ngfPlayerString.find_first_of(L' '));
	m_kyu = ngfPlayerString.substr(ngfPlayerString.find_last_of(L' '));
}

void GiboNGF::Player::set(const std::wstring& name, const std::wstring& kyu)
{
	m_name = name;
	m_kyu = kyu;
}

std::wstring GiboNGF::Player::to_string() const
{
	return std::format(_T("{: <12}{}"), m_name, m_kyu);
}

GiboNGF::Placement::Placement(int sequence, int x, int y, wchar_t color)
	: m_sequence(sequence)
	, m_x(x)
	, m_y(y)
	, m_color(color)
{
}

GiboNGF::Placement::Placement(const std::wstring& ngfString)
	: m_sequence(string_to_sequence(ngfString.substr(2, 2)))
	, m_x(ngfString[5] - _T('A'))
	, m_y(ngfString[6] - _T('A'))
	, m_color(ngfString[4])
{
}

int GiboNGF::Placement::sequence() const
{
	return m_sequence;
}

int GiboNGF::Placement::x() const
{
	return m_x;
}

int GiboNGF::Placement::y() const
{
	return m_y;
}

wchar_t GiboNGF::Placement::color() const
{
	return m_color;
}

std::wstring GiboNGF::Placement::to_string() const
{
	wchar_t x = m_x + 'A';
	wchar_t y = m_y + 'A';
	return std::wstring(_T("PM") + sequence_to_string(m_sequence) + m_color + x + y + y + x);
}

int GiboNGF::Placement::string_to_sequence(const std::wstring& sequenceString) const
{
	return (sequenceString[0] - 'A') * 26 + (sequenceString[1] - 'A');
}

std::wstring GiboNGF::Placement::sequence_to_string(int sequence) const
{
	wchar_t first = sequence / 26 + 'A';
	wchar_t second = sequence % 26 + 'A';
	return std::wstring({ first, second });
}

GiboNGF::GiboNGF()
	: m_board_size(19)
	, m_compensation(6)
	, m_go_type(0)
	, m_gongje(0)
{
}

GiboNGF::~GiboNGF()
{
}

const std::vector<GiboNGF::Placement>& GiboNGF::getPlacements() const
{
	return m_placements;
}

const GiboNGF::Player& GiboNGF::white() const
{
	return m_white;
}

const GiboNGF::Player& GiboNGF::black() const
{
	return m_black;
}

const std::wstring& GiboNGF::url() const
{
	return m_url;
}

const std::wstring& GiboNGF::date() const
{
	return m_date;
}

const std::wstring& GiboNGF::base_time() const
{
	return m_base_time;
}

const std::wstring& GiboNGF::battle_type() const
{
	return m_battle_type;
}

const std::wstring& GiboNGF::game_result() const
{
	return m_game_result;
}

int GiboNGF::gongje() const
{
	return m_gongje;
}

int GiboNGF::go_type() const
{
	return m_go_type;
}

int GiboNGF::sequence() const
{
	return m_placements.size();
}

int GiboNGF::board_size() const
{
	return m_board_size;
}

int GiboNGF::compensation() const
{
	return m_compensation;
}

void GiboNGF::add_placement(GiboNGF::Placement&& placement)
{
	m_placements.emplace_back(placement);
}

void GiboNGF::set_white_player(const std::wstring& name, const std::wstring& kyu)
{
	m_white.set(name, kyu);
}

void GiboNGF::set_black_player(const std::wstring& name, const std::wstring& kyu)
{
	m_black.set(name, kyu);
}

void GiboNGF::set_url(const std::wstring& url)
{
	m_url = url;
}

void GiboNGF::set_date(const std::wstring& date)
{
	m_date = date;
}

void GiboNGF::set_base_time(const std::wstring& base_time)
{
	m_base_time = base_time;
}

void GiboNGF::set_game_result(const std::wstring& game_result)
{
	m_game_result = game_result;
}

void GiboNGF::set_battle_type(const std::wstring& battle_type)
{
	m_battle_type = battle_type;
}

void GiboNGF::set_gongje(int gongje)
{
	m_gongje = gongje;
}

void GiboNGF::set_go_type(int go_type)
{
	m_go_type = go_type;
}

void GiboNGF::set_board_size(int board_size) 
{
	m_board_size = board_size;
}

void GiboNGF::set_compensation(int compensation)
{
	m_compensation = compensation;
}

void GiboNGF::load(std::wistream& wis)
{
	std::wstring buffer;
	getline(wis, m_battle_type);

	getline(wis, buffer);
	set_board_size(std::stoi(buffer));

	getline(wis, buffer);
	m_white.set(buffer);

	getline(wis, buffer);
	m_black.set(buffer);

	getline(wis, m_url);

	getline(wis, buffer);
	set_go_type(std::stoi(buffer));

	getline(wis, buffer);
	set_gongje(std::stoi(buffer));

	getline(wis, buffer);
	set_compensation(std::stoi(buffer));

	getline(wis, m_date);

	getline(wis, m_base_time);

	getline(wis, m_game_result);

	getline(wis, buffer);
	int sequence = std::stoi(buffer);

	if (m_placements.empty() == false)
	{
		m_placements.clear();
	}

	m_placements.reserve(sequence);
	for (int i = 0; i < sequence; i++)
	{
		getline(wis, buffer);
		m_placements.emplace_back(buffer);
	}
}

void GiboNGF::save(std::wostream& os)
{
	os << battle_type() << std::endl;
	os << board_size() << std::endl;
	os << white().to_string() << std::endl;
	os << black().to_string() << std::endl;
	os << url() << std::endl;
	os << go_type() << std::endl;
	os << gongje() << std::endl;
	os << compensation() << std::endl;
	os << date() << std::endl;
	os << base_time() << std::endl;
	os << game_result() << std::endl;
	os << sequence() << std::endl;
	for (const auto& placement : getPlacements())
	{
		os << placement.to_string() << std::endl;
	}
}
