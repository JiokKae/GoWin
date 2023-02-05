#include "Go.h"
#include "Stone/Stone.h"
#include "stdgo.h"
#include <tchar.h>
#include <cwctype>
#include <algorithm>
#include <fstream>
#include <format>

Go::Go()
	: m_mode("Single")
	, m_board()
	, m_info()
	, currentPlacementOrderIndex(0)
{
	Init();
}

Go::~Go()
{
}

bool Go::Backsies() 
{
	if (boardLog.empty() == true)
	{
		return false;
	}

	m_board = boardLog.back();
	boardLog.pop_back();

	m_info.delete_placement();
	set_placement_order_previous();
	m_info.add_sequence(-1);

	return true;
}

bool Go::Handicap(int num) 
{
	if (m_info.sequence() != 1 || num > 9 || num < 2)
		return false;
	boardLog.push_back(m_board);
	m_board.setHandicap(num);

	m_info.add_sequence(1);
	return true;
}

bool Go::Init() 
{
	boardLog.clear();
	m_board.init(19);
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
int Go::Placement( Coord2d coord_placement ) 
{
	int x = coord_placement.x;
	int y = coord_placement.y;
	int sequence = m_info.sequence();
	Color color = get_current_placement_order();

	// 0, 0 착점은 차례 넘기기
	if (x == 0 && y == 0) {
		Pass();
		return 0;
	}
	if (m_board.isBoardin(x, y) == false)
		return ERR_NOTBOARDIN;
	if (m_board.isEmpty(x, y) == false)
	{
		m_board.getStone(x, y).Print(std::cout);
		return ERR_NOTEMPTY;
	}
		
	if (m_board.isIllegalpoint(x, y, color) == true)
	{
		int direction;
		m_board.setBoardtmp(x, y, sequence);
		for (direction = 0; direction < 4; direction++)
		{
			if (m_board.isDeadGS( &m_board.getAstone(x, y, direction) ) == true && color != m_board.getAstone(x, y, direction).color())
				break;
			if (direction == 3)
			{
				m_board.setBoardtmp(x, y, sequence);
				return ERR_ILLEGALPOINT;
			}
		}
		m_board.setBoardtmp(x, y, sequence);
		if (m_board.isSolo(m_board.getAstone(x, y, direction)) && (m_board.getAstone(x, y, direction).sequence() == sequence - 1)
			&& m_board.getAstone(x, y, direction).is_killer() == true)
			return ERR_KO;
	}

	boardLog.push_back(m_board);
	int captured_stone = m_board.setBoard(x, y, sequence, color);
	if (captured_stone != 0)
		m_info.add_captured_stone(color, captured_stone);
	
	// 착수 정보 저장
	PlacementInfo placement(x, y, sequence, color);
	placement.print( std::wcout );
	m_info.add_placement(placement);

	m_info.add_sequence(1);
	set_placement_order_next();

	if (m_board.isDeadGS(&m_board.getStone(x, y)))
	{
		Backsies();
		return ERR_ILLEGALPOINT;
	}
		
	return 0;
}

bool Go::Load(GiboNGF& gibo)
{
	m_info.set_game_type(gibo.battle_type());
	m_info.set_board_size(gibo.board_size());
	m_info.set_link(gibo.url());
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
		std::wcout << i << std::endl;
		const auto& placement = gibo.getPlacements()[i];
		int errorMSG = Placement(Coord2d(placement.x(), placement.y())); // 확인 TODO
		if (0 != errorMSG)
		{
			Init();
			return false;
		}
	}
	return true;
}

