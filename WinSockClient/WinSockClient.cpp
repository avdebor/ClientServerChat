#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32.lib")


bool Initialize() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

int main()
{
    if (!Initialize()) {
        std::cout << "[!] winsock initialization failed" << std::endl;
        return 0;
    }

    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);

    
    if (s == INVALID_SOCKET) {
        std::cout << "[!] invalid socket created" << std::endl;
        return 0;
    }

    //port and adress the client will connect to
    int port = 12345;
    std::string serveraddress = "127.0.0.1";
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));

    if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
        std::cout << "[!] not able to connect to server" << std::endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::cout << "successfully connected to the server" << std::endl;

    //send/recive
    std::string message = "hello there";
    int bytesent;
    bytesent = send(s, message.c_str(), message.length(), 0);
    if (bytesent == SOCKET_ERROR) {
        std::cout << "[!] send failed" << std::endl;
    }

    closesocket(s);

    WSACleanup();
    return 0;
}
//41:07