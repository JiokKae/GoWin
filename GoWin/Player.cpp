#include "Player.h"

bool Player::init()
{
	m_captured_stone = 0;
	return true;
}

void Player::c_print()
{
	cout << "[�÷��̾� ����]---------" << endl;
	wcout << "�̸� : " << m_name << endl;
	wcout << "�� : " << m_kyu << endl;
	cout << "���� �� : " << m_captured_stone << endl;
	cout << "------------------------" << endl;
}
