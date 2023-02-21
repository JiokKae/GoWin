#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "MySocket.h"
#include <stdio.h>
#include <algorithm>

Message::Message(int type = 0)
	: type(type)
{
}

COMM_MSG::COMM_MSG()
	: Message(0)
	, dummy()
{
}

CHAT_MSG::CHAT_MSG(LPCTSTR chat)
	: Message(CHATTING)
	, buf()
{
	wcscpy_s(buf, chat);
}

Placement_MSG::Placement_MSG(int sequence, int x, int y)
	: Message(PLACEMENT)
	, sequence(sequence)
	, x(x)
	, y(y)
	, dummy()
{
}

Command_MSG::Command_MSG(int command)
	: Message(COMMAND)
	, command(command)
	, dummy()
{
}

MySocket::MySocket() 
	: server_socket(INVALID_SOCKET)
	, hWnd(nullptr)
	, m_status(MySocket::Status::notConnected)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("not equal version\n");
		return;
	}
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server_socket == SOCKET_ERROR)
	{
		printf("Create Socket Fail\n");
		return;
	}
}

SOCKET MySocket::Create(HWND hWnd) {
	this->hWnd = hWnd;

	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4777);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int rt = bind(server_socket, (const sockaddr*)&server_addr, sizeof(SOCKADDR_IN));
	if (rt == SOCKET_ERROR)
	{
		printf("바인딩 실패\n");
		closesocket(server_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	rt = listen(server_socket, 5);
	if (rt == SOCKET_ERROR)
	{
		printf("대기상태 설정 실패\n");
		closesocket(server_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	rt = WSAAsyncSelect(server_socket, hWnd, WM_ASYNC, FD_ACCEPT | FD_READ | FD_CLOSE);
	if (rt == SOCKET_ERROR)
	{
		printf("Create() : WSAAsyncSelect() 실패\n");
		closesocket(server_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	m_status = Status::Server;
	return server_socket;
}
//--------------------------------------------------------------------------------------
// Name:  OnAccept
// Desc:  FD_ACCEPT 핸들러 함수, 클라이언트의 접속을 수락한다
// Param: serSock -> 서버소켓
// Ret:   접속된 클라이언트의 새로 생성된 소켓
//--------------------------------------------------------------------------------------
SOCKET MySocket::OnAccept(HWND hWnd, SOCKET sockServ)
{
	// 접속을 수락한다
	int iLen = sizeof(SOCKADDR_IN);

	SOCKADDR_IN Addr;
	SOCKET sock = accept(sockServ, (SOCKADDR*)&Addr, &iLen);
	if (INVALID_SOCKET == sock)
		return INVALID_SOCKET;

	// 클라이언트 소켓도 비동기소켓으로 전환한다
	iLen = WSAAsyncSelect(sock, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
	if (SOCKET_ERROR == iLen)
	{
		printf("OnAccept() : WSAAsyncSelect() 실패\n");
		closesocket(sock);
		return INVALID_SOCKET;
	}

	return sock;
}
bool MySocket::FD_Accept() 
{
	if (MAXCLIENT <= client_sockets.size())
		return false;

	auto client_socket = OnAccept(hWnd, server_socket);
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
	if (SOCKET_ERROR == recvn(sock, (char*)pMsg, 512, 0))
		pMsg = nullptr;
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
		server_socket = INVALID_SOCKET;
		m_status = Status::notConnected;
	}
}

void MySocket::FD_Close(SOCKET sock)
{
	OnClose(sock);
}

bool MySocket::Enter(HWND hWnd, char* server_ip)
{
	this->hWnd = hWnd;
	printf("Enter() : 입력된 server ip = %s\n", server_ip);
	
	int rt = WSAAsyncSelect(server_socket, hWnd, WM_ASYNC, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE );
	if (rt == SOCKET_ERROR)
	{
		printf("WSAAsyncSelect() 실패\n");
		closesocket(server_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4777);
	server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip);

	rt = connect(server_socket, (const sockaddr*)&server_addr, sizeof(SOCKADDR_IN));
	if (rt == SOCKET_ERROR && WSAEWOULDBLOCK != WSAGetLastError())
	{
		printf("커넥트 실패\n");
		closesocket(server_socket);
		WSACleanup();
		return INVALID_SOCKET;
	} 

	m_status = Status::Client;

	return true;
}

int MySocket::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	int left = len;

	while (left > 0)
	{
		received = recv(s, buf, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
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
		if (client_sockets.empty()) return SOCKET_ERROR;
		return send(client_sockets[0], buf, size, 0);
		break;
	case Status::Client:
		return send(server_socket, buf, size, 0);
		break;
	case Status::notConnected:
	default:
		return SOCKET_ERROR;
	}
	return SOCKET_ERROR;
}

int MySocket::send_message(std::unique_ptr<Message> message)
{
	return Send(reinterpret_cast<char*>(message.get()), BUFSIZE);
}

bool MySocket::IsConnected() const
{
	return m_status != Status::notConnected;
}

MySocket::Status MySocket::status() const
{
	return m_status;
}

