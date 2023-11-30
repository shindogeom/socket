#define WIN32_LEAN_AND_MEAN

#include<iostream>
#include <Windows.h>
#include <WinSock2.h>
#include<ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(int argc, char** argv)
{
	WSADATA wsa;
	SOCKET s;

	struct sockaddr_in server;
	std::string message;
	char server_reply[5000] = { 0 };
	int recv_size;

	//UNICODE Locale을 한국어로 설정
	std::wcout.imbue(std::locale("kor"));
	std::wcin.imbue(std::locale("kor"));

	std::wcout << L"Intializing Winsock ..." << std::endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::wcout << L"Failed. Error code : " << WSAGetLastError() << std::endl;
	

		return 1;
	}
 
	std::wcout << L"Initialized" << std::endl;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::wcout << L"Could not create socket : " << WSAGetLastError() << std::endl;
		WSACleanup();

		return 1;
	}

	std::wcout << L"Socket Created" << std::endl;
	server.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &server.sin_addr.s_addr);
	server.sin_port = htons(8888);

	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		std::wcout << L"Connect error: " << WSAGetLastError() << std::endl;

		return 1;
	}

	std::wcout << L"Connected" << std::endl;
	
	message = "Get / HTTP/1.1 \\r\\n\\r\\n";

	if (send(s, message.c_str(), message.size(), 0) < 0)
	{
		std::wcout << L"Send Failed : " << WSAGetLastError() << std::endl;

		return 1;
	}

	std::wcout << L"Data Send" << std::endl;

	if ((recv_size = recv(s, server_reply, 4999, 0)) == SOCKET_ERROR)
	{
		std::wcout << L"Recv Failed" << std::endl;
	}
	

	std::wcout << L"Reply Received" << std::endl;
	std::wcout << server_reply << std::endl;

	closesocket(s);
	WSACleanup();

	return 0;
}