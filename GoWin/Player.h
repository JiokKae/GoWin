#pragma once
#include "framework.h"

class Player {
	wstring m_name;	// �̸�
	wstring m_kyu;	// �޼�
	int m_captured_stone;	// ���� ��

public:

	Player() { m_name = _T("player"); m_kyu = _T("18��"); m_captured_stone = 0; }
	~Player() { }

	bool init();

	void set_name(wstring name) { m_name = name; }
	void set_kyu(wstring kyu) { m_kyu = kyu; }
	void set_captured_stone(int captured_stone) { m_captured_stone = captured_stone; }
	void add_captured_stone(int num) { m_captured_stone += num; }

	wstring name() const { return m_name; }
	wstring kyu() const { return m_kyu; }
	int captured_stone() const { return m_captured_stone; }

	wstring to_wstring() {
		return _T("�̸� : ") + m_name + _T(" �� : ") + m_kyu + _T(" ���� �� : ") + std::to_wstring(m_captured_stone);
	}

	// �ܼ� ���
	void c_print(); 
	wstring to_ngf() {
		wstring name = _T("           ");
		name.replace(0, m_name.length(), m_name);

		return name + m_kyu;
	}
};
