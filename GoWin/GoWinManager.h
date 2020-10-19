#pragma once
#include "framework.h"

class GoWinManager
{
	Go game;
	bool printSequanceSwitch;
public:

	Go GetGame();
	bool GetPrintSequenceSwitch();

	void SetPrintSequenceSwitch(bool b);

	void FileSave();

};

