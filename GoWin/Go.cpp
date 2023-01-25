#include "Go.h"
#include "Board.h"
#include "Stone/Stone.h"
#include "stdgo.h"
#include <tchar.h>
#include <cwctype>
#include <algorithm>
#include <fstream>

Go::Go()
	: m_mode("Single")
	, m_board( new Board() )
	, m_info()
	, currentPlacementOrderIndex(0)
{
	Init();
}

Go::~Go()
{
}

// 무르기
bool Go::Backsies() 
{
	if (boardLog.empty() == true)
	{
		return false;
	}

	m_board = boardLog.back();
	boardLog.pop_back();

	m_info.delete_placement();
	m_info.add_sequence(-1);

	return true;
}

// 핸디캡
bool Go::Handicap(int num) 
{
	if (m_info.sequence() != 1 || num > 9 || num < 2)
		return false;
	boardLog.push_back(m_board);
	m_board->setHandicap(num);

	m_info.add_sequence(1);
	return true;
}

// 초기화
bool Go::Init() 
{
	boardLog.clear();
	m_board->init();
	m_info.Init();
	currentPlacementOrderIndex = 0;

	placementOrders.clear();
	placementOrders.push_back(Color::Black);
	placementOrders.push_back(Color::White);

	return true;
}

bool Go::Pass() 
{
	boardLog.push_back(m_board);

	m_info.add_placement(PlacementInfo(0, 0, m_info.sequence(), get_current_placement_order()));
	m_info.add_sequence(1);
	set_placement_order_next();
	return true;
}

// function:	착수
// out:		errorMessage 
// in:		x, y 좌표
int Go::Placement( Coord2d coord_placement, Color color ) 
{
	int x = coord_placement.x;
	int y = coord_placement.y;
	int sequence = m_info.sequence();

	// 0, 0 착점은 차례 넘기기
	if (x == 0 && y == 0) {
		Pass();
		return 0;
	}
	if (m_board->isBoardin(x, y) == false)
		return ERR_NOTBOARDIN;
	if (m_board->isEmpty(x, y) == false)
		return ERR_NOTEMPTY;
	if (m_board->isIllegalpoint(x, y, color) == true)
	{
		int direction;
		m_board->setBoardtmp(x, y, sequence);
		for (direction = 0; direction < 4; direction++)
		{
			if (m_board->isDeadGS( &m_board->getAstone(x, y, direction) ) == true && color != m_board->getAstone(x, y, direction).color())
				break;
			if (direction == 3)
			{
				m_board->setBoardtmp(x, y, sequence);
				return ERR_ILLEGALPOINT;
			}
		}
		m_board->setBoardtmp(x, y, sequence);
		if (m_board->isSolo(m_board->getAstone(x, y, direction)) && (m_board->getAstone(x, y, direction).sequence() == sequence - 1)
			&& m_board->getAstone(x, y, direction).is_killer() == true)
			return ERR_KO;
	}

	boardLog.push_back(m_board);
	int captured_stone = m_board->setBoard(x, y, sequence, color);
	if (captured_stone != 0)
		m_info.add_captured_stone(color, captured_stone);
	
	// 착수 정보 저장
	PlacementInfo placement(x, y, sequence, color);
	placement.print( std::wcout );
	m_info.add_placement(placement);

	m_info.add_sequence(1);
	set_placement_order_next();

	if (m_board->isDeadGS(&m_board->getStone(x, y)))
	{
		Backsies();
		return ERR_ILLEGALPOINT;
	}
		
	return 0;
}

bool Go::Load(GiboNGF& gibo)
{
	LinkedList m_placement;
	m_info.set_game_type(gibo.battle_type());
	m_info.set_board_size(gibo.board_size());
	m_info.set_link(gibo.link());
	m_info.set_go_type(gibo.go_type());
	m_info.set_gongje(gibo.gongje());
	m_info.set_compensation(gibo.compensation());
	m_info.set_date(gibo.date());
	m_info.set_base_time(gibo.base_time());
	m_info.set_game_result(gibo.game_result());

	Init();
	Handicap(m_info.go_type());
	for (int i = 0; i < gibo.sequence(); i++)
	{
		wcout << i << endl;
		const auto& placement = gibo.getPlacement(i+1);
		int errorMSG = Placement(Coord2d(placement.x(), placement.y()) , get_current_placement_order() ); // 확인 TODO
		if (0 != errorMSG)
		{
			Init();
			return false;
		}
	}
	return true;
}

void SaveNGF(LPWSTR directory, const Go::Information& goInfo, const wstring& date)
{
	wofstream gibofile(directory);
	if (gibofile.is_open() == false)
	{
		printf("Failed : 파일 열기 실패\n");
		return;
	}

	gibofile << goInfo.game_type() << endl;
	gibofile << goInfo.board_size() << endl;
	gibofile << goInfo.get_player(Color::White).to_ngf() << endl;
	gibofile << goInfo.get_player(Color::Black).to_ngf() << endl;
	gibofile << "https://blog.naver.com/damas125" << endl;
	gibofile << goInfo.go_type() << endl;
	gibofile << goInfo.gongje() << endl;
	gibofile << goInfo.compensation() << endl;
	gibofile << date << endl;
	gibofile << goInfo.base_time() << endl;
	gibofile << goInfo.game_result() << endl;
	gibofile << goInfo.sequence() - 1 << endl;
	for (int i = 0; i < goInfo.sequence() - 1; i++)
	{
		PlacementInfo data = goInfo.placement().read(i).data();
		gibofile << GiboNGF::Placement(i + 1, data.placement.x, data.placement.y, Color2Char(data.player)).ToString() << endl;
	}
	gibofile.close();
}

