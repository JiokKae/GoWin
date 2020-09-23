// GoWin.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "framework.h"
#include "GoWin.h"
#pragma warning(disable:4996)

#define MAX_LOADSTRING 100
#define SPACE_SIZE 42

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HBITMAP hbmMem, hbmMemOld;

HDCManager bitmaps(5);
bitmaps[]
HDC hdc_BlackStone, hdc_WhiteStone, hdc_BackGround, hdc_Board;
HDC hdc, hdcMem; //MemDC
HWND hWindow;
HWND hChatInputBox, hChatBox;
HWND hWCS, hBCS;
HFONT hFont;

const wchar_t* errorMSG_wchar[5] = {
    _T(""),
    _T("바둑판 안에 착수해주세요"),
    _T("이미 바둑 돌이 있습니다"),
    _T("착수 금지점입니다"),
    _T("패 입니다"),
};

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Netbox(HWND, UINT, WPARAM, LPARAM);

void DrawStone(Stone);
void WinDrawBoard();
void SendTextEdit(HWND edit, LPCWSTR pText);

Coord2d         mouse;
BoardGraphic    boardInfo({ 0, 0 }, 806, 806, SPACE_SIZE, 6);
Go              Game;
MySocket        mysocket;

//static int drop_file_count = 0;
static bool Print_Sequance_Switch;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GOWIN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GOWIN));

    MSG msg;
    
    // 기본 메시지 루프입니다:
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (msg.message == WM_KEYDOWN && msg.hwnd != hWindow)
        {
            PostMessage(hWindow, msg.message, msg.wParam, msg.lParam);
        }
        if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GOICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GOWIN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_GOICON));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
       CW_USEDEFAULT, CW_USEDEFAULT, 1200 + 16, 820 + 50 + 9, nullptr, nullptr, hInstance, nullptr);
   hWindow = hWnd;
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
char* Read(UINT message, char* buffer) {
    switch (message)
    {
    case WM_CREATE:
        strcpy(buffer, "WM_CREATE"); break;        
    case WM_MOUSEMOVE:
        strcpy(buffer, "WM_MOUSEMOVE"); break;
    case WM_COMMAND:
        strcpy(buffer, "WM_COMMAND"); break;
    case WM_LBUTTONDOWN:
        strcpy(buffer, "WM_LBUTTONDOWN"); break;
    case WM_PAINT:
        strcpy(buffer, "WM_PAINT"); break;
    case WM_SETCURSOR:
        strcpy(buffer, "WM_SETCURSOR"); break;
    case WM_KEYDOWN:
        strcpy(buffer, "WM_KEYDOWN"); break;
    default:
        itoa(message, buffer, 10);
    }
    return buffer;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    FILE* fp = NULL;

    /*char buffer[64] = "";
    if(message == WM_PAINT)
       printf("hWnd : %p\t msg : %-15s wParam : %d\t lParam : %d\n", hWnd, Read(message, buffer), wParam, lParam);
    */
    switch (message)
    {
    case WM_CREATE:
        // 콘솔창 열기
        AllocConsole();
        _wfreopen_s(&fp, _T("CONOUT$"), _T("wt"), stdout);
        //

        hdc = GetDC(hWnd);

        hdc_BlackStone = CreateCompatibleDC(hdc);
        hdc_WhiteStone = CreateCompatibleDC(hdc);
        hdc_BackGround = CreateCompatibleDC(hdc);
        hdc_Board = CreateCompatibleDC(hdc);
		hdcMem = CreateCompatibleDC(hdc);

        ReleaseDC(hWnd, hdc);

        HBITMAP bitBlackStone, bitWhiteStone, bitBackGround, bitBoard;

        bitBlackStone = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLACKSTONE));
        bitWhiteStone = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WHITESTONE));
        bitBackGround = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BACKGROUND));
        bitBoard = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOARD));

		
        SelectObject(hdc_BlackStone, bitBlackStone);
        SelectObject(hdc_WhiteStone, bitWhiteStone);
        SelectObject(hdc_BackGround, bitBackGround);
        SelectObject(hdc_Board, bitBoard);

        DeleteObject(bitBlackStone);
        DeleteObject(bitWhiteStone);
        DeleteObject(bitBackGround);
        DeleteObject(bitBoard);

        hBCS = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY ,
            840, 200, 50, 30, hWnd, (HMENU)1, hInst, NULL);
        hWCS = CreateWindow(_T("EDIT"), _T("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY ,
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

            int errorMSG = Game.Placement(placement_point);

            if (errorMSG == 0) {
                SetWindowText(hBCS, to_wstring(Game.info().black_player().captured_stone()).c_str());
                SetWindowText(hWCS, to_wstring(Game.info().white_player().captured_stone()).c_str());
                InvalidateRect(hWnd, NULL, FALSE);
                if (mysocket.Status() != SocketStatus::notConnected) 
                {
                    PlacementInfo placementInfo = Game.getLastPlacementInfo();
                    Placement_MSG msg;
                    msg.type = PLACEMENT;
                    msg.sequence = placementInfo.sequence;
                    msg.x = placementInfo.placment.x;
                    msg.y = placementInfo.placment.y;

                    mysocket.Send((char*)&msg, BUFSIZE);
                }
            }
            else if(errorMSG != ERR_NOTEMPTY)
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
            {
                tagOFNW OFN;
                WCHAR lpstrFile[MAX_PATH] = _T("");
                WCHAR str[256];
                memset(&OFN, 0, sizeof(tagOFNW));
                OFN.lStructSize = sizeof(tagOFNW);
                OFN.hwndOwner = hWnd;
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
                        Game.Load(gibo);

                        InvalidateRect(hWnd, NULL, FALSE);
                    }
                    else
                        MessageBox(hWnd, _T("지원되는 기보 파일이 아닙니다."), _T("열기 실패"), MB_OK);
                }
                break;
            }
            // 메뉴 - 파일 - 저장
            case IDM_FILE_SAVE:
            {
                tagOFNW SFN;
                WCHAR lpstrFile[MAX_PATH] = _T("");
                memset(&SFN, 0, sizeof(tagOFNW));
                SFN.lStructSize = sizeof(tagOFNW);
                SFN.hwndOwner = hWnd;
                SFN.lpstrFilter = _T("기보 파일(*.ngf)\0*.ngf\0기보 파일(*.sgf)\0*.sgf\0모든 파일(*.*)\0*.*\0");
                SFN.lpstrDefExt = _T("ngf");
                SFN.lpstrFile = lpstrFile;
                SFN.nMaxFile = 256;
                SFN.Flags = OFN_OVERWRITEPROMPT;
                if (GetSaveFileName(&SFN) != 0)
                {
                    wstring extension = lpstrFile;		//확장자 추출하기
                    extension = extension.substr(extension.length() - 3, 3);
                    Game.Save(lpstrFile, extension);
                }

                break;
            }
            break;

            case IDM_FILE_EXIT:
                // 콘솔 닫기
                FreeConsole();
                //

                DeleteDC(hdc_BlackStone);
                DeleteDC(hdc_WhiteStone);
                DeleteDC(hdc_BackGround);
                DeleteDC(hdc_Board);
                DeleteDC(hdc);
                //DeleteDC(MemDC);
                DeleteDC(hdcMem);

                // PostQuitMessage(0);
                 //break;

                DestroyWindow(hWnd);
                break;
            case IDM_SERVER_CREATE:
                if (mysocket.Create(hWnd) != INVALID_SOCKET) {
                    SendTextEdit(hChatBox, _T("[System] 서버 생성"));
                    HMENU hMenu = GetMenu(hWnd);
                    HMENU hSubMenu = GetSubMenu(hMenu, 1);
                    EnableMenuItem(hMenu, GetMenuItemID(hSubMenu, 0), MF_GRAYED);

                }
                else {
                    SendTextEdit(hChatBox, _T("[System] 서버 생성 실패"));
                }
                break;
            case IDM_SERVER_ENTER:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_NETBOX), hWnd, Netbox);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDA_BACKSIES:	//무르기 버튼
                if (Game.Backsies())
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
                if (Game.Init())
                {
                    if (mysocket.Status() != SocketStatus::notConnected)
                    {
                        Command_MSG msg;
                        msg.type = COMMAND;
                        msg.command = INIT;

                        mysocket.Send((char*)&msg, BUFSIZE);
                    }
					SetWindowText(hBCS, to_wstring(Game.info().black_player().captured_stone()).c_str());
					SetWindowText(hWCS, to_wstring(Game.info().white_player().captured_stone()).c_str());
                    InvalidateRect(hWnd, NULL, FALSE);
                    SendTextEdit(hChatBox, _T("[System] 초기화 했습니다."));
                    //MessageBox(hWnd, _T("초기화 했습니다."), _T("알림"), MB_OK);
                }
                SetFocus(hWnd);
                break;

            case IDA_PASS:	//한수쉼 버튼
                if (Game.Pass())
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
                Print_Sequance_Switch = !Print_Sequance_Switch;
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
                    Game.Placement(point);
                    InvalidateRect(hWnd, NULL, FALSE);
                    break;
                }
                case COMMAND:
                {
                    Command_MSG* commandMsg = (Command_MSG*)&msg;
                    switch (commandMsg->command)
                    {
                    case BACKSIES:
                        Game.Backsies();
                        InvalidateRect(hWnd, NULL, FALSE);
                        break;

                    case INIT:
                        Game.Init();
                        InvalidateRect(hWnd, NULL, FALSE);
                        break;

                    case PASS:
                        Game.Pass();
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            //MemDC = CreateCompatibleDC(hdc); ;;;;;;

            //hdcMem = CreateCompatibleDC(hdc); //2 프로그램 초기화때로 이동
            hbmMem = CreateCompatibleBitmap(hdc, 1200, 820);//3
            hbmMemOld = (HBITMAP)SelectObject(hdcMem, hbmMem);//4

            BitBlt(hdcMem, 0, 0, 1200, 820, hdc_BackGround, 0, 0, SRCCOPY);

            WinDrawBoard();

            BitBlt(hdc, 0, 0, 1200, 820, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, hbmMemOld); //-4
            DeleteObject(hbmMem); //-3
            // DeleteDC(hdcMem); //-2 프로그램 끝날때로 이동
			
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
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
// 멀티 대화 상자의 메시지 처리기입니다.
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
            mysocket.Enter(hWindow, WCharToChar(buffer));
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

void DrawStone(Stone stone)
{
    int x = stone.x();
    int y = stone.y();
    Color color = stone.color();

    if (color == Color::Black)
    {
        BitBlt(hdcMem, SPACE_SIZE * (x - 1) + 6, SPACE_SIZE * (y - 1) + 6, 39, 39, hdc_BlackStone, 0, 0, SRCCOPY);
        SetTextColor(hdcMem, RGB(255, 255, 255));
    }
    else if (color == Color::White)
    {
        BitBlt(hdcMem, SPACE_SIZE * (x - 1) + 6, SPACE_SIZE * (y - 1) + 6, 39, 39, hdc_WhiteStone, 0, 0, SRCCOPY);
        SetTextColor(hdcMem, RGB(0, 0, 0));
    }
}

void WinDrawBoard()
{
    BitBlt(hdcMem, 0, 0, 806, 806, hdc_Board, 0, 0, SRCCOPY);

    SetTextAlign(hdcMem, TA_CENTER);
    SetBkMode(hdcMem, TRANSPARENT);
    for (int x = 1; x < 20; x++)
    {
        for (int y = 1; y < 20; y++)
        {
            DrawStone(Game.Read({ x, y }));

            int sqc = Game.Read({ x, y }).sequence();
            if (sqc != 0 && Print_Sequance_Switch == true) 
            {
                TextOut(hdcMem, SPACE_SIZE * (x - 1) + 25, SPACE_SIZE * (y - 1) + 18, std::to_wstring(sqc).c_str(), (int)std::to_wstring(sqc).length());
            }
        }
    }
   
    if (boardInfo.IsMouseInBoard(mouse))
    {
        Coord2d board_point = boardInfo.MouseToBoard(mouse.x, mouse.y);;
        if (Game.Read(board_point).color() == Color::Null)
        {
            BLENDFUNCTION bf;
            bf.AlphaFormat = AC_SRC_ALPHA;
            bf.BlendFlags = 0;
            bf.BlendOp = 0;
            bf.SourceConstantAlpha = 180;

            if (Stone::Sqnce2color(Game.info().sequence()) == Color::Black)
                GdiAlphaBlend(hdcMem, SPACE_SIZE * (board_point.x - 1) + 6, SPACE_SIZE * (board_point.y - 1) + 6, 39, 39, hdc_BlackStone, 0, 0, 39, 39, bf);
            else
                GdiAlphaBlend(hdcMem, SPACE_SIZE * (board_point.x - 1) + 6, SPACE_SIZE * (board_point.y - 1) + 6, 39, 39, hdc_WhiteStone, 0, 0, 39, 39, bf);
        }
    }
}

void SendTextEdit(HWND hEdit, LPCWSTR pText) {
    
	if (GetWindowTextLength(hChatBox) > 100)
	{
		WCHAR buffer[50];
		GetWindowText(hEdit, buffer, 30);
		SendMessage(hEdit, EM_SETSEL, 0, (LPARAM)(wcschr(buffer, _T('\n')) - buffer + 1) );
		SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)_T(""));
		// todo
	}
	wstring newText = pText;
	newText.append(_T("\r\n"));
	SendMessage(hEdit, EM_SETSEL, 0, -1);
    SendMessage(hEdit, EM_SETSEL, -1, -1);
    SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)newText.c_str()); // append!

}
