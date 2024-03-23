#pragma once
#include <WinSock2.h>
#include <process.h>
#include <vector>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

constexpr int BUFSIZE = 512;
#define MSG_SIZE	(BUFSIZE - sizeof(int))

#define WM_ASYNC	(WM_USER + 1) // 비동기 메시지 정의
constexpr auto MAX_CLIENT = 1;

struct Message {
	enum class Type : int {
		COMMON,
		CHATTING,
		PLACEMENT,
		COMMAND,
	};

	Message(Type type);
	virtual ~Message() = default;

	Type type;
};

struct COMM_MSG : public Message {
	COMM_MSG();

	char dummy[MSG_SIZE];
};

struct CHAT_MSG : public Message {
	CHAT_MSG(LPCTSTR chat);

	TCHAR buf[BUFSIZE/2 - sizeof(int)];
};

struct Placement_MSG : public Message {
	Placement_MSG(int sequence, int x, int y);

	int sequence;
	int x, y;
	char dummy[MSG_SIZE - sizeof(int) * 3];
};

struct Command_MSG : public Message {
	enum class Command : int {
		BACKSIES,
		INIT,
		PASS,
	};

	Command_MSG(Command command);

	Command command;
	char dummy[MSG_SIZE - sizeof(int)];
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

