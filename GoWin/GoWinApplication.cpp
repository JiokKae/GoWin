#include "GoWinApplication.h"
#include "resource.h"
#include "MyString.h"
#include "stdgo.h"
#include "MyWinLibrary.h"
#include "GoWin/BoardGraphic.h"
#include "GoWin/OpenFileName.h"
#include <filesystem>
#include <algorithm>
#include <cwctype>
#include <fstream>
#include <memory>

const TCHAR* GoWinApplication::ERROR_MESSAGES[5] = {
	_T(""),
	_T("바둑판 안에 착수해주세요"),
	_T("이미 바둑 돌이 있습니다"),
	_T("착수 금지점입니다"),
	_T("패 입니다"),
};

const std::map<GoWinApplication::StringID, std::wstring> GoWinApplication::STRINGS{
	{StringID::INVALID_EXTENSION,		_T("지원하는 파일 형식이 아닙니다.")},
	{StringID::FILE_OPEN_FAIL_TITLE,	_T("파일 열기 실패")},
	{StringID::FILE_OPEN_FAIL,		_T("파일을 불러오는데 실패했습니다.")},
	{StringID::FILE_SAVE_FAIL_TITLE,	_T("파일 저장 실패")},
	{StringID::FILE_SAVE_FAIL,		_T("파일 저장을 실패했습니다.")},
};

GoWinApplication::GoWinApplication()
	: command_message_callbacks{
		{Command_MSG::Command::BACKSIES, [this](HWND hWnd) {
			if (go.backsies() == false)
			{
				return;
			}

			if (my_socket.status() == MySocket::Status::Server)
			{
				my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::BACKSIES));
			}
		}},
		{Command_MSG::Command::INIT, [this](HWND hWnd) {
			if (go.init() == false)
			{
				return;
			}

			if (my_socket.status() == MySocket::Status::Server)
			{
				my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::INIT));
			}
		}},
		{Command_MSG::Command::PASS, [this](HWND hWnd) {
			if (go.pass() == false)
			{
				return;
			}

			if (my_socket.status() == MySocket::Status::Server)
			{
				my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::PASS));
			}
		}},
	}
	, font_gungseo(30, 0, 0, 0, 0, false, false, false, HANGEUL_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY,
		VARIABLE_PITCH | FF_ROMAN, _T("궁서"))
	, procedure_callbacks {
		{ WM_CREATE, [this](HWND hWnd, WPARAM wParam, LPARAM lParam) { on_create(hWnd, wParam, lParam); }},
		{ WM_MOUSEMOVE, [this](HWND hWnd, WPARAM wParam, LPARAM lParam) { on_mouse_move(hWnd, wParam, lParam); }},
		{ WM_LBUTTONDOWN, [this](HWND hWnd, WPARAM wParam, LPARAM lParam) { on_lbutton_down(hWnd, wParam, lParam); }},
		{ WM_COMMAND, [this](HWND hWnd, WPARAM wParam, LPARAM lParam) { on_command(hWnd, wParam, lParam); }},
		{ WM_ASYNC, [this](HWND hWnd, WPARAM wParam, LPARAM lParam) { on_async(hWnd, wParam, lParam); }},
		{ WM_KEYDOWN, [this](HWND hWnd, WPARAM wParam, LPARAM lParam) { on_key_down(hWnd, wParam, lParam); }},
		{ WM_PAINT, [this](HWND hWnd, WPARAM wParam, LPARAM lParam) { on_paint(hWnd, wParam, lParam); }},
		{ WM_DESTROY, [this](HWND hWnd, WPARAM wParam, LPARAM lParam) { on_destroy(hWnd, wParam, lParam); }},
	}
{
	go.init();
}

GoWinApplication::~GoWinApplication()
{
}

void GoWinApplication::set_main_window_handle(HWND handle)
{
	m_main_window_handle = handle;
}

void GoWinApplication::set_hInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

HINSTANCE GoWinApplication::hInstance() const
{
	return m_hInstance;
}

HWND GoWinApplication::main_window_handle() const
{
	return m_main_window_handle;
}

LRESULT GoWinApplication::main_procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (procedure_callbacks.contains(message) == false)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	procedure_callbacks[message](hWnd, wParam, lParam);

	return 0;
}

INT_PTR GoWinApplication::about_procedure(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
	{
		switch (message)
		{
		case WM_INITDIALOG:
			return INT_PTR(TRUE);

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return INT_PTR(TRUE);
			}
			break;
		}
		return INT_PTR(FALSE);
	}
}

