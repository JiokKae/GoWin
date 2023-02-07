#include "GoWinApplication.h"
#include "resource.h"
#include "MyString.h"
#include "stdgo.h"
#include "MyWinLibrary.h"
#include "GoWin/OpenFileName.h"
#include <filesystem>
#include <algorithm>
#include <cwctype>
#include <fstream>

GoWinApplication::GoWinApplication()
	: m_main_window_handle(nullptr)
	, strings{
		{string_id::INVALID_EXTENSION, _T("지원하는 파일 형식이 아닙니다.")},
		{string_id::FILE_OPEN_FAIL_TITLE, _T("파일 열기 실패")},
		{string_id::FILE_OPEN_FAIL, _T("파일을 불러오는데 실패했습니다.")},
	}
	, command_message_callbacks{
		{BACKSIES, [this](HWND hWnd) {
			if (go.Backsies() == false)
			{
				return;
			}

			if (my_socket.status() == MySocket::Status::Server)
			{
				Command_MSG message{ COMMAND, BACKSIES };
				my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
			}
		}},
		{INIT, [this](HWND hWnd) {
			if (go.Init() == false)
			{
				return;
			}

			if (my_socket.status() == MySocket::Status::Server)
			{
				Command_MSG message{ COMMAND,INIT };
				my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
			}
		}},
		{PASS, [this](HWND hWnd) {
			if (go.Pass() == false)
			{
				return;
			}

			if (my_socket.status() == MySocket::Status::Server)
			{
				Command_MSG message{ COMMAND, PASS };
				my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
			}
		}},
	} {
	go.Init();
}

void GoWinApplication::set_main_window_handle(HWND handle)
{
	m_main_window_handle = handle;
}

void GoWinApplication::set_hInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

HINSTANCE GoWinApplication::hInstance()
{
	return m_hInstance;
}

HWND GoWinApplication::main_window_handle()
{
	return m_main_window_handle;
}

