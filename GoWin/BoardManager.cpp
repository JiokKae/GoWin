#include "BoardManager.h" 
#define LEFT	0
#define RIGHT	1
#define UP		2
#define DOWN	3

void BoardManager::init() {
	for (int x = 0; x < 21; x++)
	{
		for (int y = 0; y < 21; y++)
		{
			if (x == 0 || x == 20 || y == 0 || y == 20)
				board[x][y] = Stone("Wall");
			else
				board[x][y] = Stone();
		}
	}
}

void BoardManager::setHandicap(int num) {
	if (num == 8) {
		board[16][10] = Stone(16, 10, 1);
		board[4][10] = Stone(4, 10, 1);
		num = 6;
	}
	if (num == 6) {
		board[10][16] = Stone(10, 16, 1);
		board[10][4] = Stone(10, 4, 1);
		num = 4;
	}
	switch (num)
	{
	case 9:
		board[16][10] = Stone(16, 10, 1);
		board[4][10] = Stone(4, 10, 1);
	case 7:
		board[10][16] = Stone(10, 16, 1);
		board[10][4] = Stone(10, 4, 1);
	case 5:
		board[10][10] = Stone(10, 10, 1);
	case 4:
		board[16][16] = Stone(16, 16, 1);
	case 3:
		board[4][4] = Stone(4, 4, 1);
	case 2:
		board[4][16] = Stone(4, 16, 1);
		board[16][4] = Stone(16, 4, 1);
	}
}

//--------------------------------------------------------------------------------------
// Name:  setBoard
// Desc:  보드에 돌을 설정한다.
// Param: x			-> 돌의 x 좌표
//        y			-> 돌의 y 좌표
//        sequence	-> 돌의 수순
// Ret:   잡아낸 돌의 숫자
//--------------------------------------------------------------------------------------
int BoardManager::setBoard(int x, int y, int sequence) {
	int ret = 0;
	board[x][y] = Stone(x, y, sequence);
	Color color = board[x][y].color();

	for (int i = 0; i < 4; i++)
	{
		if (color == getStoneColor(getAstone(x, y, i)))
		{
			cout << i << " " << direction_char[i] << " : 일치" << endl;
			linkGS(&board[x][y], getAstone(x, y, i).getRef());
		}
		else if (color == Stone::Reverse(getStoneColor(getAstone(x, y, i))))
		{
			cout << i << " " << direction_char[i] << " : 불일치" << endl;
			if (isDeadGS(getAstone(x, y, i).getRef()))
			{
				ret = captureGS(getAstone(x, y, i).getRef());
				board[x][y].set_killer(true);
				cout << i << " " << direction_char[i] << " : 삭제" << endl;
			}
		}
		else if (getStoneColor(getAstone(x, y, i)) == Color::Wall)
			cout << i << " " << direction_char[i] << " : 장외" << endl;
		else
			cout << i << " " << direction_char[i] << " : 비었음" << endl;
	}

	return ret;
}

void BoardManager::setBoardtmp(int x, int y, int sequence) {
	if (isEmpty(board[x][y]))
		board[x][y] = Stone(x, y, sequence, "temp");
	else
		board[x][y] = Stone();
}

Color BoardManager::getStoneColor(int x, int y) {
	return board[x][y].color();
}
Color BoardManager::getStoneColor(Stone s) {
	return s.color();
}

Stone& BoardManager::getStone(int x, int y) {
	return board[x][y];
}

Stone& BoardManager::getAstone(Stone s, int index) {
	if (index == LEFT) 
		return getStone(s.x() - 1, s.y());
	else if(index == RIGHT)
		return getStone(s.x() + 1, s.y());
	else if (index == UP)
		return getStone(s.x(), s.y() - 1);
	else if (index == DOWN)
		return getStone(s.x(), s.y() + 1);
	else
		return getStone(0, 0);
}

Stone& BoardManager::getAstone(int x, int y, int index) {
	if (index == LEFT)
		return getStone(x - 1, y);
	else if (index == RIGHT)
		return getStone(x + 1, y);
	else if (index == UP)
		return getStone(x, y - 1);
	else if (index == DOWN)
		return getStone(x, y + 1);
	else
		return getStone(0, 0);
}

//s1과 s2를 연결
void BoardManager::linkGS(Stone* s1, Stone* s2) {
	Stone* s1last;
	Stone* s2first;

	s1last = s1;

	while (s1last->nextStone() != nullptr)
		s1last = s1last->nextStone();

	s2first = s2;

	while (s2first->backStone() != nullptr)
		s2first = s2first->backStone();

	if (s1 != s2first) 
	{
		s1last->set_nextStone(s2first);
		s2first->set_backStone(s1last);
	}
}

//--------------------------------------------------------------------------------------
// Name:  captureGS
// Desc:  돌을 따낸다.
// Param: capturedStone	-> 따낼 돌의 포인터
// Ret:   잡아낸 돌의 숫자
//--------------------------------------------------------------------------------------
int BoardManager::captureGS(Stone* capturedStone) {
	Stone* pStone = capturedStone;
	Stone* pStoneTemp;
	int count = 0;

	while (pStone->backStone() != nullptr)
		pStone = pStone->backStone();

	pStoneTemp = pStone->nextStone();
	while (pStone != nullptr) 
	{
		*pStone = Stone();
		count++;
		pStone = pStoneTemp;
		if (pStoneTemp != nullptr)
			pStoneTemp = pStoneTemp->nextStone();
	}
	return count;
}

bool BoardManager::isBoardin(int x, int y) {
	return ((x < 20 && x >= 1) && (y < 20 && y >= 1));}
bool BoardManager::isBoardin(Stone s) {
	return ((s.x() < 20 && s.x() >= 1) && (s.y() < 20 && s.y() >= 1));
}

bool BoardManager::isEmpty(int x, int y) {
	return (board[x][y].sequence() == 0);}
bool BoardManager::isEmpty(Stone s) {
	return (s.sequence() == 0);
}

bool BoardManager::isIllegalpoint(int x, int y, int sqnce) {
	Color Stone = Stone::Sqnce2color(sqnce);
	Color AroundStone;

	for (int i = 0; i < 4; i++) //돌 주위에 빈곳이 있다면 착수금지점 아님
	{
		AroundStone = getAstone(x, y, i).color();
		if (AroundStone == Color::Null || AroundStone == Stone)
			return false;
	}
	return true;
}

bool BoardManager::isDeadGS(Stone* s1) {
	Stone* sp = s1;
	Stone* sptmp;
	if (s1->color() == Color::Wall)
		return false;
	if (s1 == nullptr)
		return false;
	while (sp->backStone() != nullptr)
		sp = sp->backStone();

	sptmp = sp->nextStone();
	while (sp != nullptr) {
		for (int i = 0; i < 4; i++) {
			if ( getStoneColor( getAstone(*sp, i) ) == Color::Null )
				return false;
		}
		sp = sptmp;
		if (sptmp != nullptr)
			sptmp = sptmp->nextStone();
	}
	return true;
}

bool BoardManager::isSolo(int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		if (Stone::Sqnce2color(board[x][y].sequence()) == getAstone(x, y, i).color())
			return true;
	}
	if (board[x][y].nextStone() == nullptr && board[x][y].backStone() == nullptr)
		return true;
	return false;
}
bool BoardManager::isSolo(Stone s)
{
	for (int i = 0; i < 4; i++)
	{
		if (Stone::Sqnce2color(s.sequence()) == getAstone(s, i).color())
			return false;
	}
	if (s.nextStone() == nullptr && s.backStone() == nullptr)
		return true;
	return false;
}