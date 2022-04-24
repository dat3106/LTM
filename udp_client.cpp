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
	SOCKET client;
	client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client == INVALID_SOCKET) {
		cout << "Error " << WSAGetLastError() << ": Cannot create client socket";
		return 0;
	}

	cout << "Client started!" << endl;

	// Specify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);


	//Communicate with server
	char buff[BUFF_SIZE];
	int ret, serverAddrLen = sizeof(serverAddr), messageLen;
	while (1) {
		//Send message
		cout << "Send to server: ";
		gets_s(buff, BUFF_SIZE);
		messageLen = strlen(buff);
		if (messageLen == 0) break;
		

		ret = sendto(client, buff, messageLen, 0, (sockaddr *)&serverAddr, serverAddrLen);
		if (ret == SOCKET_ERROR) {
			cout << "Error " << WSAGetLastError() << ": Cannot send message.";
		}

		//Receive echo message
		ret = recvfrom(client, buff, BUFF_SIZE, 0, (sockaddr *)&serverAddr, &serverAddrLen);

		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT) {
				cout << "Time out!";
			}
			else {
				cout << "Error " << WSAGetLastError() << ": Cannot receive message";
			}
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			cout << "Receive from server: " << buff << endl;
		}
	}

	//close socket
	closesocket(client);

	WSACleanup();
	return 0;
}