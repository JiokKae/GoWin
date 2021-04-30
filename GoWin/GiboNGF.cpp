#include "GIboNGF.h"
#include "Player.h"
#include "Mystring.h"

Coord2d GiboNGF::getPlacement(int sequence)
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

bool GiboNGF::loadGibo(wchar_t* address) {
	wifstream gibofile(address);
	if (!gibofile)
		return false;
	wstring buffer;
	getline(gibofile, m_battle_type);

	getline(gibofile, buffer);
	if (!set_board_size(buffer)) return false;

	getline(gibofile, buffer);
	wstring name = buffer.substr(0, 11);
	wstring kyu = buffer.substr(11, 5);
	m_white->set_name(name);
	m_white->set_kyu(kyu);

	getline(gibofile, buffer);
	name = buffer.substr(0, 11);
	kyu = buffer.substr(11, 5);
	m_black->set_name(name);
	m_black->set_kyu(kyu);

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
