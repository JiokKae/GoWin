#include "GIboNGF.h"
#include <fstream>
#include "Mystring.h"

GiboNGF::GiboNGF(wchar_t* address)
{
	loadGibo(address);
};

GiboNGF::~GiboNGF()
{
};

const GiboNGF::Placement& GiboNGF::getPlacement(int sequence) const
{
	if (sequence < 1)
		throw std::out_of_range("sequence is less than 1");
	return m_placements[sequence - 1];
}

bool GiboNGF::set_board_size(wstring lineNum) {
	if (!isWstoi(lineNum))
		return false;
	printf("읽은 문자열 : %s\n", WCharToChar(lineNum.c_str()));
	m_board_size = stoi(lineNum);
	return true;
}

bool GiboNGF::set_go_type(wstring goType) {
	if (!isWstoi(goType))
		return false;
	m_go_type = stoi(goType);
	return true;
}

bool GiboNGF::set_gongje(wstring gongje) {
	if (!isWstoi(gongje))
		return false;
	m_gongje = stoi(gongje);
	return true;
}

bool GiboNGF::set_compensation(wstring compensation) {
	if (!isWstoi(compensation))
		return false;
	m_compensation = stoi(compensation);
	return true;
}

bool GiboNGF::set_sequence(wstring sequence) {
	if (!isWstoi(sequence))
		return false;
	m_sequence = stoi(sequence);
	return true;
}

bool GiboNGF::loadGibo(wchar_t* address) {
	wifstream gibofile(address);
	std::locale loc("ko-kr");
	gibofile.imbue(loc);
	if (!gibofile)
		return false;
	wstring buffer;
	getline(gibofile, m_battle_type);

	getline(gibofile, buffer);
	if (!set_board_size(buffer)) return false;

	getline(gibofile, buffer);
	m_white.Set(buffer);

	getline(gibofile, buffer);
	m_black.Set(buffer);

	getline(gibofile, m_link);

	getline(gibofile, buffer);
	if (!set_go_type(buffer)) return false;

	getline(gibofile, buffer);
	if (!set_gongje(buffer)) return false;

	getline(gibofile, buffer);
	if (!set_compensation(buffer)) return false;

	getline(gibofile, m_date);

	getline(gibofile, m_base_time);

	getline(gibofile, m_game_result);

	getline(gibofile, buffer);
	if (!set_sequence(buffer))
		return false;

	if (m_placements.empty() == false)
	{
		m_placements.clear();
	}

	m_placements.reserve(m_sequence);
	for (int i = 0; i < m_sequence; i++)
	{
		getline(gibofile, buffer);
		m_placements.emplace_back(buffer);
	}

	gibofile.close();

	return true;
}

void GiboNGF::Player::Set(const wstring& ngfPlayerString)
{
	this->name = ngfPlayerString.substr(0, ngfPlayerString.find_first_of(L' '));
	this->kyu = ngfPlayerString.substr(ngfPlayerString.find_last_of(L' '));
}

GiboNGF::Placement::Placement(int sequence, int x, int y, wchar_t color)
	: m_sequence(sequence)
	, m_x(x)
	, m_y(y)
	, m_color(color)
{
}

GiboNGF::Placement::Placement(const std::wstring& ngfString)
	: m_sequence(StringToSequence(ngfString.substr(2, 2)))
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

int GiboNGF::Placement::StringToSequence(const std::wstring& sequenceString) const
{
	return (sequenceString[0] - 'A') * 26 + (sequenceString[1] - 'A');
}

std::wstring GiboNGF::Placement::SequenceToString(int sequence) const
{
	wchar_t first = sequence / 26 + 'A';
	wchar_t second = sequence % 26 + 'A';
	return std::wstring({ first, second });
}

std::wstring GiboNGF::Placement::ToString() const
{
	wchar_t x = this->m_x + 'A';
	wchar_t y = this->m_y + 'A';
	return std::wstring( _T("PM") + SequenceToString(m_sequence) + m_color + x + y + y + x );
}
