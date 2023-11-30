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
	SOCKET s, newSocket;  
	struct  sockaddr_in server, client;
	
	int c;
	std::string messegae;

	//UNICODE Locale을 한국어로 설정
	std::wcout.imbue(std::locale("kor"));
	std::wcin.imbue(std::locale("kor"));

	std::wcerr << std::endl << L"Initializing WinSock..." << std::endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::wcout << L"Fail. Error Code: " << WSAGetLastError() << std::endl;
		WSACleanup();

		return 1;
	}

	std::wcout << L"Initialized" << std::endl;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::wcout << L"Could not create socket : " << WSAGetLastError() << std::endl;
		WSACleanup();

		return 1;
	}

	std::wcout << L"Socket Created." << std::endl;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::wcout << L"Bind Failed with error code : " << WSAGetLastError() << std::endl;
		WSACleanup();

		return 1;
	}
	std::wcout << L"Bind done" << std::endl;

	listen(s, 3);
	std::wcout << L"Waiting for incoming connections ..." << std::endl;

	c = sizeof(struct sockaddr_in);
	char clientIP[INET_ADDRSTRLEN];

	while ((newSocket = accept(s, (struct sockaddr*)&client, &c)) != INVALID_SOCKET)
	{
		std::cout << L"Connection accepted" << std::endl;

		inet_ntop(client.sin_family, &client.sin_addr, clientIP, INET_ADDRSTRLEN);

		std::wcout << L"Client Address: " << clientIP << std::endl;
	
		messegae = "Hello Client I have received your connection. But I have to go now, bye.";
		send(newSocket, messegae.c_str(), messegae.size(), 0);
	}

	if (newSocket == INVALID_SOCKET) {
		std::cout << L"Accept failed with error code." << WSAGetLastError() << std::endl;

		return 1;
	}

	closesocket(s);
	WSACleanup();

	return 0;
}