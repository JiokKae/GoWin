#pragma once
#include <WinSock2.h>
#include <process.h>
#include <vector>
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
#define MAXCLIENT	1

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

class MySocket {
public:
	enum class Status {
		notConnected,
		Server,
		Client,
	};

	MySocket();
	~MySocket() {}

	SOCKET Create(HWND hWnd);

	bool FD_Accept();
	void FD_Read(SOCKET sock, COMM_MSG* pMsg);
	void FD_Close(SOCKET sock);

	bool Enter(HWND hWnd, char* server_ip);
	int send_message(std::unique_ptr<Message>);
	
	bool IsConnected() const;
	MySocket::Status status() const;

private:
	SOCKET OnAccept(HWND hWnd, SOCKET sockServ);
	void OnClose(SOCKET sock);
	int Send(char* buf, int size);
	int recvn(SOCKET s, char* buf, int len, int flags);

	std::vector<SOCKET> client_sockets;
	WSADATA wsa;
	SOCKET server_socket;
	HWND hWnd;
	Status m_status;
};

