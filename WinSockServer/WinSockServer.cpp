#include "Include.h"

void InteractWithClient(SOCKET clientSocket){
    //send/recv cleint
    std::cout << "client connected" << std::endl;
    char buffer[4069];

    while (true) {
        int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesrecvd <= 0) {
            std::cout << "client disconnected" << std::endl;
            break;
        }

        std::string message(buffer, bytesrecvd);
        std::cout << "message from client : " << message << std::endl;
    }

    closesocket(clientSocket);
}

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

    std::cout << "server app\n";

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (listenSocket == INVALID_SOCKET) {
        std::cout << "[!] socket creation failed" << std::endl;
        return 0; 
    }

    /*create adress structure*/
    int port = 12345; //port the server will listen
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port); 

    //convert the ip addr
    if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
        std::cout << "[!] setting adress structure failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 0;
    }

    //bind shit togeather
    if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
        std::cout << "[!] bind failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 0;
    }

    //listen
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "[!] listen failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 0;
    }

    std::cout << "server has started listening on port : " << port << std::endl;
    std::vector<SOCKET> cleints;

    while (true) {
        //accept
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "[!] invalid client socket" << std::endl;
        }

        cleints.push_back(clientSocket);
        std::thread t1(InteractWithClient, clientSocket);
    }

    closesocket(listenSocket);

    WSACleanup();
    return 0;
}
//41:07