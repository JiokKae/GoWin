#include "GIboNGF.h"
#include "Player.h"
#include "Mystring.h"
#include <fstream>

HRESULT GiboNGF::init(LPTSTR address)
{
	if (m_white == nullptr)
		m_white = new Player();
	m_white->init();

	if (m_black == nullptr)
		m_black = new Player();
	m_black->init();

	loadGibo(address);

	return S_OK;
}

void GiboNGF::release()
{
	SAFE_RELEASE(m_white);
	SAFE_RELEASE(m_black);

	delete[] m_placement;
}

Coord2d GiboNGF::get_placement(int sequence)
{
	Coord2d placement;
	wstring placement_str;

	placement_str = m_placement[sequence].substr(5, 2);
	placement = { placement_str[0] - 65, placement_str[1] - 65 };

	return placement;
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

bool GiboNGF::loadGibo(LPTSTR address) {
	wifstream gibofile(address);
	if (!gibofile)
		return false;
	wstring temp;
	getline(gibofile, m_battle_type);

	getline(gibofile, temp);
	if (!set_board_size(temp)) return false;

	getline(gibofile, temp);
	if (!m_white->setPlayer(temp)) return false;

	getline(gibofile, temp);
	if (!m_black->setPlayer(temp)) return false;

	getline(gibofile, m_link);

	getline(gibofile, temp);
	if (!set_go_type(temp)) return false;

	getline(gibofile, temp);
	if (!set_gongje(temp)) return false;

	getline(gibofile, temp);
	if (!set_compensation(temp)) return false;

	getline(gibofile, m_date);

	getline(gibofile, m_base_time);

	getline(gibofile, m_game_result);

	getline(gibofile, temp);
	if (!set_sequence(temp))
		return false;
	if (m_placement != nullptr)
		delete[] m_placement;
	m_placement = new wstring[m_sequence];
	for (int i = 0; i < m_sequence; i++)
	{
		getline(gibofile, m_placement[i]);
		wcout << m_placement[i] << endl;
	}
		
	gibofile.close();

	return true;
}
