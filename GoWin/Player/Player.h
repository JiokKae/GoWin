#pragma once
#include "../framework.h"

enum class Color;
class Player 
{
public:
	Player( Color color );
	~Player() {}

	bool init( Color color );

	void set_name( const std::wstring& name )	{ m_name = name; }
	void set_kyu( const std::wstring& kyu )		{ m_kyu = kyu; }
	void set_captured_stone( int captured_stone )	{ m_captured_stone = captured_stone; }
	void add_captured_stone( int num )		{ m_captured_stone += num; }

	std::wstring name() const	{ return m_name; }
	std::wstring kyu() const	{ return m_kyu; }
	Color color() const { return m_color; }
	int captured_stone() const	{ return m_captured_stone; }

	std::wstring to_wstring() const;
	void print( std::wostream& wos) const;
	
private:
	std::wstring m_name;
	std::wstring m_kyu;	// 급수
	Color m_color;
	int m_captured_stone;	// 따낸 돌
};
