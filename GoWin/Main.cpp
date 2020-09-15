/*

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib,"msimg32.lib")

#include <windows.h>
#include <tchar.h>
#include "Go.h"
#include "resource.h"
#include "Mywmkey.h"
#include "GiboNGF.h"
#include "GiboSGF.h"
#include "BoardGraphic.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass = _T("�ٵ� by damas125");

HBITMAP hbmMem, hbmMemOld;
HDC hdc_BlackStone, hdc_WhiteStone, hdc_BackGround, hdc_Board;
HDC hdc, MemDC, hdcMem;

HWND hWnd;
HWND hBacksisButton;
int Player::player_number = 0;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);									// ����ü ũ��
	WndClass.style = CS_HREDRAW | CS_VREDRAW;							// ��� ����
	WndClass.lpfnWndProc = (WNDPROC)WndProc;							// ���ν��� �Լ�
	WndClass.cbClsExtra = 0;											// Ŭ���� ���� �޸�
	WndClass.cbWndExtra = 0;											// ������ ���� �޸�
	WndClass.hInstance = hInstance;										// ������ �ν��Ͻ�
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));	// ������
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);						// Ŀ��
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// ��� ��
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);					// �޴� �̸�
	WndClass.lpszClassName = lpszClass;									// Ŭ���� �̸�
	WndClass.hIconSm = NULL;											// ���� ������
	
	RegisterClassEx(&WndClass);

	hWnd = CreateWindowEx(WS_EX_ACCEPTFILES,lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000 + 16, 900 + 50 + 9,
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) 
	{
		if ((Message.message == WM_KEYDOWN || Message.message == WM_KEYUP) && Message.hwnd != hWnd)
		{
			PostMessage(hWnd, Message.message, Message.wParam, Message.lParam);
		}
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

    return Message.wParam;
}

void DrawStone(Stone);
void WinDrawBoard();
Coord2d mouse;
BoardGraphic boardInfo({ 0, 0 }, 806, 806);
Go Game;
#define SPACE_SIZE 42
static int drop_file_count = 0;
static bool Print_Sequance_Switch;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	FILE* fp = NULL;

	switch (iMessage) 
	{
	case WM_CREATE:

		// �ܼ�â ����
		//AllocConsole();
		//freopen_s(&fp, "CONOUT$", "wt", stdout);
		//

		hdc = GetDC(hWnd);

		hdc_BlackStone = CreateCompatibleDC(hdc);
		hdc_WhiteStone = CreateCompatibleDC(hdc);
		hdc_BackGround = CreateCompatibleDC(hdc);
		hdc_Board = CreateCompatibleDC(hdc);
		
		ReleaseDC(hWnd, hdc);

		HBITMAP bitBlackStone, bitWhiteStone, bitBackGround, bitBoard;
		bitBlackStone = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BLACKSTONE));
		bitWhiteStone = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WHITESTONE));
		bitBackGround = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BACKGROUND));
		bitBoard = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BOARD));

		SelectObject(hdc_BlackStone, bitBlackStone);
		SelectObject(hdc_WhiteStone, bitWhiteStone);
		SelectObject(hdc_BackGround, bitBackGround);
		SelectObject(hdc_Board, bitBoard);
		
		DeleteObject(bitBlackStone);
		DeleteObject(bitWhiteStone);
		DeleteObject(bitBackGround);
		DeleteObject(bitBoard);

		hBacksisButton = CreateWindow("button", "������(Z)", WS_CHILD |  WS_VISIBLE | BS_PUSHBUTTON,
			830, 200, 150, 30, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow("button", "�ʱ�ȭ(I)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			830, 240, 150, 30, hWnd, (HMENU)1, g_hInst, NULL);
		CreateWindow("button", "�Ѽ���(P)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			830, 280, 150, 30, hWnd, (HMENU)2, g_hInst, NULL);
		CreateWindow("button", "���� ǥ�� ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			830, 320, 150, 30, hWnd, (HMENU)3, g_hInst, NULL);

		return 0;

	case WM_MOUSEMOVE:
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);

		InvalidateRect(hWnd, NULL, FALSE);

		return 0;

	case WM_LBUTTONDOWN:
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		if (boardInfo.IsMouseInBoard(mouse))
		{ 
			
			Coord2d board;
			board.x = (mouse.x - 6) / SPACE_SIZE + 1;
			board.y = (mouse.y - 6) / SPACE_SIZE + 1;
			char* errorMSG = Game.Placement(board);
			if (0 == errorMSG)
				InvalidateRect(hWnd, NULL, FALSE);
			else  if("" != errorMSG)
				MessageBox(hWnd, errorMSG, "ERROR", MB_OK);

		}
		else {
			system("cls");
			Game.info().placement().print();
		}


		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		// �޴� - ���� - ����
		case ID_FILE_OPEN: 
		{
			OPENFILENAME OFN;
			char lpstrFile[MAX_PATH] = "";
			char str[256];
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = "�⺸ ����(*.ngf)\0*.ngf\0��� ����(*.*)\0*.*\0";
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			if (GetOpenFileName(&OFN) != 0)
			{
				wsprintf(str, "%s ������ �����߽��ϴ�.", lpstrFile);

				string extension = lpstrFile;		//Ȯ���� �����ϱ�
				extension = extension.substr(extension.length() - 3, 3);
				if (extension == "NGF" || extension == "ngf") //Ȯ���� NGF
				{
					GiboNGF gibo(lpstrFile);
					Game.Load(gibo);

					InvalidateRect(hWnd, NULL, FALSE);
				}
				else
					MessageBox(hWnd, "�����Ǵ� �⺸ ������ �ƴմϴ�.", "���� ����", MB_OK);
			}
			break; 
		}
		// �޴� - ���� - ����
		case ID_FILE_SAVE:
		{
			OPENFILENAME SFN;
			char lpstrFile[MAX_PATH] = "";
			memset(&SFN, 0, sizeof(OPENFILENAME));
			SFN.lStructSize = sizeof(OPENFILENAME);
			SFN.hwndOwner = hWnd;
			SFN.lpstrFilter = _T("�⺸ ����(*.ngf)\0*.ngf\0�⺸ ����(*.sgf)\0*.sgf\0��� ����(*.*)\0*.*\0");
			SFN.lpstrDefExt = "ngf";
			SFN.lpstrFile = lpstrFile;
			SFN.nMaxFile = 256;
			SFN.Flags = OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&SFN) != 0)
			{
				string extension = lpstrFile;		//Ȯ���� �����ϱ�
				extension = extension.substr(extension.length() - 3, 3);
				Game.Save(lpstrFile, extension);
			}

			break;
		}
			break;

		// �޴� - ���� - ����
		case ID_FILE_EXIT: 
			// �ܼ� �ݱ�
			FreeConsole();
			//

			DeleteDC(hdc_BlackStone);
			DeleteDC(hdc_WhiteStone);
			DeleteDC(hdc_BackGround);
			DeleteDC(hdc_Board);
			DeleteDC(hdc);
			DeleteDC(MemDC);
			DeleteDC(hdcMem);

			PostQuitMessage(0);
			break; 

		case 0:	//������ ��ư
			if (Game.Backsies()) {
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case 1:	//�ʱ�ȭ ��ư
			if (Game.Init()) 
			{
				InvalidateRect(hWnd, NULL, FALSE);
				MessageBox(hWnd, _T("�ʱ�ȭ �߽��ϴ�."), _T("�˸�"), MB_OK);
			}
			break;
		case 2:	//�Ѽ��� ��ư
			if (Game.Pass())
			{
				InvalidateRect(hWnd, NULL, FALSE);
				MessageBox(hWnd, _T("�Ѽ� �������ϴ�."), _T("�˸�"), MB_OK);
			}
			break;
		case 3:	// ���� ǥ��
			Print_Sequance_Switch = !Print_Sequance_Switch;

			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		
		return 0;
	case WM_DROPFILES:
	{
		char drag_filename[MAX_PATH] = { 0 };

								   //���ڷ� 0xFFFFFFFF�� �Ѱ��ָ�, ��ӵ� ������ ������ ���ɴϴ�. 
		drop_file_count = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);//���� ����

		DragQueryFile((HDROP)wParam, 0, drag_filename, 1024);
 
		string extension = drag_filename;		//Ȯ���� �����ϱ�
		extension = extension.substr(extension.length() - 3, 3);
		if (extension == "NGF" || extension == "ngf") //Ȯ���� NGF
		{
			GiboNGF gibo(drag_filename);
			Game.Load(gibo);
			
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else
			MessageBox(hWnd, "�����Ǵ� �⺸ ������ �ƴմϴ�.", "���� ����", MB_OK);

		//�巡�� ��� ���� ����Ʈ�� �����մϴ�. 
		DragFinish((HDROP)wParam);

		return 0;
	}

	case WM_KEYDOWN:
	//	cout << wParam <<"Ű ����" << endl;
		switch (wParam)
		{
			if (Game.mode() == "gibo") 
			{
			case KEY_LEFT:
					
				break;
			case KEY_RIGHT:
					
				break;
			}
		case KEY_Z:	// ������ ����Ű
			if (Game.Backsies()) 
				InvalidateRect(hWnd, NULL, FALSE);
			break;

		case KEY_I:	// �ʱ�ȭ ����Ű
			if (Game.Init()) 
				InvalidateRect(hWnd, NULL, FALSE);
			break;

		case KEY_P:	// 
			if (Game.Pass()) 
				InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		MemDC = CreateCompatibleDC(hdc);

		hdcMem = CreateCompatibleDC(hdc); //2
		hbmMem = CreateCompatibleBitmap(hdc, 1000, 900);//3
		hbmMemOld = (HBITMAP)SelectObject(hdcMem, hbmMem);//4
		
		BitBlt(hdcMem, 0, 0, 1000, 900, hdc_BackGround, 0, 0, SRCCOPY);

		WinDrawBoard();

		BitBlt(hdc, 0, 0, 1000, 900, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hbmMemOld); //-4
		DeleteObject(hbmMem); //-3
		DeleteDC(hdcMem); //-2

		EndPaint(hWnd, &ps);

		return 0;

	case WM_DESTROY:
		// �ܼ� �ݱ�
		//FreeConsole();
		//

		DeleteDC(hdc_BlackStone);
		DeleteDC(hdc_WhiteStone);
		DeleteDC(hdc_BackGround);
		DeleteDC(hdc_Board);
		DeleteDC(hdc);
		DeleteDC(MemDC);
		DeleteDC(hdcMem);

		PostQuitMessage(0);

		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawStone(Stone stone)
{
	int x = stone.x();
	int y = stone.y();
	string color = stone.color();

	if (color == "Black")
	{
		BitBlt(hdcMem, SPACE_SIZE * (x - 1) + 6, SPACE_SIZE * (y - 1) + 6, 39, 39, hdc_BlackStone, 0, 0, SRCCOPY);
		SetTextColor(hdcMem, RGB(255, 255, 255));
	}
	else if (color == "White")
	{
		BitBlt(hdcMem, SPACE_SIZE * (x - 1) + 6, SPACE_SIZE * (y - 1) + 6, 39, 39, hdc_WhiteStone, 0, 0, SRCCOPY);
		SetTextColor(hdcMem, RGB(0, 0, 0));
	}
}

void WinDrawBoard() {
	BitBlt(hdcMem, 0, 0, 806, 806, hdc_Board, 0, 0, SRCCOPY);

	SetTextAlign(hdcMem, TA_CENTER);
	SetBkMode(hdcMem, TRANSPARENT);
	for (int x = 1; x < 20; x++)
	{
		for (int y = 1; y < 20; y++)
		{
			DrawStone(Game.Read( { x, y } ));

			string A = std::to_string(Game.Read({ x, y }).sequence());
			if (A != "0" && Print_Sequance_Switch != false)
			{
				TextOut(hdcMem, SPACE_SIZE * (x - 1) + 25, SPACE_SIZE * (y - 1) + 18, A.c_str(), A.length());
			}
		}
	}

	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = 0;
	bf.SourceConstantAlpha = 180;
	
	Coord2d board;
	board.x = (mouse.x - 6) / SPACE_SIZE + 1;
	board.y = (mouse.y - 6) / SPACE_SIZE + 1;
	if (boardInfo.IsMouseInBoard(mouse) && Game.Read(board).color() == "NULL")
	{
		if (Stone::Sqnce2color(Game.info().sequence()) == "Black")
			GdiAlphaBlend(hdcMem, SPACE_SIZE * (board.x - 1) + 6, SPACE_SIZE * (board.y - 1) + 6, 39, 39, hdc_BlackStone, 0, 0, 39, 39, bf);
		else
			GdiAlphaBlend(hdcMem, SPACE_SIZE * (board.x - 1) + 6, SPACE_SIZE * (board.y - 1) + 6, 39, 39, hdc_WhiteStone, 0, 0, 39, 39, bf);
	}
	
}

*/