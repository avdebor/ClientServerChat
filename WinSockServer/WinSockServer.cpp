#include "Include.h"

//global handles
HANDLE hConsole;

void InteractWithClient(SOCKET clientSocket, std::vector<SOCKET>& cleints){
    //send/recv cleint
    SetConsoleTextAttribute(hConsole, lightGray);
    std::cout << "client connected" << std::endl;
    SetConsoleTextAttribute(hConsole, White);
    char buffer[4069];

    while (true) {
        int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesrecvd <= 0) {
            SetConsoleTextAttribute(hConsole, Yellow);
            std::cout << "client disconnected" << std::endl;
            SetConsoleTextAttribute(hConsole, White);
            break;
        }

        std::string message(buffer, bytesrecvd);
        std::cout << message << std::endl;

        for (auto client : cleints) {
            if (client != clientSocket) {
                send(client, message.c_str(), message.length(), 0);

            }
        }
    }

    auto it = find(cleints.begin(), cleints.end(), clientSocket);
    if (it != cleints.end()) {
        cleints.erase(it);
    }

    closesocket(clientSocket);
}

bool Initialize() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

int main()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!Initialize()) {
       SetConsoleTextAttribute(hConsole, Red);
       std::cout << "[!] winsock initialization failed" << std::endl;
       return 0;
    }


    SetConsoleTextAttribute(hConsole, Green);
    std::cout << "server app is running" << std::endl;
    SetConsoleTextAttribute(hConsole, White);

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (listenSocket == INVALID_SOCKET) {
        SetConsoleTextAttribute(hConsole, Red);
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
        SetConsoleTextAttribute(hConsole, Red);
        std::cout << "[!] setting adress structure failed" << std::endl;
        SetConsoleTextAttribute(hConsole, White);
        closesocket(listenSocket);
        WSACleanup();
        return 0;
    }

    //bind shit togeather
    if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
        SetConsoleTextAttribute(hConsole, Red);
        std::cout << "[!] bind failed" << std::endl;
        SetConsoleTextAttribute(hConsole, White);
        closesocket(listenSocket);
        WSACleanup();
        return 0;
    }

    //listen
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        SetConsoleTextAttribute(hConsole, Red);
        std::cout << "[!] listen failed" << std::endl;
        SetConsoleTextAttribute(hConsole, White);
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
            SetConsoleTextAttribute(hConsole, Red);
            std::cout << "[!] invalid client socket" << std::endl;
            SetConsoleTextAttribute(hConsole, White);
        }

        cleints.push_back(clientSocket);
        std::thread t1(InteractWithClient, clientSocket, std::ref(cleints));
        t1.detach();
    }

    closesocket(listenSocket);

    WSACleanup();
    return 0;
}
//52:16