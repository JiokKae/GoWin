#include "Go.h"
#include "Stone/Stone.h"
#include "stdgo.h"
#include <tchar.h>
#include <cwctype>
#include <algorithm>
#include <fstream>
#include <format>
#include "GiboSGF.h"
#include <iostream>
#include <wtypes.h>

Go::Information::Information()
	: m_game_type(_T("친선대국"))
	, m_board_size(19)
	, m_compensation(6)
	, m_base_time(_T("0"))
	, m_game_result(_T("결과 없음"))
{
	init();
}

void Go::Information::init()
{
	m_placements.clear();
	m_sequence = 1;
	m_players.clear();
	m_players.emplace(Color::Black, Player(Color::Black));
	m_players.emplace(Color::White, Player(Color::White));
}

const Player& Go::Information::get_player(Color color) const
{
	static Player nullPlayer(Color::Black);

	return m_players.contains(color) == true ? m_players.at(color) : nullPlayer;
}

void Go::Information::add_captured_stone(Color color, int captured_stone)
{
	if (m_players.contains(color) == true)
	{
		m_players[color].add_captured_stone(captured_stone);
	}
}

void Go::Information::add_placement(PlacementInfo&& placement)
{
	m_placements.emplace_back(placement);
}

void Go::Information::delete_placement()
{
	m_placements.pop_back();
}

Go::Go()
	: m_board()
	, m_info()
	, m_current_placement_order_index(0)
{
	init();
}

const Go::Information& Go::info() const
{
	return m_info;
}

bool Go::backsies() 
{
	if (m_board_log.empty() == true)
	{
		return false;
	}

	m_board = m_board_log.back();
	m_board_log.pop_back();

	m_info.delete_placement();
	set_placement_order_previous();
	m_info.m_sequence -= 1;
	return true;
}

bool Go::handicap(int num) 
{
	if (m_info.m_sequence != 1 || num > 9 || num < 2)
		return false;
	m_board_log.push_back(m_board);
	m_board.setHandicap(num);

	m_info.m_sequence += 1;
	return true;
}

bool Go::init() 
{
	m_board_log.clear();
	m_board.init(19);
	m_info.init();
	m_current_placement_order_index = 0;

	m_placement_orders.clear();
	m_placement_orders.push_back(Color::Black);
	m_placement_orders.push_back(Color::White);

	return true;
}

bool Go::pass() 
{
	m_board_log.push_back(m_board);

	m_info.add_placement(PlacementInfo(0, 0, m_info.m_sequence, current_placement_order()));
	m_info.m_sequence += 1;
	set_placement_order_next();
	return true;
}

// function:	착수
// out:		errorMessage 
// in:		x, y 좌표
Go::PlacementError Go::placement(Coord2d coord_placement)
{
	int x = coord_placement.x;
	int y = coord_placement.y;
	int sequence = m_info.m_sequence;
	Color color = current_placement_order();

	// 0, 0 착점은 차례 넘기기
	if (x == 0 && y == 0) {
		pass();
		return PlacementError::NONE;
	}
	if (m_board.isBoardin(x, y) == false)
		return PlacementError::NOT_BOARD_IN;
	if (m_board.isEmpty(x, y) == false)
	{
		std::cout << m_board.getStone(x, y);
		return PlacementError::NOT_EMPTY;
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
				return PlacementError::ILLEGAL_POINT;
			}
		}
		m_board.setBoardtmp(x, y, sequence);
		if (m_board.isSolo(m_board.getAstone(x, y, direction)) && (m_board.getAstone(x, y, direction).sequence() == sequence - 1)
			&& m_board.getAstone(x, y, direction).is_capturer() == true)
			return PlacementError::KO;
	}

	m_board_log.push_back(m_board);
	int captured_stone = m_board.setBoard(x, y, sequence, color);
	if (captured_stone != 0)
		m_info.add_captured_stone(color, captured_stone);
	
	// 착수 정보 저장
	m_info.add_placement(PlacementInfo(x, y, sequence, color));
	std::wcout << m_info.m_placements.back().data();

	m_info.m_sequence += 1;
	set_placement_order_next();

	if (m_board.isDeadGS(&m_board.getStone(x, y)))
	{
		backsies();
		return PlacementError::ILLEGAL_POINT;
	}
		
	return PlacementError::NONE;
}