LRESULT GoWinApplication::main_procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		create(hWnd);
		return 0;

	case WM_MOUSEMOVE:
		GetMouseCoord(mouse, lParam);
		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case WM_LBUTTONDOWN:
		GetMouseCoord(mouse, lParam);
		if (board_graphic.IsMouseInBoard(mouse))
		{
			const Coord2d placement_point = board_graphic.MouseToBoard(mouse.x, mouse.y);
			if (my_socket.status() == MySocket::Status::Client)
			{
				Placement_MSG message{ PLACEMENT, go.info().sequence() + 1, placement_point.x, placement_point.y };
				my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
				break;
			}

			int errorMSG = go.Placement(placement_point);
			if (errorMSG == 0)
			{
				if (my_socket.status() == MySocket::Status::Server)
				{
					const auto& placementInfo = go.getLastPlacementInfo();
					Placement_MSG message{ PLACEMENT, placementInfo.sequence, placementInfo.placement.x, placementInfo.placement.y };
					my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
				}
				SetWindowText(hBCS, std::to_wstring(go.info().get_player(Color::Black).captured_stone()).c_str());
				SetWindowText(hWCS, std::to_wstring(go.info().get_player(Color::White).captured_stone()).c_str());
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (errorMSG != ERR_NOTEMPTY)
			{
				MessageBox(hWnd, ERROR_MESSAGES[errorMSG], _T("ERROR"), MB_OK);
			}
		}

		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
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
		{
			OPENFILENAME SFN;
			WCHAR lpstrFile[MAX_PATH] = _T("");
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hWnd;
			SFN.lpstrFilter = _T("기보 파일(*.ngf)\0*.ngf\0기보 파일(*.sgf)\0*.sgf\0모든 파일(*.*)\0*.*\0");
			SFN.lpstrDefExt = _T("ngf");
			SFN.lpstrFile = lpstrFile;
			SFN.nMaxFile = 256;
			SFN.Flags = OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&SFN) != 0)
			{
				std::wstring extension = lpstrFile;		//확장자 추출하기
				extension = extension.substr(extension.length() - 3, 3);
				go.Save(lpstrFile, extension);
			}

			break;
		}
		break;

		case IDM_FILE_EXIT:
			// 콘솔 닫기
			FreeConsole();
			//

			DeleteDC(hdc);
			DeleteDC(hdcMem);

			// PostQuitMessage(0);
			 //break;

			DestroyWindow(hWnd);
			break;
		case IDM_SERVER_CREATE:
			if (my_socket.Create(hWnd) != INVALID_SOCKET) {
				send_text_to_chat(_T("[System] 서버 생성"));
				HMENU hMenu = GetMenu(hWnd);
				HMENU hSubMenu = GetSubMenu(hMenu, 1);
				EnableMenuItem(hMenu, GetMenuItemID(hSubMenu, 0), MF_GRAYED);

			}
			else {
				send_text_to_chat(_T("[System] 서버 생성 실패"));
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

		case 3:	// 수순 표시
			board_graphic.TogglePrintSequence();
			InvalidateRect(hWnd, NULL, FALSE);
			SetFocus(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_ASYNC:
		switch (WSAGETSELECTEVENT(lParam))
		{
			//
			// 클라이언트의 접속요청이 있을때
		case FD_ACCEPT:
			if (my_socket.FD_Accept()) {
				send_text_to_chat(_T("클라이언트 접속 !!"));
			}
			else
			{
				send_text_to_chat(_T("클라이언트 접속요청수락 실패 !!"));
			}
			break;

			//
			// 클라이언트가 채팅메시지를 보내왔을때
		case FD_READ:
		{
			COMM_MSG msg;
			my_socket.FD_Read((SOCKET)wParam, &msg);
			switch (msg.type)
			{
			case CHATTING:
			{
				CHAT_MSG* chat_msg = reinterpret_cast<CHAT_MSG*>(&msg);
				append_text(hChatBox, _T("상대 : "));
				send_text_to_chat(chat_msg->buf);
				break;
			}
			case PLACEMENT:
			{
				Placement_MSG* placement_msg = (Placement_MSG*)&msg;
				int errorMSG = go.Placement(Coord2d(placement_msg->x, placement_msg->y));
				if (errorMSG == 0)
				{
					if (my_socket.status() == MySocket::Status::Server)
					{
						const auto& placementInfo = go.getLastPlacementInfo();
						Placement_MSG message{ PLACEMENT, placementInfo.sequence, placementInfo.placement.x, placementInfo.placement.y };
						my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
					}
					SetWindowText(hBCS, std::to_wstring(go.info().get_player(Color::Black).captured_stone()).c_str());
					SetWindowText(hWCS, std::to_wstring(go.info().get_player(Color::White).captured_stone()).c_str());
					InvalidateRect(hWnd, NULL, FALSE);
				}
				break;
			}
			case COMMAND:
			{
				command_message_callbacks[reinterpret_cast<Command_MSG*>(&msg)->command](hWnd);
				InvalidateRect(hWnd, NULL, FALSE);
			}

			default:
				break;
			}

		}
		break;

		// 클라이언트가 접속을 해제했을때
		case FD_CLOSE:
			my_socket.FD_Close((SOCKET)wParam);
			send_text_to_chat(_T("클라이언트 연결 해제"));
			break;
		}
		return TRUE;

	case WM_KEYDOWN:
		//cout << wParam <<"키 눌림" << endl;
		switch (wParam)
		{
		case KEY_ENTER:
			if (GetWindowTextLength(hChatInputBox) != 0)
			{
				append_text(hChatBox, _T("당신 : "));
				WCHAR buffer[512];
				GetWindowText(hChatInputBox, buffer, 512);
				send_text_to_chat(buffer);
				SetWindowText(hChatInputBox, _T(""));

				if (my_socket.IsConnected())
				{
					CHAT_MSG msg;
					msg.type = CHATTING;
					wcscpy_s(msg.buf, buffer);

					if (SOCKET_ERROR == my_socket.Send((char*)&msg, BUFSIZE))
						send_text_to_chat(_T("[System] 메세지 보내기 실패"));
				}
			}
			SetFocus(hChatInputBox);

			break;
		}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdcMem, 0, 0, 1200, 820, hdc_BackGround, 0, 0, SRCCOPY);
		board_graphic.Draw(hdcMem, go, mouse);

		BitBlt(hdc, 0, 0, 1200, 820, hdcMem, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		DeleteObject(hBitmapMem); //-3
		DeleteDC(hdcMem); //-2

		DeleteDC(hdc_BackGround);
		board_graphic.Release();

		ReleaseDC(hWnd, hdc);

		PostQuitMessage(0);

		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;;
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

void GoWinApplication::create(HWND hWnd)
{
#ifdef _DEBUG	// 콘솔창 열기
	AllocConsole();
	FILE* fp = NULL;
	_wfreopen_s(&fp, _T("CONOUT$"), _T("wt"), stdout);
#endif
	hdc = GetDC(hWnd);

	board_graphic.SetWidth(806);
	board_graphic.SetHeight(806);
	board_graphic.SetLeftTopPoint({ 0, 0 });
	board_graphic.SetSpaceSize(42);
	board_graphic.SetBorderSize(6);
	board_graphic.Init(hdc, m_hInstance);

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

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN, _T("궁서"));
	SendMessage(hBCS, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);
	SendMessage(hWCS, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);

	CreateWindow(_T("button"), _T("무르기(Z)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		840, 300, 150, 30, hWnd, (HMENU)IDA_BACKSIES, m_hInstance, NULL);
	CreateWindow(_T("button"), _T("초기화(I)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1010, 300, 150, 30, hWnd, (HMENU)IDA_INIT, m_hInstance, NULL);
	CreateWindow(_T("button"), _T("한수쉼(P)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		840, 340, 150, 30, hWnd, (HMENU)IDA_PASS, m_hInstance, NULL);
	CreateWindow(_T("button"), _T("수순 표시"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1010, 340, 150, 30, hWnd, (HMENU)3, m_hInstance, NULL);

	hChatBox = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL | ES_MULTILINE,
		840, 400, 320, 120, hWnd, (HMENU)4, m_hInstance, NULL);
	hChatInputBox = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN,
		840, 530, 320, 30, hWnd, (HMENU)5, m_hInstance, NULL);
}

void GoWinApplication::file_open(HWND hWnd)
{
	try
	{
		GoWin::OpenFileName ofn(hWnd, _T("기보 파일(*.ngf)\0*.ngf\0"), OFN_FILEMUSTEXIST);

		auto result = ofn.open();
		if (result.success == false)
		{
			MessageBox(hWnd, strings[string_id::FILE_OPEN_FAIL].c_str(), strings[string_id::FILE_OPEN_FAIL_TITLE].c_str() , MB_OK);
			return;
		}

		std::wstring extension = get_extension(result.file_path);
		if (extension != _T(".ngf"))
		{
			MessageBox(hWnd, strings[string_id::INVALID_EXTENSION].c_str(), strings[string_id::FILE_OPEN_FAIL_TITLE].c_str(), MB_OK);
			return;
		}

		std::wifstream file(result.file_path);
		file.imbue(std::locale(""));

		GiboNGF gibo;
		gibo.load(file);

		file.close();

		go.Load(gibo);
	}
	catch (const std::exception&)
	{
		MessageBox(hWnd, strings[string_id::FILE_OPEN_FAIL].c_str(), strings[string_id::FILE_OPEN_FAIL_TITLE].c_str(), MB_OK);
	}
}

void GoWinApplication::backsies(HWND hWnd)
{
	if (my_socket.status() == MySocket::Status::Client)
	{
		Command_MSG message{ COMMAND, BACKSIES };
		my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
		return;
	}

	if (go.Backsies() == false)
	{
		return;
	}

	if (my_socket.status() == MySocket::Status::Server)
	{
		Command_MSG message{ COMMAND, BACKSIES };
		my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
	}

	InvalidateRect(hWnd, NULL, FALSE);
}

void GoWinApplication::init(HWND hWnd)
{
	if (my_socket.status() == MySocket::Status::Client)
	{
		Command_MSG message{ COMMAND, INIT };
		my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
		return;
	}

	if (go.Init() == false)
	{
		return;
	}

	if (my_socket.status() == MySocket::Status::Server)
	{
		Command_MSG message{ COMMAND, INIT };
		my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
	}

	InvalidateRect(hWnd, NULL, FALSE);
	send_text_to_chat(_T("[System] 바둑판을 초기화 했습니다."));
}

void GoWinApplication::pass(HWND hWnd)
{
	if (my_socket.status() == MySocket::Status::Client)
	{
		Command_MSG message{ COMMAND, PASS };
		my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
		return;
	}

	if (go.Pass() == false)
	{
		return;
	}

	if (my_socket.status() == MySocket::Status::Server)
	{
		Command_MSG message{ COMMAND, PASS };
		my_socket.Send(reinterpret_cast<char*>(&message), BUFSIZE);
	}

	InvalidateRect(hWnd, NULL, FALSE);
	send_text_to_chat(_T("[System] 한수 쉬었습니다."));
}

void GoWinApplication::append_text(HWND editbox, LPCTSTR text)
{
	SendMessage(editbox, EM_SETSEL, 0, -1);
	SendMessage(editbox, EM_SETSEL, -1, -1);
	SendMessage(editbox, EM_REPLACESEL, 0, (LPARAM)text); // append!
}

void GoWinApplication::send_text_to_chat(LPCTSTR text)
{
	append_text(hChatBox, text);
	append_text(hChatBox, _T("\r\n"));
}

std::wstring GoWinApplication::get_extension(const std::wstring& path)
{
	std::wstring extension = std::filesystem::path(path).extension();
	std::transform(extension.begin(), extension.end(), extension.begin(), std::towlower);

	return extension;
}
