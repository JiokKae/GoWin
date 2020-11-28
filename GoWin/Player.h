#pragma once
#include "framework.h"

class Player {
	wstring m_name;	// 이름
	wstring m_kyu;	// 급수
	int m_captured_stone;	// 따낸 돌

public:

	Player() { m_name = _T("player"); m_kyu = _T("18급"); m_captured_stone = 0; }
	~Player() { }

	LRESULT init();
	void release();

	void set_name(wstring name) { m_name = name; }
	void set_kyu(wstring kyu) { m_kyu = kyu; }
	void set_captured_stone(int captured_stone) { m_captured_stone = captured_stone; }
	void add_captured_stone(int num) { m_captured_stone += num; }

	wstring name() const { return m_name; }
	wstring kyu() const { return m_kyu; }
	int captured_stone() const { return m_captured_stone; }

	wstring to_wstring() {
		return _T("이름 : ") + m_name + _T(" 급 : ") + m_kyu + _T(" 따낸 돌 : ") + std::to_wstring(m_captured_stone);
	}
	void print() {
		cout << "[플레이어 정보]---------" << endl;
		wcout << "이름 : " << m_name << endl;
		wcout << "급 : " << m_kyu << endl;
		cout << "따낸 돌 : " << m_captured_stone << endl;
		cout << "------------------------" << endl;
	}

	bool setPlayer(wstring name_kyu) { m_name = name_kyu.substr(0, 11); m_kyu = name_kyu.substr(11, 5); return true; }
	wstring to_ngf() {
		wstring name = _T("           ");
		name.replace(0, m_name.length(), m_name);

		return name + m_kyu;
	}
};

