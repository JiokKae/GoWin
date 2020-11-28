#include "PlacementInfo.h"
#include "Stone.h"

void PlacementInfo::print()
{
	cout << "[���� ����]-----" << endl;
	cout << "�� : " << sequence << endl;
	cout << "�÷��̾� : " << Stone::color2char(m_player) << endl;
	cout << "��ǥ : (" << placment.x << ", " << placment.y << ")" << endl;
	cout << "----------------" << endl;
}

wstring PlacementInfo::to_wstr_sgf()
{
	wchar_t player = Stone::color2char(m_player);
	wchar_t x = (placment.x + 19) % 20 + 97;
	wchar_t y = (placment.y + 19) % 20 + 97;

	return wstring({ ';', player, '[', x, y, ']' });
}

wstring PlacementInfo::to_wstr_ngf()
{
	wchar_t player = Stone::color2char(m_player);
	wchar_t x = placment.x + 65;
	wchar_t y = placment.y + 65;

	return wstring({ player, x, y, y, x });
}