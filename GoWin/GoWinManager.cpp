#include "GoWinManager.h"
#include "resource.h"

HRESULT GoWinManager::Init()
{
	bitmaps = HDCManager(5);
	// 콘솔창 열기
	AllocConsole();
	FILE* fp;
	_wfreopen_s(&fp, _T("CONOUT$"), _T("wt"), stdout);
	//

	HBITMAP bitBlackStone, bitWhiteStone, bitBackGround, bitBoard;
	bitBlackStone = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLACKSTONE));
	bitWhiteStone = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WHITESTONE));
	bitBackGround = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUND));
	bitBoard = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOARD));

	bitmaps.AddBitmap(hdc, hWnd, EBitmapName::BlackStone, bitBlackStone);
	bitmaps.AddBitmap(hdc, hWnd, EBitmapName::WhiteStone, bitWhiteStone);
	bitmaps.AddBitmap(hdc, hWnd, EBitmapName::BackGround, bitBackGround);
	bitmaps.AddBitmap(hdc, hWnd, EBitmapName::Board, bitBoard);

	hdc = GetDC(hWnd);

	hdcMem = CreateCompatibleDC(hdc);

	ReleaseDC(hWnd, hdc);

	hBCS = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
		840, 200, 50, 30, hWnd, (HMENU)1, hInst, NULL);
	hWCS = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
		1010, 200, 50, 30, hWnd, (HMENU)2, hInst, NULL);

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN, _T("궁서"));
	SendMessage(hBCS, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);
	SendMessage(hWCS, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);

	CreateWindow(_T("button"), _T("무르기(Z)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		840, 300, 150, 30, hWnd, (HMENU)IDA_BACKSIES, hInst, NULL);
	CreateWindow(_T("button"), _T("초기화(I)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1010, 300, 150, 30, hWnd, (HMENU)IDA_INIT, hInst, NULL);
	CreateWindow(_T("button"), _T("한수쉼(P)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		840, 340, 150, 30, hWnd, (HMENU)IDA_PASS, hInst, NULL);
	CreateWindow(_T("button"), _T("수순 표시"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1010, 340, 150, 30, hWnd, (HMENU)3, hInst, NULL);

	hChatBox = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL | ES_MULTILINE,
		840, 400, 320, 120, hWnd, (HMENU)4, hInst, NULL);
	hChatInputBox = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN,
		840, 530, 320, 30, hWnd, (HMENU)5, hInst, NULL);
	return S_OK;
}

Go& GoWinManager::GetGame()
{
	return game;
}

bool GoWinManager::GetPrintSequenceSwitch()
{
	return printSequanceSwitch;
}

void GoWinManager::SetPrintSequenceSwitch(bool b)
{
	printSequanceSwitch = b;
}

void GoWinManager::FileOpen()
{
	tagOFNW OFN;
	WCHAR lpstrFile[MAX_PATH] = _T("");
	WCHAR str[256];
	memset(&OFN, 0, sizeof(tagOFNW));
	OFN.lStructSize = sizeof(tagOFNW);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = _T("기보 파일(*.ngf)\0*.ngf\0모든 파일(*.*)\0*.*\0");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 256;
	if (GetOpenFileName(&OFN) != 0)
	{
		wsprintf(str, _T("%s 파일을 선택했습니다."), lpstrFile);

		wstring extension = lpstrFile;		//확장자 추출하기
		extension = extension.substr(extension.length() - 3, 3);
		if (extension == _T("NGF") || extension == _T("ngf")) //확장자 NGF
		{
			GiboNGF gibo(lpstrFile);
			game.Load(gibo);

			InvalidateRect(g_hWnd, NULL, FALSE);
		}
		else
			MessageBox(g_hWnd, _T("지원되는 기보 파일이 아닙니다."), _T("열기 실패"), MB_OK);
	}
}

void GoWinManager::FileSave()
{
	tagOFNW SFN;
	WCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&SFN, 0, sizeof(tagOFNW));
	SFN.lStructSize = sizeof(tagOFNW);
	SFN.hwndOwner = g_hWnd;
	SFN.lpstrFilter = _T("기보 파일(*.ngf)\0*.ngf\0기보 파일(*.sgf)\0*.sgf\0모든 파일(*.*)\0*.*\0");
	SFN.lpstrDefExt = _T("ngf");
	SFN.lpstrFile = lpstrFile;
	SFN.nMaxFile = 256;
	SFN.Flags = OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&SFN) != 0)
	{
		wstring extension = lpstrFile;		//확장자 추출하기
		extension = extension.substr(extension.length() - 3, 3);
		game.Save(lpstrFile, extension);
	}
}

void GoWinManager::DrawStone(HDC hdc, Stone stone)
{
	int x = stone.x();
	int y = stone.y();
	Color color = stone.color();

	if (color == Color::Black)
	{
		BitBlt(hdc, SPACE_SIZE * (x - 1) + 6, SPACE_SIZE * (y - 1) + 6, 39, 39, bitmaps[BlackStone], 0, 0, SRCCOPY);
		SetTextColor(hdc, RGB(255, 255, 255));
	}
	else if (color == Color::White)
	{
		BitBlt(hdc, SPACE_SIZE * (x - 1) + 6, SPACE_SIZE * (y - 1) + 6, 39, 39, bitmaps[WhiteStone], 0, 0, SRCCOPY);
		SetTextColor(hdc, RGB(0, 0, 0));
	}
}

void GoWinManager::DrawBoard(HDC hdc)
{
	BitBlt(hdc, 0, 0, 806, 806, bitmaps[Board], 0, 0, SRCCOPY);

	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, TRANSPARENT);
	for (int x = 1; x < 20; x++)
	{
		for (int y = 1; y < 20; y++)
		{
			DrawStone(hdc, game.Read({ x, y }));

			int sqc = game.Read({ x, y }).sequence();
			if (sqc != 0 && printSequenceSwitch == true)
			{
				TextOut(hdc, SPACE_SIZE * (x - 1) + 25, SPACE_SIZE * (y - 1) + 18, std::to_wstring(sqc).c_str(), (int)std::to_wstring(sqc).length());
			}
		}
	}

	if (boardInfo.IsMouseInBoard(mouse))
	{
		Coord2d board_point = boardInfo.MouseToBoard(mouse.x, mouse.y);;
		if (game.Read(board_point).color() == Color::Null)
		{
			BLENDFUNCTION bf;
			bf.AlphaFormat = AC_SRC_ALPHA;
			bf.BlendFlags = 0;
			bf.BlendOp = 0;
			bf.SourceConstantAlpha = 180;

			if (Stone::Sqnce2color(game.info().sequence()) == Color::Black)
				GdiAlphaBlend(hdc, SPACE_SIZE * (board_point.x - 1) + 6, SPACE_SIZE * (board_point.y - 1) + 6, 39, 39, bitmaps[BlackStone], 0, 0, 39, 39, bf);
			else
				GdiAlphaBlend(hdc, SPACE_SIZE * (board_point.x - 1) + 6, SPACE_SIZE * (board_point.y - 1) + 6, 39, 39, bitmaps[WhiteStone], 0, 0, 39, 39, bf);
		}
	}
}