void SaveNGF(std::wostream& wos, const Go::Information& goInfo, const std::wstring& date)
{
	GiboNGF ngf;
	ngf.set_battle_type(goInfo.game_type());
	ngf.set_board_size(goInfo.board_size());
	ngf.set_white_player(goInfo.get_player(Color::White).name(), goInfo.get_player(Color::White).kyu());
	ngf.set_black_player(goInfo.get_player(Color::Black).name(), goInfo.get_player(Color::Black).kyu());
	ngf.set_url(_T("https://blog.naver.com/damas125"));
	ngf.set_go_type(goInfo.go_type());
	ngf.set_gongje(goInfo.gongje());
	ngf.set_compensation(goInfo.compensation());
	ngf.set_date(date);
	ngf.set_base_time(goInfo.base_time());
	ngf.set_game_result(goInfo.game_result());
	for (int i = 0; i < goInfo.sequence() - 1; i++)
	{
		const PlacementInfo& data = goInfo.placements()[i].data();
		ngf.add_placement(GiboNGF::Placement(data.sequence, data.placement.x, data.placement.y, Color2Char(data.player)));
	}
	ngf.save(wos);
}

void SaveSGF(std::wostream& wos, const Go::Information& goInfo, const std::wstring& date)
{
	wos << "(";
	wos << ";AP[Go:1.0.2]";
	wos << "SZ[" << goInfo.board_size() << "]";
	wos << "GN[" << goInfo.game_type() << "]";
	wos << "DT[" << date << "]";
	wos << "PB[" << goInfo.get_player(Color::Black).name() << "]";
	wos << "BR[" << goInfo.get_player(Color::Black).kyu() << "]";
	wos << "PW[" << goInfo.get_player(Color::White).name() << "]";
	wos << "WR[" << goInfo.get_player(Color::White).kyu() << "]";
	wos << "KM[" << goInfo.compensation() << ".5" << "]";
	wos << "HA[" << goInfo.go_type() << "]";
	wos << "RE[" << goInfo.game_result() << "]";
	wos << "US[" << "https://blog.naver.com/damas125" << "]";
	wos << std::endl;
	for (int i = 0; i < goInfo.sequence() - 1; i++)
	{
		wos << goInfo.placements()[i].data().to_sgf();
		if (i % 14 == 13)
			wos << std::endl;
	}
	wos << ")";
}

bool Go::Save(LPWSTR directory, std::wstring file_extension)
{
	std::transform(file_extension.begin(), file_extension.end(), file_extension.begin(), std::towlower);
	if (file_extension != _T("ngf") && file_extension != _T("sgf"))
	{
		printf("Save failed: invalid file file extension.\n");
		return false;
	}

	printf("기보 저장 시작--------\n");
	printf("경로 : %ls \n확장자 : %ws\n", directory, file_extension.c_str());
	SYSTEMTIME time;
	GetLocalTime(&time);
	std::wstring date = std::format(_T("{}{:0>2}{:0>2} [{:0>2}:{:0>2}]"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);

	std::wofstream file(directory);
	file.imbue(std::locale(""));
	if (file_extension == _T("ngf"))
	{
		SaveNGF(file, m_info, date);
	} 
	else if (file_extension == _T("sgf"))
	{
		SaveSGF(file, m_info, date);
	}
	file.close();

	return true;
}

const Stone& Go::ReadCoord( Coord2d coord )
{
	return m_board.getStone(coord.x, coord.y);
}

const PlacementInfo& Go::getLastPlacementInfo() const
{
	return m_info.placements().back().data();
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
	m_placements.push_back(Node(placement));
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

const std::vector<Node>& Go::Information::placements() const
{
	return m_placements;
}

Color Go::get_current_placement_order() const
{
	return placementOrders.at(currentPlacementOrderIndex);
}

void Go::set_placement_order_previous()
{
	currentPlacementOrderIndex += placementOrders.size() - 1;
	currentPlacementOrderIndex %= placementOrders.size();
}

void Go::set_placement_order_next()
{
	++currentPlacementOrderIndex %= placementOrders.size();
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
	m_placements.clear();
	set_sequence(1);
	players.clear();
	players.emplace(std::make_pair(Color::Black, Player(Color::Black)));
	players.emplace(std::make_pair(Color::White, Player(Color::White)));
}
