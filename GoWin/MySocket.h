#pragma once
#include <WinSock2.h>
#include <process.h>
//#include <stdio.h>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE		512
#define MSGSIZE		(BUFSIZE - sizeof(int))
#define CHATTING	1000
#define PLACEMENT	1001
#define COMMAND		1002

#define BACKSIES	10021
#define INIT		10022
#define PASS		10023

#define WM_ASYNC	(WM_USER+1) // 비동기 메시지 정의
#define MAXCLIENT	2

struct Message {
	Message(int type);
	virtual ~Message() = default;
	int type;
};

struct COMM_MSG : public Message {
	COMM_MSG();
	char dummy[MSGSIZE];
};

struct CHAT_MSG : public Message {
	CHAT_MSG(LPCTSTR chat);
	TCHAR buf[BUFSIZE/2 - sizeof(int)];
};

struct Placement_MSG : public Message {
	Placement_MSG(int sequence, int x, int y);
	int sequence;
	int x, y;
	char dummy[MSGSIZE - sizeof(int) * 3];
};

struct Command_MSG : public Message {
	Command_MSG(int command);
	int command;
	char dummy[MSGSIZE - sizeof(int)];
};

class MySocket : public Message {
public:
	// 소켓 상태
	enum class Status {
		notConnected,
		Server,
		Client,
	};

	MySocket();
	~MySocket() {}

	SOCKET Create(HWND hWnd);
	
	SOCKET OnAccept(HWND hWnd, SOCKET sockServ);
	bool FD_Accept();

	void FD_Read(SOCKET sock, COMM_MSG* pMsg);
	void OnClose(SOCKET sock);
	void FD_Close(SOCKET sock);


	bool Enter(HWND hWnd, char* server_ip);
	int Send(char* buf, int size);
	int send_message(std::unique_ptr<Message>);
	
	int CurrentAcceptIndex() { return current_accept_index; }
	
	bool IsConnected() const;
	MySocket::Status status() const;

private:
	int recvn(SOCKET s, char* buf, int len, int flags);

	char server_ip[18] = "";
	WSADATA wsa;
	int current_accept_index = 0;
	SOCKET client_sockets[MAXCLIENT];
	SOCKET server_socket;
	HWND hWnd;
	Status m_status;
};

