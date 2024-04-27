#include "Player.h"

#include <tchar.h>

Player::Player()
	: m_name{ _T("player") }
	, m_kyu{ _T("18급") }
	, m_color{ Color::None }
	, m_captured_stone{ 0 } {

}

Player::Player(Color color)
	: m_name{ _T("player") }
	, m_kyu{ _T("18급") }
	, m_color{ color }
	, m_captured_stone{ 0 }
{
}

void Player::set_name(const std::wstring& name)
{
	m_name = name;
}

void Player::set_kyu(const std::wstring& kyu)
{
	m_kyu = kyu;
}

void Player::set_captured_stone(int captured_stone)
{
	m_captured_stone = captured_stone;
}

void Player::add_captured_stone(int num)
{
	m_captured_stone += num;
}

const std::wstring& Player::name() const
{
	return m_name;
}

const std::wstring& Player::kyu() const
{
	return m_kyu;
}

Color Player::color() const
{
	return m_color;
}

int Player::captured_stone() const
{
	return m_captured_stone;
}