bool Go::load(const GiboNGF& gibo)
{
	m_info.m_game_type = gibo.battle_type();
	m_info.m_board_size = gibo.board_size();
	m_info.m_link = gibo.url();
	m_info.m_go_type = gibo.go_type();
	m_info.m_gongje = gibo.gongje();
	m_info.m_compensation = gibo.compensation();
	m_info.m_date = gibo.date();
	m_info.m_base_time = gibo.base_time();
	m_info.m_game_result = gibo.game_result();

	init();
	handicap(m_info.m_go_type);
	for (int i = 0; i < gibo.sequence(); i++)
	{
		std::wcout << i << std::endl;
		const GiboNGF::Placement& ngf_placement = gibo.getPlacements()[i];
		PlacementError errorMSG = placement(Coord2d{ ngf_placement.x(), ngf_placement.y() });
		if (PlacementError::NONE != errorMSG)
		{
			init();
			return false;
		}
	}
	return true;
}

void SaveNGF(std::wostream& wos, const Go::Information& goInfo, const std::wstring& date)
{
	GiboNGF ngf;
	ngf.set_battle_type(goInfo.m_game_type);
	ngf.set_board_size(goInfo.m_board_size);
	ngf.set_white_player(goInfo.get_player(Color::White).name(), goInfo.get_player(Color::White).kyu());
	ngf.set_black_player(goInfo.get_player(Color::Black).name(), goInfo.get_player(Color::Black).kyu());
	ngf.set_url(_T("https://blog.naver.com/damas125"));
	ngf.set_go_type(goInfo.m_go_type);
	ngf.set_gongje(goInfo.m_gongje);
	ngf.set_compensation(goInfo.m_compensation);
	ngf.set_date(date);
	ngf.set_base_time(goInfo.m_base_time);
	ngf.set_game_result(goInfo.m_game_result);
	for (int i = 0; i < goInfo.m_sequence- 1; i++)
	{
		const PlacementInfo& data = goInfo.m_placements[i].data();
		ngf.add_placement(GiboNGF::Placement(data.sequence, data.x, data.y, Color2Char(data.player)));
	}
	ngf.save(wos);
}

void SaveSGF(std::wostream& wos, const Go::Information& goInfo, const std::wstring& date)
{
	GiboSGF sgf{
		.application = _T("Go:1.0.2"),
		.game_name = goInfo.m_game_type,
		.date = date,
		.black_name = goInfo.get_player(Color::Black).name(),
		.black_rank = goInfo.get_player(Color::Black).kyu(),
		.white_name = goInfo.get_player(Color::White).name(),
		.white_rank = goInfo.get_player(Color::White).kyu(),
		.komi = std::to_wstring(goInfo.m_compensation) + _T(".5"),
		.result = goInfo.m_game_result,	// TODO: https://www.red-bean.com/sgf/user_guide/index.html format 맞추기
		.user = _T("https://blog.naver.com/damas125"),
		.size = goInfo.m_board_size,
		.handicap = goInfo.m_go_type,
	};
	
	for (const auto& placement : goInfo.m_placements)
	{
		sgf.placements.emplace_back(GiboSGF::Placement{ .x = placement.data().x, .y = placement.data().y, .color = TCHAR(Color2Char(placement.data().player)) });
	}

	wos << sgf;
}

bool Go::save(const std::wstring& directory, const std::wstring& gibo_extension)
{
	if (gibo_extension != _T(".ngf") && gibo_extension != _T(".sgf"))
	{
		printf("Save failed: invalid file file extension.\n");
		return false;
	}

	printf("기보 저장 시작--------\n");
	printf("경로 : %ls \n확장자 : %ws\n", directory.c_str(), gibo_extension.c_str());
	SYSTEMTIME time;
	GetLocalTime(&time);
	std::wstring date = std::format(_T("{}{:0>2}{:0>2} [{:0>2}:{:0>2}]"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);

	std::wofstream file(directory);
	file.imbue(std::locale(""));
	if (gibo_extension == _T(".ngf"))
	{
		SaveNGF(file, m_info, date);
	} 
	else if (gibo_extension == _T(".sgf"))
	{
		SaveSGF(file, m_info, date);
	}

	return true;
}

const Stone& Go::read_coord( Coord2d coord ) const
{
	return m_board.getStone(coord.x, coord.y);
}

const PlacementInfo& Go::getLastPlacementInfo() const
{
	return m_info.m_placements.back().data();
}

Color Go::current_placement_order() const
{
	return m_placement_orders.at(m_current_placement_order_index);
}

void Go::set_placement_order_previous()
{
	m_current_placement_order_index += m_placement_orders.size() - 1;
	m_current_placement_order_index %= m_placement_orders.size();
}

void Go::set_placement_order_next()
{
	++m_current_placement_order_index %= m_placement_orders.size();
}
