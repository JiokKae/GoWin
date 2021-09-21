#include "Player.h"

bool Player::init()
{
	m_captured_stone = 0;
	return true;
}

void Player::c_print()
{
	cout << "[플레이어 정보]---------" << endl;
	wcout << "이름 : " << m_name << endl;
	wcout << "급 : " << m_kyu << endl;
	cout << "따낸 돌 : " << m_captured_stone << endl;
	cout << "------------------------" << endl;
}
