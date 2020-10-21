// GoWin.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "framework.h"
#include "GoWin.h"
#pragma warning(disable:4996)

#define MAX_LOADSTRING 100
#define SPACE_SIZE 42

const wchar_t* errorMSG_wchar[5] = {
	_T(""),
	_T("바둑판 안에 착수해주세요"),
	_T("이미 바둑 돌이 있습니다"),
	_T("착수 금지점입니다"),
	_T("패 입니다"),
};



// 전역 변수:
HINSTANCE g_hInstance;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

GoWinManager manager;
HWND g_hWnd;


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Netbox(HWND, UINT, WPARAM, LPARAM);

void SendTextEdit(HWND edit, LPCWSTR pText);

Coord2d         mouse;
BoardGraphic    boardInfo({ 0, 0 }, 806, 806, SPACE_SIZE, 6);
MySocket        mysocket;

//static int drop_file_count = 0;

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

    // 기본 메시지 루프입니다:
	MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (msg.message == WM_KEYDOWN && msg.hwnd != g_hWnd)
        {
            PostMessage(g_hWnd, msg.message, msg.wParam, msg.lParam);
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


//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindow (szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
       CW_USEDEFAULT, CW_USEDEFAULT, 1200 + 16, 820 + 50 + 9, nullptr, nullptr, hInstance, nullptr);
   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return manager.MainProc(hWnd, message, wParam, lParam);
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
            mysocket.Enter(g_hWnd, WCharToChar(buffer));
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
