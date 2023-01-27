#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "MySocket.h"
#include <stdio.h>

MySocket::MySocket() {
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
	server_ip[0] = '\0';
}

SOCKET MySocket::Create(HWND hWnd) {
	int rt;
	this->hWnd = hWnd;
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4777);
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	rt = bind(server_socket, (const sockaddr*)&server_addr, sizeof(SOCKADDR_IN));
	if (rt == SOCKET_ERROR)
	{
		printf("���ε� ����\n");
		closesocket(server_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	rt = listen(server_socket, 5);
	if (rt == SOCKET_ERROR)
	{
		printf("������ ���� ����\n");
		closesocket(server_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	rt = WSAAsyncSelect(server_socket, hWnd, WM_ASYNC, FD_ACCEPT | FD_READ | FD_CLOSE);
	if (rt == SOCKET_ERROR)
	{
		printf("Create() : WSAAsyncSelect() ����\n");
		closesocket(server_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	m_status = Status::Server;
	return server_socket;
}
//--------------------------------------------------------------------------------------
// Name:  OnAccept
// Desc:  FD_ACCEPT �ڵ鷯 �Լ�, Ŭ���̾�Ʈ�� ������ �����Ѵ�
// Param: serSock -> ��������
// Ret:   ���ӵ� Ŭ���̾�Ʈ�� ���� ������ ����
//--------------------------------------------------------------------------------------
SOCKET MySocket::OnAccept(HWND hWnd, SOCKET sockServ)
{
	SOCKET		sock;
	SOCKADDR_IN	Addr;
	int		iLen;

	//
	// ������ �����Ѵ�
	iLen = sizeof(SOCKADDR_IN);
	sock = accept(sockServ, (SOCKADDR*)&Addr, &iLen);
	if (INVALID_SOCKET == sock)
		return INVALID_SOCKET;

	//
	// Ŭ���̾�Ʈ ���ϵ� �񵿱�������� ��ȯ�Ѵ�
	iLen = WSAAsyncSelect(sock, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);
	if (SOCKET_ERROR == iLen)
	{
		printf("OnAccept() : WSAAsyncSelect() ����\n");
		closesocket(sock);
		return INVALID_SOCKET;
	}

	return sock;
}
bool MySocket::FD_Accept() {
	// �ִ�Ŭ���̾�Ʈ ���� üũ�ؼ� �� á�ٸ� ���ӿ�û�� �����Ѵ�
	if (MAXCLIENT - 1 <= current_accept_index)
		return false;

	client_socket[current_accept_index] = OnAccept(hWnd, server_socket);

	if (INVALID_SOCKET == client_socket[current_accept_index])
		return false;
	else
	{
		current_accept_index++;
		return true;
	}
}

void MySocket::FD_Read(SOCKET sock, COMM_MSG* pMsg) {
	printf("FD_READ ȣ���\n");
	if (SOCKET_ERROR == recvn(sock, (char*)pMsg, 512, 0))
		pMsg = nullptr;
}


//--------------------------------------------------------------------------------------
// Name:	OnClose
// Desc:	FD_CLOSE �ڵ鷯. Ŭ���̾�Ʈ�� ������ �����ϰ� �迭�� �������Ѵ�
// Param:	sockArray	-> Ŭ���̾�Ʈ�� ���Ϲ迭
//		sock		-> FD_CLOSE �޽����� �߻���Ų ����
// Retern:	void
//--------------------------------------------------------------------------------------
void MySocket::OnClose(SOCKET* sockArray, SOCKET sock)
{
	int			i, j;

	for (i = 0; i < MAXCLIENT; i++)
	{
		if (sock == sockArray[i])
		{
			closesocket(sockArray[i]);
			for (j = i; j < MAXCLIENT - 1; j++)
				sockArray[j] = sockArray[j + 1];
		}
	}
}
void MySocket::FD_Close(SOCKET sock) {
	OnClose(client_socket, sock);
	current_accept_index--;
}

bool MySocket::Enter(HWND hWnd, char* server_ip) {
	int rt;
	this->hWnd = hWnd;
	printf("Enter() : �Էµ� server ip = %s\n", server_ip);
	
	rt = WSAAsyncSelect(server_socket, hWnd, WM_ASYNC, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE );
	if (rt == SOCKET_ERROR)
	{
		printf("WSAAsyncSelect() ����\n");
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
		printf("Ŀ��Ʈ ����\n");
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
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}


int MySocket::Send(char* buf, int size) {
	switch (m_status)
	{
	case Status::Server:
		return send(client_socket[current_accept_index-1], buf, size, 0);
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

bool MySocket::IsConnected() const
{
	return m_status != Status::notConnected;
}

MySocket::Status MySocket::status() const
{
	return m_status;
}

