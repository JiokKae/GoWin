#pragma once
#include <Windows.h>
#include <string>

#define KEY_ENTER	0x0D
#define KEY_LEFT	0x25
#define KEY_RIGHT	0x27

// ������ �޼����� ���ڿ��� �ٲ��ִ� �Լ�
std::string ReadMessage(UINT message);
