#include "Include.h"


bool Initialize() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void SendMsg(SOCKET s) {
    std::cout << "enter your user name :" << std::endl;
    std::string name;
    std::getline(std::cin, name); 
    std::string message;

    while (true) {
        std::getline(std::cin, message);
        std::string msg = name + " : " + message;
        int bytesent = send(s, msg.c_str(), msg.length(), 0);

        if (bytesent == SOCKET_ERROR) {
            std::cout << "[!] error sending message" << std::endl;
            break;
        }

        if (message == "quit") {
            std::cout << "[!] stopping the application" << std::endl;
            break;
        }
    }
    closesocket(s);
    WSACleanup();
}

void ReciveMsg(SOCKET s) {
    char buffer[4096];
    int recvlength;
    std::string msg = "";

    while (true) {
        recvlength = recv(s, buffer, sizeof(buffer), 0);
        if (recvlength <= 0) {
            std::cout << "[!] disconnected from server" << std::endl;
            break;
        }
        else {
            msg = std::string(buffer, recvlength);
            std::cout << msg << std::endl;
        }
    }
    closesocket(s);
    WSACleanup();
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
        std::cout << ": " << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::cout << "successfully connected to the server" << std::endl;

    std::thread senderthread(SendMsg, s);
    std::thread reciver(ReciveMsg, s);

    senderthread.join();
    reciver.join();

    return 0;
}
//52:16