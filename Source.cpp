#include<WinSock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
WSADATA wsaData;
WORD wVersion = MAKEWORD(2, 2);

int main() {
	WSAStartup(wVersion, &wsaData);
	addrinfo *result;

	int rc;
	sockaddr_in *address;

	addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	rc = getaddrinfo("soict.hust.edu.vn", NULL, &hints, &result);
	char ipStr[INET6_ADDRSTRLEN];

	if (rc == 0) {
		address = (struct sockaddr_in *) result->ai_addr;
		inet_ntop(AF_INET, &address->sin_addr, ipStr, sizeof(ipStr));
		printf("IPv4 address: %s\n", ipStr);
	}
	else {
		printf("Failed. Error code: %d", WSAGetLastError());
	}
	freeaddrinfo(result);
	return 0;
}