INT_PTR GoWinApplication::netbox_procedure(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
	switch (message)
	{
	case WM_INITDIALOG:
		ip_input_box = GetDlgItem(hDlg, IDC_IP_INPUT);
		return INT_PTR(TRUE);

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CONNECT:
		{
			WCHAR buffer[64];
			GetWindowText(ip_input_box, buffer, 64);
			my_socket.Enter(m_main_window_handle, WCharToChar(buffer));
			EndDialog(hDlg, LOWORD(wParam));
			return INT_PTR(TRUE);
		}

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return INT_PTR(TRUE);

		default:
			break;
		}
	}
	return INT_PTR(FALSE);
}

void GoWinApplication::on_create(HWND hWnd, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
#ifdef _DEBUG	// 콘솔창 열기
	AllocConsole();
	FILE* fp = NULL;
	_wfreopen_s(&fp, _T("CONOUT$"), _T("wt"), stdout);
#endif
	hdc = GetDC(hWnd);

	board_graphic = std::move(std::make_unique<BoardGraphic>(hdc));
	if (board_graphic)
	{
		board_graphic->SetLeftTopPoint(0, 0);
		board_graphic->SetSpaceSize(42);
		board_graphic->SetBorderSize(6);
		board_graphic->AddBitmap("blackStone", m_hInstance, IDB_BLACKSTONE);
		board_graphic->AddBitmap("whiteStone", m_hInstance, IDB_WHITESTONE);
		board_graphic->AddBitmap("board", m_hInstance, IDB_BOARD);
	}
	
	hdc_BackGround = CreateCompatibleDC(hdc);
	HBITMAP bitBackGround = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BACKGROUND));
	SelectObject(hdc_BackGround, bitBackGround);
	//DeleteObject(bitBackGround);

	hdcMem = CreateCompatibleDC(hdc); //2
	hBitmapMem = CreateCompatibleBitmap(hdc, 1200, 820);//3
	DeleteObject(SelectObject(hdcMem, hBitmapMem));

	hBCS = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
		840, 200, 50, 30, hWnd, (HMENU)1, m_hInstance, NULL);
	hWCS = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
		1010, 200, 50, 30, hWnd, (HMENU)2, m_hInstance, NULL);

	SendMessage(hBCS, WM_SETFONT, (WPARAM)font_gungseo.handle(), (LPARAM)FALSE);
	SendMessage(hWCS, WM_SETFONT, (WPARAM)font_gungseo.handle(), (LPARAM)FALSE);

	CreateWindow(_T("button"), _T("무르기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		840, 300, 150, 30, hWnd, (HMENU)IDA_BACKSIES, m_hInstance, NULL);
	CreateWindow(_T("button"), _T("초기화"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1010, 300, 150, 30, hWnd, (HMENU)IDA_INIT, m_hInstance, NULL);
	CreateWindow(_T("button"), _T("한수쉼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		840, 340, 150, 30, hWnd, (HMENU)IDA_PASS, m_hInstance, NULL);
	CreateWindow(_T("button"), _T("수순 표시"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1010, 340, 150, 30, hWnd, (HMENU)TOGGLE_SHOW_SEQUENCE, m_hInstance, NULL);
	chatting.set_output_handle(CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL | ES_MULTILINE,
		840, 400, 320, 120, hWnd, (HMENU)4, m_hInstance, NULL));
	chatting.set_input_handle(CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN,
		840, 530, 320, 30, hWnd, (HMENU)5, m_hInstance, NULL));
}

void GoWinApplication::on_mouse_move(HWND hWnd, WPARAM /*wParam*/, LPARAM lParam)
{
	set_mouse_coord(lParam);

	InvalidateRect(hWnd, NULL, FALSE);
}

void GoWinApplication::on_lbutton_down(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	set_mouse_coord(lParam);

	if (board_graphic && board_graphic->IsPointInBoard(mouse.x, mouse.y) == true)
	{
		const int placement_x = board_graphic->PointToBoard(mouse.x);
		const int placement_y = board_graphic->PointToBoard(mouse.y);
		const Coord2d placement_point{ placement_x , placement_y};

		if (my_socket.status() == MySocket::Status::Client)
		{
			my_socket.send_message(std::make_unique<Placement_MSG>(go.info().m_sequence + 1, placement_point.x, placement_point.y));
			return;
		}

		Go::PlacementError errorMSG = go.placement(placement_point);
		if (errorMSG == Go::PlacementError::NONE)
		{
			if (my_socket.status() == MySocket::Status::Server)
			{

				const auto& placementInfo = go.getLastPlacementInfo();
				my_socket.send_message(std::make_unique<Placement_MSG>(placementInfo.sequence, placementInfo.x, placementInfo.y));
			}
			SetWindowText(hBCS, std::to_wstring(go.info().get_player(Color::Black).captured_stone()).c_str());
			SetWindowText(hWCS, std::to_wstring(go.info().get_player(Color::White).captured_stone()).c_str());

			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (errorMSG != Go::PlacementError::NOT_EMPTY)
		{
			MessageBox(hWnd, ERROR_MESSAGES[(int)errorMSG], _T("ERROR"), MB_OK);
		}
	}
}

void GoWinApplication::on_command(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	const int wmId = LOWORD(wParam);
	// 메뉴 선택을 구문 분석합니다:
	switch (wmId)
	{
	case IDM_FILE_OPEN:
	{
		file_open(hWnd);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	}
	// 메뉴 - 파일 - 저장
	case IDM_FILE_SAVE:
		file_save(hWnd);
		break;

	case IDM_FILE_EXIT:
#ifdef _DEBUG	// 콘솔창 닫기
		FreeConsole();
#endif
		DeleteDC(hdc);
		DeleteDC(hdcMem);

		// PostQuitMessage(0);
		// break;

		DestroyWindow(hWnd);
		break;

	case IDM_SERVER_CREATE:
		if (my_socket.Create(hWnd) != INVALID_SOCKET) 
		{
			chatting.system_print(_T("서버 생성"));
			const HMENU hMenu = GetMenu(hWnd);
			const HMENU hSubMenu = GetSubMenu(hMenu, 1);
			EnableMenuItem(hMenu, GetMenuItemID(hSubMenu, 0), MF_GRAYED);
		}
		else
		{
			chatting.system_print(_T("서버 생성 실패"));
		}
		break;

	case IDA_BACKSIES:	//무르기 버튼
		backsies(hWnd);
		SetFocus(hWnd);
		break;

	case IDA_INIT:	//초기화 버튼
		init(hWnd);
		SetFocus(hWnd);
		break;

	case IDA_PASS:	//한수쉼 버튼
		pass(hWnd);
		SetFocus(hWnd);
		break;

	case TOGGLE_SHOW_SEQUENCE:
		if (board_graphic)
		{
			board_graphic->TogglePrintSequence();
		}
		InvalidateRect(hWnd, NULL, FALSE);
		SetFocus(hWnd);
		break;
	}
}

void GoWinApplication::on_async(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	// 클라이언트의 접속 요청이 있을 때
	case FD_ACCEPT:
		if (my_socket.FD_Accept()) 
		{
			chatting.system_print(_T("클라이언트 접속"));
		}
		else
		{
			chatting.system_print(_T("클라이언트 접속 실패"));
		}
		break;

	// 상호간에 메시지를 받았을 때
	case FD_READ:
	{
		COMM_MSG msg;
		my_socket.FD_Read((SOCKET)wParam, &msg);
		switch (msg.type)
		{
		case Message::Type::CHATTING:
		{
			CHAT_MSG* chat_msg = reinterpret_cast<CHAT_MSG*>(&msg);
			chatting.print(std::format(_T("상대: {}"), chat_msg->buf).c_str());
			break;
		}
		case Message::Type::PLACEMENT:
		{
			Placement_MSG* placement_msg = reinterpret_cast<Placement_MSG*>(&msg);
			Go::PlacementError errorMSG = go.placement({ placement_msg->x, placement_msg->y });
			if (errorMSG == Go::PlacementError::NONE)
			{
				if (my_socket.status() == MySocket::Status::Server)
				{
					const PlacementInfo& placementInfo = go.getLastPlacementInfo();
					my_socket.send_message(std::make_unique<Placement_MSG>(placementInfo.sequence, placementInfo.x, placementInfo.y));
				}
				SetWindowText(hBCS, std::to_wstring(go.info().get_player(Color::Black).captured_stone()).c_str());
				SetWindowText(hWCS, std::to_wstring(go.info().get_player(Color::White).captured_stone()).c_str());

				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		}
		case Message::Type::COMMAND:
		{
			Command_MSG* command_msg = reinterpret_cast<Command_MSG*>(&msg);
			command_message_callbacks[command_msg->command](hWnd);

			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		default:
			break;
		}
	}
	break;

	// 클라이언트가 접속을 해제 했을 때
	case FD_CLOSE:
		my_socket.FD_Close((SOCKET)wParam);
		chatting.system_print(_T("클라이언트 접속 종료"));
		break;
	}
}

void GoWinApplication::on_key_down(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	//cout << wParam <<"키 눌림" << endl;
#endif
	switch (wParam)
	{
	case KEY_ENTER:
	{
		SetFocus(chatting.input_handle());

		const std::wstring chat = chatting.send();
		if (chat.empty() == false && my_socket.IsConnected() == true)
		{
			const int ret = my_socket.send_message(std::make_unique<CHAT_MSG>(chat.c_str()));
			if (ret == SOCKET_ERROR)
			{
				chatting.system_print(_T("채팅 보내기 실패"));
			}
		}
		break;
	}
	default:
		break;
	}
}

void GoWinApplication::on_paint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	hdc = BeginPaint(hWnd, &ps);

	BitBlt(hdcMem, 0, 0, 1200, 820, hdc_BackGround, 0, 0, SRCCOPY);

	if (board_graphic)
	{
		board_graphic->Draw(hdcMem, go, mouse);
	}

	BitBlt(hdc, 0, 0, 1200, 820, hdcMem, 0, 0, SRCCOPY);

	EndPaint(hWnd, &ps);
}

void GoWinApplication::on_destroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DeleteObject(hBitmapMem); //-3
	DeleteDC(hdcMem); //-2

	DeleteDC(hdc_BackGround);

	ReleaseDC(hWnd, hdc);

	PostQuitMessage(0);

}

void GoWinApplication::file_open(HWND hWnd)
{
	try
	{
		GoWin::OpenFileName ofn(hWnd, _T("기보 파일(*.ngf)\0*.ngf\0"), OFN_FILEMUSTEXIST);

		auto result = ofn.open();
		if (result.success == false)
		{
			ok_message_box(hWnd, STRINGS.at(StringID::FILE_OPEN_FAIL_TITLE), STRINGS.at(StringID::FILE_OPEN_FAIL));
			return;
		}

		std::wstring extension = get_extension(result.file_path);
		if (extension != _T(".ngf"))
		{
			ok_message_box(hWnd, STRINGS.at(StringID::FILE_OPEN_FAIL_TITLE), STRINGS.at(StringID::INVALID_EXTENSION));
			return;
		}

		std::wifstream file(result.file_path);
		file.imbue(std::locale(""));

		go.load(GiboNGF(file));
	}
	catch (const std::exception&)
	{
		ok_message_box(hWnd, STRINGS.at(StringID::FILE_OPEN_FAIL_TITLE), STRINGS.at(StringID::FILE_OPEN_FAIL));
	}
}

void GoWinApplication::file_save(HWND hWnd)
{
	try
	{
		GoWin::OpenFileName sfn(hWnd, _T("기보 파일(*.ngf)\0*.ngf\0기보 파일(*.sgf)\0*.sgf\0모든 파일(*.*)\0*.*\0"), OFN_OVERWRITEPROMPT);

		auto result = sfn.save(_T("ngf"));
		if (result.success == false)
		{
			return;
		}

		go.save(result.file_path, get_extension(result.file_path));
	}
	catch (const std::exception& /*e*/)
	{
		ok_message_box(hWnd, STRINGS.at(StringID::FILE_SAVE_FAIL_TITLE), STRINGS.at(StringID::FILE_SAVE_FAIL));
	}
}

void GoWinApplication::backsies(HWND hWnd)
{
	if (my_socket.status() == MySocket::Status::Client)
	{
		my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::BACKSIES));
		return;
	}

	if (go.backsies() == false)
	{
		return;
	}

	if (my_socket.status() == MySocket::Status::Server)
	{
		my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::BACKSIES));
	}

	InvalidateRect(hWnd, NULL, FALSE);
}

