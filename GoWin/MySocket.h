#pragma once
#include <WinSock2.h>
#include <process.h>
#include <vector>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

struct Message {
	enum class Type : int {
		COMMON,
		CHATTING,
		PLACEMENT,
		COMMAND,
	};

	static constexpr int BUF_SIZE = 512;
	static constexpr int MSG_SIZE = Message::BUF_SIZE - sizeof(Message::Type);

public:
	Message(Type type);

	Type type = Type::COMMON;
};

struct COMM_MSG : public Message {
	COMM_MSG();

	char dummy[Message::MSG_SIZE]{};
};

struct CHAT_MSG : public Message {
	static constexpr int DUMMY_MIN_SIZE = 1;
	static constexpr int CHAT_SIZE = Message::MSG_SIZE / sizeof(TCHAR) - DUMMY_MIN_SIZE;

public:
	CHAT_MSG(LPCTSTR chat);

	TCHAR buf[CHAT_MSG::CHAT_SIZE];
	char dummy[Message::MSG_SIZE - sizeof(TCHAR) * CHAT_MSG::CHAT_SIZE]{};
};

struct Placement_MSG : public Message {
	Placement_MSG(int sequence, int x, int y);

	int sequence;
	int x, y;
	char dummy[Message::MSG_SIZE - sizeof(int) * 3]{};
};

struct Command_MSG : public Message {
	enum class Command : int {
		BACKSIES,
		INIT,
		PASS,
	};

public:
	Command_MSG(Command command);

	Command command;
	char dummy[Message::MSG_SIZE - sizeof(Command)]{};
};

class MySocket {
public:
	static constexpr auto WM_ASYNC = WM_USER + 1; // 비동기 메시지 정의
	enum class Status {
		notConnected,
		Server,
		Client,
	};

public:
	MySocket();

	bool Create(HWND hWnd);

	bool FD_Accept();
	void FD_Read(SOCKET sock, COMM_MSG* pMsg);
	void FD_Close(SOCKET sock);

	bool Enter(HWND hWnd, char* server_ip);
	bool send_message(std::unique_ptr<Message>);
	
	bool IsConnected() const;
	MySocket::Status status() const;

private:
	void CloseServerSocketImpl(const char* func_name, const char* error_message);

	SOCKET OnAccept(HWND hWnd, SOCKET sockServ);
	void OnClose(SOCKET sock);
	int Send(char* buf, int size);
	int recvn(SOCKET s, char* buf, int len, int flags);

	std::vector<SOCKET> client_sockets{};
	WSADATA wsa{};
	SOCKET m_server_socket{ INVALID_SOCKET };
	HWND m_hWnd{ nullptr };
	Status m_status{ MySocket::Status::notConnected };

private:
	static constexpr int MAX_CLIENT = 1;
	static constexpr int SERVER_PORT = 4777;
};
