#include "GoWinManager.h"
#include "PlacementInfo.h"
#include "BoardGraphic.h"
#include "GiboNGF.h"
#include "Player.h"
#include "Stone.h"
#include "Go.h"

INT_PTR CALLBACK    Netbox(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void SendTextEdit(HWND edit, LPCWSTR pText);

HRESULT GoWinManager::init(HINSTANCE hInstance, HWND hWnd)
{
	ImageManager::GetSingleton()->Init();

	if (m_game == nullptr) 
		m_game = new Go();
	m_game->init();

	if(boardInfo == nullptr)
		boardInfo = new BoardGraphic();
	boardInfo->init({ 0, 0 }, 806, 806, SPACE_SIZE, 6);

	// �ܼ�â ����
	AllocConsole();
	FILE* fp;
	_wfreopen_s(&fp, _T("CONOUT$"), _T("wt"), stdout);
	//

	ImageManager::GetSingleton()->AddImage("BlackStone", IDB_BLACKSTONE, 39, 39);
	ImageManager::GetSingleton()->AddImage("WhiteStone", IDB_WHITESTONE, 39, 39);
	ImageManager::GetSingleton()->AddImage("BackGround", IDB_BACKGROUND, 1200, 820);
	ImageManager::GetSingleton()->AddImage("Board", IDB_BOARD, 806, 806);

	board = ImageManager::GetSingleton()->FindImage("Board");
	background = ImageManager::GetSingleton()->FindImage("BackGround");

	hdc = GetDC(hWnd);

	hdcMem = CreateCompatibleDC(hdc);

	ReleaseDC(hWnd, hdc);

	hBCS = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
		840, 200, 50, 30, hWnd, (HMENU)1, g_hInstance, NULL);
	hWCS = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
		1010, 200, 50, 30, hWnd, (HMENU)2, g_hInstance, NULL);

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN, _T("�ü�"));
	SendMessage(hBCS, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);
	SendMessage(hWCS, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);

	CreateWindow(_T("button"), _T("������(Z)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		840, 300, 150, 30, hWnd, (HMENU)IDA_BACKSIES, hInstance, NULL);
	CreateWindow(_T("button"), _T("�ʱ�ȭ(I)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1010, 300, 150, 30, hWnd, (HMENU)IDA_INIT, hInstance, NULL);
	CreateWindow(_T("button"), _T("�Ѽ���(P)"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		840, 340, 150, 30, hWnd, (HMENU)IDA_PASS, hInstance, NULL);
	CreateWindow(_T("button"), _T("���� ǥ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1010, 340, 150, 30, hWnd, (HMENU)3, hInstance, NULL);

	hChatBox = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL | ES_MULTILINE,
		840, 400, 320, 120, hWnd, (HMENU)4, hInstance, NULL);
	hChatInputBox = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN,
		840, 530, 320, 30, hWnd, (HMENU)5, hInstance, NULL);

	is_init = true;
	return S_OK;
}

void GoWinManager::release()
{
	SAFE_RELEASE(m_game);
	SAFE_RELEASE(boardInfo);

	ImageManager::GetSingleton()->Release();
}

void GoWinManager::render(HDC hdc)
{
	board->Render(hdc);

	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, TRANSPARENT);
	for (int x = 1; x < 20; x++)
	{
		for (int y = 1; y < 20; y++)
		{
			Stone* stone = m_game->Read({ x, y });

			stone->render(hdc);
			int sqc = stone->sequence();
			if (sqc != 0 && printSequenceSwitch == true)
			{
				TextOut(hdc, SPACE_SIZE * (x - 1) + 25, SPACE_SIZE * (y - 1) + 18, std::to_wstring(sqc).c_str(), (int)std::to_wstring(sqc).length());
			}
		}
	}

	if (boardInfo->IsMouseInBoard(mouse))
	{
		Coord2d board_point = boardInfo->MouseToBoard(mouse.x, mouse.y);
		if (m_game->Read(board_point)->color() == Color::Null)
		{
			BLENDFUNCTION bf;
			bf.AlphaFormat = AC_SRC_ALPHA;
			bf.BlendFlags = 0;
			bf.BlendOp = 0;
			bf.SourceConstantAlpha = 180;

			if (Stone::Sqnce2color(m_game->info()->sequence()) == Color::Black)
				ImageManager::GetSingleton()->FindImage("BlackStone")->Render(hdc, SPACE_SIZE * (board_point.x - 1) + 6, SPACE_SIZE * (board_point.y - 1) + 6);
			//GdiAlphaBlend(hdc, SPACE_SIZE * (board_point.x - 1) + 6, SPACE_SIZE * (board_point.y - 1) + 6, 39, 39, (*bitmaps)[BlackStone], 0, 0, 39, 39, bf);
			else
				ImageManager::GetSingleton()->FindImage("WhiteStone")->Render(hdc, SPACE_SIZE * (board_point.x - 1) + 6, SPACE_SIZE * (board_point.y - 1) + 6);
			//GdiAlphaBlend(hdc, SPACE_SIZE * (board_point.x - 1) + 6, SPACE_SIZE * (board_point.y - 1) + 6, 39, 39, (*bitmaps)[WhiteStone], 0, 0, 39, 39, bf);
		}
	}
}

bool GoWinManager::GetPrintSequenceSwitch()
{
	return printSequenceSwitch;
}


void GoWinManager::SetPrintSequenceSwitch(bool b)
{
	printSequenceSwitch = b;
}

void GoWinManager::FileOpen()
{
	tagOFNW OFN;
	WCHAR lpstrFile[MAX_PATH] = _T("");
	WCHAR str[256];
	memset(&OFN, 0, sizeof(tagOFNW));
	OFN.lStructSize = sizeof(tagOFNW);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = _T("�⺸ ����(*.ngf)\0*.ngf\0��� ����(*.*)\0*.*\0");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 256;
	if (GetOpenFileName(&OFN) != 0)
	{
		wsprintf(str, _T("%s ������ �����߽��ϴ�."), lpstrFile);

		wstring extension = lpstrFile;		//Ȯ���� �����ϱ�
		extension = extension.substr(extension.length() - 3, 3);
		if (extension == _T("NGF") || extension == _T("ngf")) //Ȯ���� NGF
		{
			GiboNGF* gibo = new GiboNGF();
			gibo->init(lpstrFile);

			m_game->Load(gibo);

			gibo->release();
			delete gibo;
			InvalidateRect(g_hWnd, NULL, FALSE);
		}
		else
			MessageBox(g_hWnd, _T("�����Ǵ� �⺸ ������ �ƴմϴ�."), _T("���� ����"), MB_OK);
	}
}

void GoWinManager::FileSave()
{
	tagOFNW SFN;
	WCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&SFN, 0, sizeof(tagOFNW));
	SFN.lStructSize = sizeof(tagOFNW);
	SFN.hwndOwner = g_hWnd;
	SFN.lpstrFilter = _T("�⺸ ����(*.ngf)\0*.ngf\0�⺸ ����(*.sgf)\0*.sgf\0��� ����(*.*)\0*.*\0");
	SFN.lpstrDefExt = _T("ngf");
	SFN.lpstrFile = lpstrFile;
	SFN.nMaxFile = 256;
	SFN.Flags = OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&SFN) != 0)
	{
		wstring extension = lpstrFile;		//Ȯ���� �����ϱ�
		extension = extension.substr(extension.length() - 3, 3);
		m_game->Save(lpstrFile, extension);
	}
}

