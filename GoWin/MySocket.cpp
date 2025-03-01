#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "MySocket.h"
#include <stdio.h>
#include <algorithm>

#define CloseServerSocket(error_message) \
	CloseServerSocketImpl(__func__, error_message);

Message::Message(Type type)
	: type{ type }
{
}

COMM_MSG::COMM_MSG()
	: Message(Type::COMMON)
{
}

CHAT_MSG::CHAT_MSG(LPCTSTR chat)
	: Message(Type::CHATTING)
	, buf()
{
	wcscpy_s(buf, chat);
}

Placement_MSG::Placement_MSG(int sequence, int x, int y)
	: Message(Type::PLACEMENT)
	, sequence(sequence)
	, x{ x }
	, y{ y }
{
}

Command_MSG::Command_MSG(Command command)
	: Message(Type::COMMAND)
	, command{ command }
{
}

MySocket::MySocket() 
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("not equal version\n");
		return;
	}
	m_server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (m_server_socket == SOCKET_ERROR)
	{
		printf("Create Socket Fail\n");
		return;
	}
}

bool MySocket::Create(HWND hWnd)
{
	if (m_status != Status::notConnected)
	{
		return false;
	}

	m_hWnd = hWnd;

	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(MySocket::SERVER_PORT);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(m_server_socket, (const sockaddr*)&server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		CloseServerSocket("바인딩 실패");
		return false;
	}

	if (listen(m_server_socket, 5) == SOCKET_ERROR)
	{
		CloseServerSocket("대기상태 설정 실패");
		return false;
	}

	if (WSAAsyncSelect(m_server_socket, hWnd, MySocket::WM_ASYNC, FD_ACCEPT | FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		CloseServerSocket("비동기 소켓 설정 실패");
		return false;
	}

	m_status = Status::Server;

	return true;
}

void MySocket::CloseServerSocketImpl(const char* func_name, const char* error_message)
{
	printf("MySocket::%s(): %s\n", func_name, error_message);

	closesocket(m_server_socket);
	m_server_socket = INVALID_SOCKET;

	WSACleanup();
}

//--------------------------------------------------------------------------------------
// Name:  OnAccept
// Desc:  FD_ACCEPT 핸들러 함수, 클라이언트의 접속을 수락한다
// Param: serSock -> 서버소켓
// Ret:   접속된 클라이언트의 새로 생성된 소켓
//--------------------------------------------------------------------------------------
SOCKET MySocket::OnAccept(HWND hWnd, SOCKET sockServ)
{
	int iAddrLen = sizeof(SOCKADDR_IN);

	SOCKADDR_IN Addr{};
	SOCKET client_socket = accept(sockServ, (SOCKADDR*)&Addr, &iAddrLen);
	if (INVALID_SOCKET == client_socket)
	{
		return INVALID_SOCKET;
	}

	// 클라이언트 소켓도 비동기소켓으로 전환한다
	if (WSAAsyncSelect(client_socket, hWnd, MySocket::WM_ASYNC, FD_READ | FD_CLOSE) == SOCKET_ERROR)
	{
		printf("OnAccept() : WSAAsyncSelect() 실패\n");
		closesocket(client_socket);

		return INVALID_SOCKET;
	}

	return client_socket;
}
bool MySocket::FD_Accept() 
{
	if (MySocket::MAX_CLIENT <= client_sockets.size())
	{
		return false;
	}

	const SOCKET client_socket = OnAccept(m_hWnd, m_server_socket);
	if (client_socket == INVALID_SOCKET)
	{
		return false;
	}

	client_sockets.push_back(client_socket);

	return true;
}

void MySocket::FD_Read(SOCKET sock, COMM_MSG* pMsg)
{
	printf("MySocket::FD_READ()\n");

	recvn(sock, (char*)pMsg, Message::BUF_SIZE, 0);
}


//--------------------------------------------------------------------------------------
// Name:	OnClose
// Desc:	FD_CLOSE 핸들러. 클라이언트의 소켓을 해제하고 배열을 재정리한다
// Param:	sock		-> FD_CLOSE 메시지를 발생시킨 소켓
//--------------------------------------------------------------------------------------
void MySocket::OnClose(SOCKET sock)
{
	closesocket(sock);

	if (m_status == Status::Server)
	{
		client_sockets.erase(std::ranges::find(client_sockets, sock));
	}
	else if (m_status == Status::Client)
	{
		WSACleanup();
		m_server_socket = INVALID_SOCKET;
		m_status = Status::notConnected;
	}
}

void MySocket::FD_Close(SOCKET sock)
{
	OnClose(sock);
}

bool MySocket::Enter(HWND hWnd, char* server_ip)
{
	if (m_status != Status::notConnected)
	{
		return false;
	}

	m_hWnd = hWnd;
	printf("Enter() : 입력된 server ip = %s\n", server_ip);
	
	constexpr long lEvent = FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE;
	if (WSAAsyncSelect(m_server_socket, hWnd, MySocket::WM_ASYNC, lEvent) == SOCKET_ERROR)
	{
		CloseServerSocket("WSAAsyncSelect() 실패");
		return false;
	}

	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(MySocket::SERVER_PORT);
	server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip);

	if (connect(m_server_socket, (const sockaddr*)&server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR && WSAEWOULDBLOCK != WSAGetLastError())
	{
		CloseServerSocket("커넥트 실패");
		return false;
	} 

	m_status = Status::Client;

	return true;
}

int MySocket::recvn(SOCKET s, char* buf, int len, int flags)
{
	int left = len;

	while (left > 0)
	{
		const int received = recv(s, buf, left, flags);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
		{
			break;
		}
		left -= received;
		buf += received;
	}

	return (len - left);
}


int MySocket::Send(char* buf, int size)
{
	switch (m_status)
	{
	case Status::Server:
		if (client_sockets.empty() == true)
		{
			return SOCKET_ERROR;
		}
		return send(client_sockets[0], buf, size, 0);

	case Status::Client:
		return send(m_server_socket, buf, size, 0);

	case Status::notConnected:
	default:
		return SOCKET_ERROR;
	}
}

bool MySocket::send_message(std::unique_ptr<Message> message)
{
	return Send(reinterpret_cast<char*>(message.get()), Message::BUF_SIZE) != SOCKET_ERROR;
}

bool MySocket::IsConnected() const
{
	return m_status != Status::notConnected;
}

MySocket::Status MySocket::status() const
{
	return m_status;
}

