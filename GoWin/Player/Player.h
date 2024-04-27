#pragma once

#include <string>
#include "../stdgo.h"

class Player {
public:
	Player();
	Player(Color);

	void set_name(const std::wstring&);
	void set_kyu(const std::wstring&);
	void set_captured_stone(int);
	void add_captured_stone(int);

	const std::wstring& name() const;
	const std::wstring& kyu() const;
	Color color() const; 
	int captured_stone() const;

private:
	std::wstring m_name;
	std::wstring m_kyu;
	Color m_color;
	int m_captured_stone;
};