void SaveSGF(LPWSTR directory, const Go::Information& goInfo, const wstring& date)
{
	wofstream gibofile(directory);
	if (gibofile.is_open() == false)
	{
		printf("Failed : 파일 열기 실패\n");
		return;
	}

	gibofile << "(";
	gibofile << ";AP[Go:1.0.2]";
	gibofile << "SZ[" << goInfo.board_size() << "]";
	gibofile << "GN[" << goInfo.game_type() << "]";
	gibofile << "DT[" << date << "]";
	gibofile << "PB[" << goInfo.get_player(Color::Black).name() << "]";
	gibofile << "BR[" << goInfo.get_player(Color::Black).kyu() << "]";
	gibofile << "PW[" << goInfo.get_player(Color::White).name() << "]";
	gibofile << "WR[" << goInfo.get_player(Color::White).kyu() << "]";
	gibofile << "KM[" << goInfo.compensation() << ".5" << "]";
	gibofile << "HA[" << goInfo.go_type() << "]";
	gibofile << "RE[" << goInfo.game_result() << "]";
	gibofile << "US[" << "https://blog.naver.com/damas125" << "]";
	gibofile << endl;
	for (int i = 0; i < goInfo.sequence() - 1; i++)
	{
		PlacementInfo data = goInfo.placement().read(i).data();
		gibofile << data.to_sgf();
		if (i % 14 == 13)
			gibofile << endl;
	}
	gibofile << ")";
	gibofile.close();
}

bool Go::Save(LPWSTR address, wstring extension) 
{
	printf("기보 저장 시작--------\n");
	printf("경로 : %ls \n확장자 : %ws\n", address, extension.c_str());
	locale::global(locale("Korean"));
	SYSTEMTIME time;
	GetLocalTime(&time);
	wstring date = to_wstring(time.wYear)
		+ ((time.wMonth < 10) ? _T("0") : _T("")) + to_wstring(time.wMonth)
		+ ((time.wDay < 10) ? _T("0") : _T(""))	+ to_wstring(time.wDay)
		+_T(" [") + ((time.wHour < 10) ? _T("0") : _T("")) + to_wstring(time.wHour) + _T(":") + ((time.wMinute < 10) ? _T("0") : _T("")) + to_wstring(time.wMinute) + _T("]");

	transform(extension.begin(), extension.end(), extension.begin(), towlower);
	if (extension == _T("ngf"))
	{
		printf("    ngf 기록---------\n");
		SaveNGF(address, m_info, date);
		return true;
	}
	
	if(extension == _T("sgf"))
	{
		printf("    sgf 기록---------\n");
		SaveSGF(address, m_info, date);
		return true;
	}

	printf("Failed : 일치하는 확장자 없음\n");
	return false;
}

const Stone& Go::ReadCoord( Coord2d coord )
{
	return m_board->getStone(coord.x, coord.y);
}

const PlacementInfo& Go::getLastPlacementInfo() const
{
	return m_info.placement().getLastNode().data();
}

void Go::Information::add_captured_stone(Color color, int captured_stone)
{
	auto playerItr = players.find(color);

	if (playerItr == players.cend())
	{
		return;
	}

	playerItr->second.add_captured_stone(captured_stone);
}

void Go::Information::add_placement(PlacementInfo placement)
{
	m_placement.push_back(new Node(placement));
}

void Go::Information::clear_placement()
{
	while ( m_placement.size() != 0 )
	{
		m_placement.delete_back();
	}
}

void Go::set_placement_order_next()
{
	++currentPlacementOrderIndex %= placementOrders.size();
}

const Player& Go::Information::get_player(Color color) const
{
	auto playerItr = players.find(color);

	if ( playerItr != players.cend() )
	{
		return playerItr->second;
	}

	static Player nullPlayer(Color::Black);
	return nullPlayer;
}

const std::wstring& Go::Information::game_type() const
{
	return m_game_type;
}

int Go::Information::board_size() const
{
	return m_board_size;
}

const std::wstring& Go::Information::link() const
{
	return m_link;
}

int Go::Information::go_type() const
{
	return m_go_type;
}

int Go::Information::gongje() const
{
	return m_gongje;
}

int Go::Information::compensation() const
{
	return m_compensation;
}

const std::wstring& Go::Information::date() const
{
	return m_date;
}

const std::wstring& Go::Information::base_time() const
{
	return m_base_time;
}

const std::wstring& Go::Information::game_result() const
{
	return m_game_result;
}

int Go::Information::sequence() const
{
	return m_sequence;
}

LinkedList Go::Information::placement() const
{
	return m_placement;
}

Color Go::get_current_placement_order() const
{
	return placementOrders.at(currentPlacementOrderIndex);
}

Go::Information::Information()
	: m_game_type( _T( "친선대국" ) )
	, m_board_size( 19 )
	, m_compensation( 6 )
	, m_base_time( _T( "0" ) )
	, m_game_result( _T( "결과 없음" ) )
	
{
	Init();
}

void Go::Information::Init()
{
	clear_placement();
	set_sequence(1);
	players.clear();
	players.emplace(std::make_pair(Color::Black, Player(Color::Black)));
	players.emplace(std::make_pair(Color::White, Player(Color::White)));
}