LRESULT GoWinManager::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	/*char buffer[64] = "";
	if(message == WM_PAINT)
	   printf("hWnd : %p\t msg : %-15s wParam : %d\t lParam : %d\n", hWnd, Read(message, buffer), wParam, lParam);
	*/
	switch (message)
	{
	case WM_CREATE:
		manager.Init();
		return 0;

	case WM_MOUSEMOVE:
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);

		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case WM_LBUTTONDOWN:
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		//cout << "마우스 좌표 x" << mouse.x << endl;
		//cout << "마우스 좌표 y" << mouse.y << endl;
		if (boardInfo.IsMouseInBoard(mouse))
		{
			//cout << "보드 안에 있음" << endl;
			Coord2d placement_point = boardInfo.MouseToBoard(mouse.x, mouse.y);

			int errorMSG = manager.GetGame().Placement(placement_point);

			if (errorMSG == 0) {
				SetWindowText(hBCS, to_wstring(manager.GetGame().info().black_player().captured_stone()).c_str());
				SetWindowText(hWCS, to_wstring(manager.GetGame().info().white_player().captured_stone()).c_str());
				InvalidateRect(hWnd, NULL, FALSE);
				if (mysocket.Status() != SocketStatus::notConnected)
				{
					PlacementInfo placementInfo = manager.GetGame().getLastPlacementInfo();
					Placement_MSG msg;
					msg.type = PLACEMENT;
					msg.sequence = placementInfo.sequence;
					msg.x = placementInfo.placment.x;
					msg.y = placementInfo.placment.y;

					mysocket.Send((char*)&msg, BUFSIZE);
				}
			}
			else if (errorMSG != ERR_NOTEMPTY)
				MessageBox(hWnd, errorMSG_wchar[errorMSG], _T("ERROR"), MB_OK);
		}
		else {
			// Game.info().placement().print();
		}

		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_FILE_OPEN:
			manager.FileOpen();
			break;

			// 메뉴 - 파일 - 저장
		case IDM_FILE_SAVE:
			manager.FileSave();
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
			if (mysocket.Create(hWnd) != INVALID_SOCKET)
			{
				SendTextEdit(hChatBox, _T("[System] 서버 생성"));
				// 서버 생성 비활성화
				HMENU hMenu = GetMenu(hWnd);
				HMENU hSubMenu = GetSubMenu(hMenu, 1);
				EnableMenuItem(hMenu, GetMenuItemID(hSubMenu, 0), MF_GRAYED);
			}
			else {
				SendTextEdit(hChatBox, _T("[System] 서버 생성 실패"));
			}
			break;
		case IDM_SERVER_ENTER:
			DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_NETBOX), hWnd, Netbox);
			break;
		case IDM_ABOUT:
			DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDA_BACKSIES:	//무르기 버튼
			if (manager.GetGame().Backsies())
			{
				if (mysocket.Status() != SocketStatus::notConnected)
				{
					Command_MSG msg;
					msg.type = COMMAND;
					msg.command = BACKSIES;

					mysocket.Send((char*)&msg, BUFSIZE);
				}
				InvalidateRect(hWnd, NULL, FALSE);
			}
			SetFocus(hWnd);
			break;

		case IDA_INIT:	//초기화 버튼
			if (manager.GetGame().Init())
			{
				if (mysocket.Status() != SocketStatus::notConnected)
				{
					Command_MSG msg;
					msg.type = COMMAND;
					msg.command = INIT;

					mysocket.Send((char*)&msg, BUFSIZE);
				}
				SetWindowText(hBCS, to_wstring(manager.GetGame().info().black_player().captured_stone()).c_str());
				SetWindowText(hWCS, to_wstring(manager.GetGame().info().white_player().captured_stone()).c_str());
				InvalidateRect(hWnd, NULL, FALSE);
				SendTextEdit(hChatBox, _T("[System] 초기화 했습니다."));
				//MessageBox(hWnd, _T("초기화 했습니다."), _T("알림"), MB_OK);
			}
			SetFocus(hWnd);
			break;

		case IDA_PASS:	//한수쉼 버튼
			if (manager.GetGame().Pass())
			{
				if (mysocket.Status() != SocketStatus::notConnected)
				{
					Command_MSG msg;
					msg.type = COMMAND;
					msg.command = PASS;

					mysocket.Send((char*)&msg, BUFSIZE);
				}
				InvalidateRect(hWnd, NULL, FALSE);
				SendTextEdit(hChatBox, _T("[System] 한수 쉬었습니다."));
			}
			SetFocus(hWnd);
			break;

		case 3:	// 수순 표시
			manager.SetPrintSequenceSwitch(!manager.GetPrintSequenceSwitch());
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
			if (mysocket.FD_Accept()) {
				SendTextEdit(hChatBox, _T("클라이언트 접속 !!"));
			}
			else
			{
				SendTextEdit(hChatBox, _T("클라이언트 접속요청수락 실패 !!"));
			}
			break;

			//
			// 클라이언트가 채팅메시지를 보내왔을때
		case FD_READ:
		{
			COMM_MSG msg;
			mysocket.FD_Read((SOCKET)wParam, &msg);
			switch (msg.type)
			{
			case CHATTING:
			{
				CHAT_MSG* chat_msg = (CHAT_MSG*)&msg;
				wstring chatMSG = _T("상대 : ");
				chatMSG.append(chat_msg->buf);
				SendTextEdit(hChatBox, chatMSG.c_str());
				break;
			}
			case PLACEMENT:
			{
				Placement_MSG* placement_msg = (Placement_MSG*)&msg;
				//cout << "착수 통신 테스트" << endl;
				//PlacementInfo info = { placement_msg->sequence, Color::Black , { placement_msg->x, placement_msg->y } };
				//print_data(info);
				Coord2d point = { placement_msg->x, placement_msg->y };
				manager.GetGame().Placement(point);
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			}
			case COMMAND:
			{
				Command_MSG* commandMsg = (Command_MSG*)&msg;
				switch (commandMsg->command)
				{
				case BACKSIES:
					manager.GetGame().Backsies();
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				case INIT:
					manager.GetGame().Init();
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				case PASS:
					manager.GetGame().Pass();
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				default:
					break;
				}
			}

			default:
				break;
			}

		}
		break;

		//
		// 클라이언트가 접속을 해제했을때
		case FD_CLOSE:
			mysocket.FD_Close((SOCKET)wParam);
			SendTextEdit(hChatBox, _T("클라이언트 연결 해제"));
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
				WCHAR buffer[512];
				GetWindowText(hChatInputBox, buffer, 512);

				wstring newText = _T("당신 : ");
				newText.append(buffer);

				SendTextEdit(hChatBox, newText.c_str());

				SetWindowTextW(hChatInputBox, _T(""));

				if (mysocket.Status() != SocketStatus::notConnected)
				{
					CHAT_MSG msg;
					msg.type = CHATTING;
					wcscpy(msg.buf, buffer);

					if (SOCKET_ERROR == mysocket.Send((char*)&msg, BUFSIZE))
						SendTextEdit(hChatBox, _T("[System] 메세지 보내기 실패"));
				}
			}
			SetFocus(hChatInputBox);

			break;
		}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		hbmMem = CreateCompatibleBitmap(hdc, 1200, 820);//3
		hbmMemOld = (HBITMAP)SelectObject(hdcMem, hbmMem);//4

		BitBlt(hdcMem, 0, 0, 1200, 820, bitmaps[EBitmapName::BackGround], 0, 0, SRCCOPY);

		manager.DrawBoard(hdcMem);

		BitBlt(hdc, 0, 0, 1200, 820, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hbmMemOld); //-4
		DeleteObject(hbmMem); //-3

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		DeleteDC(hdcMem);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
