#pragma once
#include "framework.h"

class Stone;
class BoardManager {
	Stone* board[21][21];

public:
	HRESULT init();
	void release();
														// OverLoad
	void setHandicap(int num);
	int setBoard(int x, int y, int sequence);
	void setBoardtmp(int x, int y, int sequence);
	Color getStoneColor(int x, int y);
	Stone* getStone(int x, int y);
	Stone* getAstone(Stone* s, int i);					Stone* getAstone(int x, int y, int i);
	void linkGS(Stone* s1, Stone* s2);
	int captureGS(Stone* pStone);
	bool isBoardin(int x, int y);						bool isBoardin(Stone s);
	bool isEmpty(int x, int y);							bool isEmpty(Stone* s);
	bool isIllegalpoint(int x, int y, int sqnce);
	bool isDeadGS(Stone* s1);
	bool isSolo(int x, int y);							bool isSolo(Stone* s);

	const char* direction_char[4] = { "[аб]", "[©Л]", "[╩С]", "[го]" };
};
