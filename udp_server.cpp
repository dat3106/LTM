#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>


#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
using namespace std;

int main() {
	// Step 1: Inittiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		cout << "Winsock 2.2 is not supported" << endl;
		return 0;
	}

	// Construct socket
	SOCKET server;
	server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (server == INVALID_SOCKET) {
		cout << "Error " << WSAGetLastError() << ": Cannot create server socket";
		return 0;
	}

	// Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

	if (bind(server, (sockaddr *)&serverAddr, sizeof(serverAddr))) {
		cout << "Error " << WSAGetLastError() << ": Cannot bind this address";
		return 0;
	}

	cout << "Server started!" << endl;

	// Communicate with client
	sockaddr_in clientAddr;
	char buff[BUFF_SIZE], clientIP[INET_ADDRSTRLEN];
	int ret, clientAddrLen = sizeof(clientAddr), clientPort;

	while (1) {
		//Receive message
		ret = recvfrom(server, buff, BUFF_SIZE, 0, (sockaddr *)&clientAddr, &clientAddrLen);
		if (ret == SOCKET_ERROR) {
			cout << "Error " << WSAGetLastError() << ": Cannot receive data.";
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
			clientPort = ntohs(clientAddr.sin_port);
			cout << "Receive from client[" << clientIP << ":" << clientPort << "] " << buff << endl;

			// Echo to client
			ret = sendto(server, buff, strlen(buff), 0, (sockaddr *)&clientAddr, sizeof(clientAddr));
			if (ret == SOCKET_ERROR) {
				cout << "Error " << WSAGetLastError() << ": Cannot send data.";
			}
		}
	}

	// Close socket
	closesocket(server);

	WSACleanup();
	return 0;
}