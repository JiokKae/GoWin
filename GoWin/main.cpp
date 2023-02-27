// GoWin.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "framework.h"
#include "GoWinApplication.h"
#include <format>

constexpr int MAX_LOADSTRING = 100;

TCHAR szTitle[MAX_LOADSTRING];		// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];	// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AboutProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    NetboxProc(HWND, UINT, WPARAM, LPARAM);

GoWinApplication goWin;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE /*hPrevInstance*/,
	_In_ LPWSTR    /*lpCmdLine*/,
	_In_ int       nCmdShow)
{
	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GOWIN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GOWIN));

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if ( false ) std::cout << std::format("hWnd : {:<15}\t msg : {:<15} wParam : {}\t lParam : {}\n", static_cast<void*>(msg.hwnd), ReadMessage(msg.message), msg.wParam, msg.lParam);

		if (msg.message == WM_KEYDOWN && msg.hwnd != goWin.main_window_handle())
		{
			PostMessage(goWin.main_window_handle(), msg.message, msg.wParam, msg.lParam);
		}
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex{
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GOICON)),
		.hCursor = LoadCursor(nullptr, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		.lpszMenuName = MAKEINTRESOURCEW(IDC_GOWIN),
		.lpszClassName = szWindowClass,
		.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GOICON)),
	};

	return RegisterClassEx(&wcex);
}

//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	goWin.set_hInstance(hInstance);

	HWND main = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, 1200 + 16, 820 + 50 + 9, nullptr, nullptr, hInstance, nullptr);
	
	if (!main)
	{
		return FALSE;
	}

	goWin.set_main_window_handle(main);
	
	ShowWindow(main, nCmdShow);
	UpdateWindow(main);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_SERVER_ENTER:
			DialogBox(goWin.hInstance(), MAKEINTRESOURCE(IDD_NETBOX), hWnd, NetboxProc);
			break;
		case IDM_ABOUT:
			DialogBox(goWin.hInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutProc);
			break;
		default:
			break;
		}
	}
	default:
		break;
	}

	return goWin.main_procedure(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return goWin.about_procedure(hDlg, message, wParam, lParam);
}

INT_PTR CALLBACK NetboxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return goWin.netbox_procedure(hDlg, message, wParam, lParam);
}
