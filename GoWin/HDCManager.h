#pragma once
#include <Windows.h>
#include <array>

class HDCManager {
	HDC *hdcs;

public:
	HDCManager(int num);
	~HDCManager();
};