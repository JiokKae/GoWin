#include "Board.h" 


#define LEFT	0
#define RIGHT	1
#define UP	2
#define DOWN	3

const char* Board::direction_char[4] = { "[좌]", "[우]", "[상]", "[하]" };

void Board::init(int size) 
{
	board.resize(size + 2);
	for (int x = 0; x < size + 2; x++)
	{
		board[x].resize(size + 2);
		for (int y = 0; y < size + 2; y++)
		{
			if (x == 0 || x == size + 1 || y == 0 || y == size + 1)
				board[x][y] = Stone("Wall");
			else
				board[x][y] = Stone();
		}
	}

}

void Board::setHandicap( int num )
{
	if (num == 8) 
	{
		board[16][10] = Stone( 16, 10, 1, Color::Black );
		board[4][10] = Stone( 4, 10, 1, Color::Black );
		num = 6;
	}
	if (num == 6) 
	{
		board[10][16] = Stone( 10, 16, 1, Color::Black );
		board[10][4] = Stone( 10, 4, 1, Color::Black );
		num = 4;
	}
	switch (num)
	{
	case 9:
		board[16][10] = Stone( 16, 10, 1, Color::Black );
		board[4][10] = Stone( 4, 10, 1, Color::Black );
	case 7:
		board[10][16] = Stone( 10, 16, 1, Color::Black );
		board[10][4] = Stone( 10, 4, 1, Color::Black );
	case 5:
		board[10][10] = Stone( 10, 10, 1, Color::Black );
	case 4:
		board[16][16] = Stone( 16, 16, 1, Color::Black );
	case 3:
		board[4][4] = Stone( 4, 4, 1, Color::Black );
	case 2:
		board[4][16] = Stone( 4, 16, 1, Color::Black );
		board[16][4] = Stone( 16, 4, 1, Color::Black );
	}
}

//--------------------------------------------------------------------------------------
// Name:  setBoard
// Desc:  보드에 돌을 설정한다.
// Param: x		-> 돌의 x 좌표
//        y		-> 돌의 y 좌표
//        sequence	-> 돌의 수순
// Ret:   잡아낸 돌의 숫자
//--------------------------------------------------------------------------------------
int Board::setBoard(int x, int y, int sequence, Color color) 
{
	int ret = 0;
	board[x][y] = Stone(x, y, sequence, color);

	for (int i = 0; i < 4; i++)
	{
		switch (getAstone(x, y, i).state())
		{
		case Stone::State::Null:
			std::cout << i << " " << direction_char[i] << " : 비었음" << std::endl;
			break;
		case Stone::State::Wall:
			std::cout << i << " " << direction_char[i] << " : 장외" << std::endl;
			break;
		case Stone::State::Normal:
			if (color == getAstone(x, y, i).color())
			{
				std::cout << i << " " << direction_char[i] << " : 일치" << std::endl;
				linkGS(&board[x][y], &getAstone(x, y, i));
			}
			else
			{
				std::cout << i << " " << direction_char[i] << " : 불일치" << std::endl;
				if (isDeadGS(&getAstone(x, y, i)) == true)
				{
					ret = captureGS(&getAstone(x, y, i));
					board[x][y].set_killer(true);
					std::cout << i << " " << direction_char[i] << " : 삭제" << std::endl;
				}
			}
			break;
		default:
			break;
		}

		
	}

	return ret;
}

void Board::setBoardtmp(int x, int y, int sequence) {
	if (isEmpty(board[x][y]))
		board[x][y] = Stone(x, y, sequence, "temp");
	else
		board[x][y] = Stone();
}

Color Board::getStoneColor(int x, int y) {
	return board[x][y].color();
}
Color Board::getStoneColor(Stone s) {
	return s.color();
}

const Stone& Board::getStone(int x, int y) const
{
	return board[x][y];
}

Stone& Board::getStone(int x, int y)
{
	return board[x][y];
}

Stone& Board::getAstone( Stone s, int index ) 
{
	return getAstone( s.x(), s.y(), index );
}

Stone& Board::getAstone( int x, int y, int index ) 
{
	if (index == LEFT)
		return getStone( x - 1, y );
	else if (index == RIGHT)
		return getStone( x + 1, y );
	else if (index == UP)
		return getStone( x, y - 1 );
	else if (index == DOWN)
		return getStone( x, y + 1 );
	else
		return getStone( 0, 0 );
}

//s1과 s2를 연결
// TODO: GS 객체 만들어서 관리하도록 수정
void Board::linkGS(Stone* s1, Stone* s2) {
	Stone* s1last = s1;

	while (s1last->nextStone() != nullptr)
		s1last = s1last->nextStone();

	Stone* s2first = s2;

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
int Board::captureGS(Stone* capturedStone) 
{
	Stone* pStone = capturedStone;
	Stone* pStoneTemp;
	

	while (pStone->backStone() != nullptr)
	{
		pStone = pStone->backStone();
	}

	pStoneTemp = pStone->nextStone();
	int count = 0;
	while (pStone != nullptr) 
	{
		board[ pStone->x() ][ pStone->y() ] = Stone();
		
		*pStone = Stone();
		count++;
		pStone = pStoneTemp;
		if (pStoneTemp != nullptr) 
		{
			pStoneTemp = pStoneTemp->nextStone();
		}
			
	}
	return count;
}

bool Board::isBoardin(int x, int y) 
{
	return ((x < 20 && x >= 1) && (y < 20 && y >= 1));
}
bool Board::isBoardin(Stone s) 
{
	return ((s.x() < 20 && s.x() >= 1) && (s.y() < 20 && s.y() >= 1));
}

bool Board::isEmpty(int x, int y) 
{
	return (board[x][y].sequence() == 0);
}
bool Board::isEmpty(Stone s) 
{
	return (s.sequence() == 0);
}

bool Board::isIllegalpoint( int x, int y, Color color )
{
	//돌 주위에 빈곳이 있다면 착수금지점 아님
	for ( int i = 0; i < 4; i++ ) 
	{
		Stone& AroundStone = getAstone( x, y, i );
		if ( AroundStone.state() == Stone::State::Null || AroundStone.color() == color )
		{
			return false;
		}
	}
	return true;
}

bool Board::isDeadGS(const Stone* s1) 
{
	if (s1 == nullptr)
	{
		return false;
	}
	
	if (s1->state() == Stone::State::Wall) 
	{
		return false;
	}

	Stone* sp( const_cast<Stone*>(s1) );
	while ( sp->backStone() != nullptr )
	{
		sp = sp->backStone();
	}
		
	while (sp != nullptr) 
	{
		for (int i = 0; i < 4; i++) 
		{
			if ( getAstone(*sp, i).state() == Stone::State::Null )
			{
				return false;
			}
		}
		sp = sp->nextStone();
	}
	return true;
}

bool Board::isSolo( int x, int y )
{
	return isSolo( getStone( x, y ) );
}

bool Board::isSolo( const Stone& stone )
{
	for ( int i = 0; i < 4; i++ )
	{
		if ( stone.color() == getAstone( stone, i ).color() )
		{
			return false;
		}
	}

	return true;
}