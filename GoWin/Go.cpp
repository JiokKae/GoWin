#include "Go.h"
#include "Player.h"
using namespace std;

// 무르기
bool Go::Backsies() {
	if (boardLog.empty())
		return false;
	m_board = boardLog.at(m_info.sequence() - 2);
	boardLog.pop_back();
	m_info.delete_placement();

	m_info.add_sequence(-1);
	return true;
}

// 핸디캡
bool Go::Handicap(int num) {
	if (m_info.sequence() != 1 || num > 9 || num < 2)
		return false;
	boardLog.push_back(m_board);
	m_board.setHandicap(num);

	m_info.add_sequence(1);
	return true;
}

// 초기화
bool Go::Init() {
	if (m_info.sequence() == 1)
		return false;
	boardLog.clear();

	m_board.init();
	m_info.Init();
	
	return true;
}

bool Go::Pass() {
	boardLog.push_back(m_board);

	PlacementInfo placement;
	placement.player = Stone::Sqnce2color(m_info.sequence());
	placement.placment = { 0, 0 };
	m_info.add_placement(placement);

	m_info.add_sequence(1);
	return true;
}

//	errormessage 착수(x, y)
int Go::Placement(Coord2d coord_placement) {
	int x = coord_placement.x;
	int y = coord_placement.y;
	int sequence = m_info.sequence();
	Color color = Stone::Sqnce2color(sequence);

	// 0, 0 착점은 넘기기
	if (x == 0 && y == 0) {
		Pass();
		return 0;
	}
	if (m_board.isBoardin(x, y) == false)
		return ERR_NOTBOARDIN;
	if (m_board.isEmpty(x, y) == false) 
		return ERR_NOTEMPTY;
	if (m_board.isIllegalpoint(x, y, sequence) == true)
	{
		int i;
		m_board.setBoardtmp(x, y, sequence);
		for (i = 0; i < 4; i++)
		{
			if (m_board.isDeadGS(m_board.getAstone(x, y, i).getRef()) == true && color != m_board.getAstone(x, y, i).color())
				break;
			if (i == 3)
			{
				m_board.setBoardtmp(x, y, sequence);
				return ERR_ILLEGALPOINT;
			}
		}
		m_board.setBoardtmp(x, y, sequence);
		if (m_board.isSolo(m_board.getAstone(x, y, i)) && (m_board.getAstone(x, y, i).sequence() == sequence - 1)
			&& m_board.getAstone(x, y, i).is_killer() == true)
			return ERR_KO;
	}

	boardLog.push_back(m_board);
	int captured_stone = m_board.setBoard(x, y, sequence);
	if (captured_stone)
		m_info.add_captured_stone(color, captured_stone);
	
	// 착수 정보 저장
	PlacementInfo placement;
	placement.sequence = sequence;
	placement.player = color;
	placement.placment = { x, y };
	print_data(placement);
	m_info.add_placement(placement);

	m_info.add_sequence(1);

	if (m_board.isDeadGS(&m_board.getStone(x, y)))
	{
		Backsies();
		return ERR_ILLEGALPOINT;
	}
		
	return 0;
}

bool Go::Load(GiboNGF& gibo)
{
	Player m_white_player;
	Player m_black_player;
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
		cout << i << endl;
		int errorMSG = Placement(gibo.getPlacement(i));
		if (0 != errorMSG)
		{
			Init();
			return false;
		}
	}
	return true;
}

bool Go::Save(LPWSTR address, wstring extension) {
	printf("기보 저장 시작--------\n");
	printf("경로 : %ls \n확장자 : %ws\n", address, extension.c_str());
	locale::global(locale("Korean"));
	SYSTEMTIME time;
	GetLocalTime(&time);
	wstring date = to_wstring(time.wYear)
		+ ((time.wMonth < 10) ? _T("0") : _T("")) + to_wstring(time.wMonth)
		+ ((time.wDay < 10) ? _T("0") : _T(""))	+ to_wstring(time.wDay)
		+_T(" [") + ((time.wHour < 10) ? _T("0") : _T("")) + to_wstring(time.wHour) + _T(":") + ((time.wMinute < 10) ? _T("0") : _T("")) + to_wstring(time.wMinute) + _T("]");

	if (extension == _T("NGF") || extension == _T("ngf")) //확장자 NGF
	{
		printf("    ngf 기록---------\n");
		wofstream gibofile(address);
		if (!gibofile.is_open())
		{
			printf("Failed : 파일 열기 실패\n");
			return false;
		}
			
		gibofile << m_info.game_type() << endl;
		gibofile << m_info.board_size() << endl;
		gibofile << m_info.white_player().to_ngf() << endl;
		gibofile << m_info.black_player().to_ngf() << endl;
		gibofile << "https://blog.naver.com/damas125" << endl;
		gibofile << m_info.go_type() << endl;
		gibofile << m_info.gongje() << endl;
		gibofile << m_info.compensation() << endl;
		gibofile << date << endl;
		gibofile << m_info.base_time() << endl;
		gibofile << m_info.game_result() << endl;
		gibofile << m_info.sequence()-1 << endl;
		for (int i = 0; i < m_info.sequence()-1; i++)
		{
			PlacementInfo data = m_info.placement().read(i).data();
			gibofile << _T("PM") << int2str_ngf(i) << data2str_ngf(data) << endl;
		}	
		gibofile.close();

	}
	else if(extension == _T("SGF") || extension == _T("sgf")) //확장자 SGF
	{
		printf("    sgf 기록---------\n");
		wofstream gibofile(address);
		if (!gibofile.is_open())
			return false;

		gibofile << "(";
		gibofile << ";AP[Go:1.0.2]";
		gibofile << "SZ[" << m_info.board_size() << "]";
		gibofile << "GN[" << m_info.game_type() << "]";
		gibofile << "DT[" << date << "]";
		gibofile << "PB[" << m_info.black_player().name() << "]";
		gibofile << "BR[" << m_info.black_player().kyu() << "]";
		gibofile << "PW[" << m_info.white_player().name() << "]";
		gibofile << "WR[" << m_info.white_player().kyu() << "]";
		gibofile << "KM[" << m_info.compensation() << ".5" << "]";
		gibofile << "HA[" << m_info.go_type() << "]";
		gibofile << "RE[" << m_info.game_result() << "]";
		gibofile << "US[" << "https://blog.naver.com/damas125" << "]";
		gibofile << endl;
		for (int i = 0; i < m_info.sequence() - 1; i++)
		{
			PlacementInfo data = m_info.placement().read(i).data();
			gibofile << data2str_sgf(data);
			if (i % 14 == 13)
				gibofile << endl;
		}
		gibofile << ")";
		gibofile.close();

	}
	else 
	{
		printf("Failed : 일치하는 확장자 없음\n");
		return false;
	}
		

	return true;
}

Stone Go::Read(Coord2d coord_read) {
	return m_board.getStone(coord_read.x, coord_read.y);
}

void GoInformation::add_captured_stone(Color color, int captured_stone)
{
	if (color == Color::Black)
		m_black_player->add_captured_stone(captured_stone);
	else if (color == Color::White)
		m_white_player->add_captured_stone(captured_stone);
}

LRESULT GoInformation::init()
{
	if (m_white_player == nullptr)
		m_white_player = new Player();
	m_white_player->init();

	if (m_white_player == nullptr)
		m_black_player = new Player();
	m_black_player->init();

	clear_placement();
	m_sequence = 1;

	return S_OK;
}

void GoInformation::release()
{
	m_white_player->release();
	delete m_white_player;

	m_black_player->release();
	delete m_black_player;
}