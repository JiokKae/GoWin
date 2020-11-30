#include "PlacementInfo.h"
#include "Stone.h"

void PlacementInfo::print()
{
	cout << "[���� ����]-----" << endl;
	cout << "�� : " << m_sequence << endl;
	cout << "�÷��̾� : " << Stone::color2char(m_player) << endl;
	cout << "��ǥ : (" << m_coord.x << ", " << m_coord.y << ")" << endl;
	cout << "----------------" << endl;
}

wstring PlacementInfo::to_wstr_sgf()
{
	wchar_t player = Stone::color2char(m_player);
	wchar_t x = (m_coord.x + 19) % 20 + 97;
	wchar_t y = (m_coord.y + 19) % 20 + 97;

	return wstring({ ';', player, '[', x, y, ']' });
}

wstring PlacementInfo::to_wstr_ngf()
{
	wchar_t player = Stone::color2char(m_player);
	wchar_t x = m_coord.x + 65;
	wchar_t y = m_coord.y + 65;

	return wstring({ player, x, y, y, x });
}