#pragma once

#include "framework.h"
#include "Stone/Stone.h"
#include "stdgo.h"

class Board {
public:
	void init(int size);				// OverLoad
	void setHandicap(int num);
	int setBoard(int x, int y, int sequence, Color color);
	void setBoardtmp(int x, int y, int sequence);
	const Stone& getStone(int x, int y) const;
	Stone& getStone(int x, int y);
	Stone& getAstone(int x, int y, int i);		Stone& getAstone(Stone s, int i);
	bool isBoardin(int x, int y) const;
	bool isEmpty(int x, int y);
	bool isIllegalpoint(int x, int y, Color color);
	bool isDeadGS(const Stone* s1);
	bool isSolo(const Stone& s);		bool isSolo( int x, int y );

private:
	int captureGS(Stone* pStone);
	void linkGS(Stone* s1, Stone* s2);

	static const char* direction_char[4];

	std::vector< std::vector<Stone>> board;
};