LRESULT GoWinManager::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	/*char buffer[64] = "";
	if(message == WM_PAINT)
	   printf("hWnd : %p\t msg : %-15s wParam : %d\t lParam : %d\n", hWnd, Read(message, buffer), wParam, lParam);
	*/
	switch (iMessage)
	{
	case WM_CREATE:
		init(g_hInstance, hWnd);
		return 0;

	case WM_MOUSEMOVE:
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);

		InvalidateRect(hWnd, NULL, FALSE);

		break;

	case WM_LBUTTONDOWN:
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		//cout << "���콺 ��ǥ x" << mouse.x << endl;
		//cout << "���콺 ��ǥ y" << mouse.y << endl;
		if (boardInfo->IsMouseInBoard(mouse))
		{
			//cout << "���� �ȿ� ����" << endl;
			Coord2d placement_point = boardInfo->MouseToBoard(mouse.x, mouse.y);

			int errorMSG = m_game->Placement(placement_point);

			if (errorMSG == 0) {
				SetWindowText(hBCS, to_wstring(m_game->info()->black_player()->captured_stone()).c_str());
				SetWindowText(hWCS, to_wstring(m_game->info()->white_player()->captured_stone()).c_str());
				InvalidateRect(hWnd, NULL, FALSE);
				if (mysocket.Status() != SocketStatus::notConnected)
				{
					PlacementInfo* placementInfo = m_game->getLastPlacementInfo();
					Placement_MSG msg;
					msg.type = PLACEMENT;
					msg.sequence = placementInfo->sequence();
					msg.x = placementInfo->coord().x;
					msg.y = placementInfo->coord().y;

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
		// �޴� ������ ���� �м��մϴ�:
		switch (wmId)
		{
		case IDM_FILE_OPEN:
			FileOpen();
			break;

			// �޴� - ���� - ����
		case IDM_FILE_SAVE:
			FileSave();
			break;

		case IDM_FILE_EXIT:
			// �ܼ� �ݱ�
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
				SendTextEdit(hChatBox, _T("[System] ���� ����"));
				// ���� ���� ��Ȱ��ȭ
				HMENU hMenu = GetMenu(hWnd);
				HMENU hSubMenu = GetSubMenu(hMenu, 1);
				EnableMenuItem(hMenu, GetMenuItemID(hSubMenu, 0), MF_GRAYED);
			}
			else {
				SendTextEdit(hChatBox, _T("[System] ���� ���� ����"));
			}
			break;
		case IDM_SERVER_ENTER:
			DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_NETBOX), hWnd, Netbox);
			break;
		case IDM_ABOUT:
			DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDA_BACKSIES:	//������ ��ư
			if (m_game->Backsies())
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

		case IDA_INIT:	//�ʱ�ȭ ��ư
			if (SUCCEEDED(m_game->init()))
			{
				if (mysocket.Status() != SocketStatus::notConnected)
				{
					Command_MSG msg;
					msg.type = COMMAND;
					msg.command = INIT;

					mysocket.Send((char*)&msg, BUFSIZE);
				}
				SetWindowText(hBCS, to_wstring(m_game->info()->black_player()->captured_stone()).c_str());
				SetWindowText(hWCS, to_wstring(m_game->info()->white_player()->captured_stone()).c_str());
				InvalidateRect(hWnd, NULL, FALSE);
				SendTextEdit(hChatBox, _T("[System] �ʱ�ȭ �߽��ϴ�."));
				//MessageBox(hWnd, _T("�ʱ�ȭ �߽��ϴ�."), _T("�˸�"), MB_OK);
			}
			SetFocus(hWnd);
			break;

		case IDA_PASS:	//�Ѽ��� ��ư
			if (m_game->Pass())
			{
				if (mysocket.Status() != SocketStatus::notConnected)
				{
					Command_MSG msg;
					msg.type = COMMAND;
					msg.command = PASS;

					mysocket.Send((char*)&msg, BUFSIZE);
				}
				InvalidateRect(hWnd, NULL, FALSE);
				SendTextEdit(hChatBox, _T("[System] �Ѽ� �������ϴ�."));
			}
			SetFocus(hWnd);
			break;

		case 3:	// ���� ǥ��
			SetPrintSequenceSwitch(!GetPrintSequenceSwitch());
			InvalidateRect(hWnd, NULL, FALSE);
			SetFocus(hWnd);
			break;

		default:
			return DefWindowProc(hWnd, iMessage, wParam, lParam);
		}
	}
	break;

	case WM_ASYNC:
		switch (WSAGETSELECTEVENT(lParam))
		{
			//
			// Ŭ���̾�Ʈ�� ���ӿ�û�� ������
		case FD_ACCEPT:
			if (mysocket.FD_Accept()) {
				SendTextEdit(hChatBox, _T("Ŭ���̾�Ʈ ���� !!"));
			}
			else
			{
				SendTextEdit(hChatBox, _T("Ŭ���̾�Ʈ ���ӿ�û���� ���� !!"));
			}
			break;

			//
			// Ŭ���̾�Ʈ�� ä�ø޽����� ����������
		case FD_READ:
		{
			COMM_MSG msg;
			mysocket.FD_Read((SOCKET)wParam, &msg);
			switch (msg.type)
			{
			case CHATTING:
			{
				CHAT_MSG* chat_msg = (CHAT_MSG*)&msg;
				wstring chatMSG = _T("��� : ");
				chatMSG.append(chat_msg->buf);
				SendTextEdit(hChatBox, chatMSG.c_str());
				break;
			}
			case PLACEMENT:
			{
				Placement_MSG* placement_msg = (Placement_MSG*)&msg;
				//cout << "���� ��� �׽�Ʈ" << endl;
				//PlacementInfo info = { placement_msg->sequence, Color::Black , { placement_msg->x, placement_msg->y } };
				//print_data(info);
				Coord2d point = { placement_msg->x, placement_msg->y };
				m_game->Placement(point);
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			}
			case COMMAND:
			{
				Command_MSG* commandMsg = (Command_MSG*)&msg;
				switch (commandMsg->command)
				{
				case BACKSIES:
					m_game->Backsies();
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				case INIT:
					m_game->init();
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				case PASS:
					m_game->Pass();
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
		// Ŭ���̾�Ʈ�� ������ ����������
		case FD_CLOSE:
			mysocket.FD_Close((SOCKET)wParam);
			SendTextEdit(hChatBox, _T("Ŭ���̾�Ʈ ���� ����"));
			break;
		}
		return TRUE;

	case WM_KEYDOWN:
		//cout << wParam <<"Ű ����" << endl;
		switch (wParam)
		{
		case KEY_ENTER:
			if (GetWindowTextLength(hChatInputBox) != 0)
			{
				WCHAR buffer[512];
				GetWindowText(hChatInputBox, buffer, 512);

				wstring newText = _T("��� : ");
				newText.append(buffer);

				SendTextEdit(hChatBox, newText.c_str());

				SetWindowTextW(hChatInputBox, _T(""));

				if (mysocket.Status() != SocketStatus::notConnected)
				{
					CHAT_MSG msg;
					msg.type = CHATTING;
					wcscpy(msg.buf, buffer);

					if (SOCKET_ERROR == mysocket.Send((char*)&msg, BUFSIZE))
						SendTextEdit(hChatBox, _T("[System] �޼��� ������ ����"));
				}
			}
			SetFocus(hChatInputBox);

			break;
		}

	case WM_PAINT:
	{
		if (!is_init)
			break;

		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);

		hbmMem = CreateCompatibleBitmap(hdc, 1200, 820);//3
		hbmMemOld = (HBITMAP)SelectObject(hdcMem, hbmMem);//4

		background->Render(hdcMem);

		render(hdcMem);

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
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

GoWinManager::GoWinManager() : m_game(nullptr), is_init(false)
{
}

GoWinManager::~GoWinManager()
{
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HWND hIpInputBox;
// ��Ƽ ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK Netbox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		hIpInputBox = GetDlgItem(hDlg, IDC_IP_INPUT);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CONNECT:
		{
			WCHAR buffer[64];
			GetWindowText(hIpInputBox, buffer, 64);
			MySocket::GetSingleton()->Enter(g_hWnd, WCharToChar(buffer));
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		default:
			break;
		}
	}
	return (INT_PTR)FALSE;
}

void SendTextEdit(HWND hEdit, LPCWSTR pText) {

	if (GetWindowTextLength(hEdit) > 100)
	{
		WCHAR buffer[50];
		GetWindowText(hEdit, buffer, 30);
		SendMessage(hEdit, EM_SETSEL, 0, (LPARAM)(wcschr(buffer, _T('\n')) - buffer + 1));
		SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)_T(""));
		// todo
	}
	wstring newText = pText;
	newText.append(_T("\r\n"));
	SendMessage(hEdit, EM_SETSEL, 0, -1);
	SendMessage(hEdit, EM_SETSEL, -1, -1);
	SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)newText.c_str()); // append!

}