void GoWinApplication::init(HWND hWnd)
{
	if (my_socket.status() == MySocket::Status::Client)
	{
		my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::INIT));
		return;
	}

	if (go.init() == false)
	{
		return;
	}

	if (my_socket.status() == MySocket::Status::Server)
	{
		my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::INIT));
	}

	InvalidateRect(hWnd, NULL, FALSE);
	chatting.system_print(_T("바둑판을 초기화 했습니다."));
}

void GoWinApplication::pass(HWND hWnd)
{
	if (my_socket.status() == MySocket::Status::Client)
	{
		my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::PASS));
		return;
	}

	if (go.pass() == false)
	{
		return;
	}

	if (my_socket.status() == MySocket::Status::Server)
	{
		my_socket.send_message(std::make_unique<Command_MSG>(Command_MSG::Command::PASS));
	}

	InvalidateRect(hWnd, NULL, FALSE);
	chatting.system_print(_T("한수 쉬었습니다."));
}

void GoWinApplication::set_mouse_coord(LPARAM lparam)
{
	mouse.x = LOWORD(lparam);
	mouse.y = HIWORD(lparam);
}

std::wstring GoWinApplication::get_extension(const std::wstring& path)
{
	std::wstring extension = std::filesystem::path(path).extension();
	std::transform(extension.begin(), extension.end(), extension.begin(), std::towlower);

	return extension;
}

void GoWinApplication::ok_message_box(HWND hWnd, const std::wstring& title, const std::wstring& content)
{
	MessageBox(hWnd, content.c_str(), title.c_str(), MB_OK);
}
