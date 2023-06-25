#include "NetworkServer.h"

std::string NetworkServer::MessageConstructor(std::string text)
{
    std::string message = "[*] " + _name + " : " + text;
    return message;
}
int NetworkServer::History()
{
    for (auto& hist : _history)
        std::cout << hist << std::endl;
    return size_t();
}

int NetworkServer::HistoryLast()
{
    std::cout << _history.back() << std::endl;
    return size_t();
}

NetworkServer::NetworkServer()
{
    _iResult = WSAStartup(MAKEWORD(2, 2), &_WSADATA);
}

NetworkServer::~NetworkServer()
{
    closesocket(clientSock_);
    WSACleanup();
}


int NetworkServer::Listen() 
{
    if (_iResult != 0)
    {
        std::cout << "[Server WSAStartup error]: " << _iResult << std::endl;
        return 1;
    }

    listenAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    listenAddr_.sin_family = 2;
    listenAddr_.sin_port = htons(PORT);
    listenSock_ = socket(2, 1, 0);
    if (listenSock_ == -1)
    {
        std::cout << "[создание socket error]" << std::endl;
        return 1;
    }
    _iResult = bind(listenSock_, (struct sockaddr*)&listenAddr_, sizeof(listenAddr_));
    if (_iResult == -1)
    {
        std::cout << "[bind error]" << std::endl;
        closesocket(listenSock_);
        return 1;
    }
    _iResult = listen(listenSock_, 5);
    if (_iResult == -1)
    {
        std::cout << "[listen error]" << std::endl;
        closesocket(listenSock_);
        return 1;
    }
    clientSock_ = accept(listenSock_, 0, 0);
    if (clientSock_ == -1)
    {
        std::cout << "[accept error]" << std::endl;
        closesocket(listenSock_);
        return 1;
    }
    closesocket(listenSock_);
    return 0;
}

int NetworkServer::Read()
{
    _iResult = recv(clientSock_, _R_BUF, sizeof(_R_BUF), 0);
    std::string str;
    if (_iResult <= 0)
    {
        std::cout << "[recv error]" << std::endl;
        return 1;
    }
    else
    {
        str = std::string(_R_BUF);
        if (str == "exit")
        {
            std::cout << "[connection closed]" << std::endl; 
            return 1;
        }
    }

    _history.push_back(str);
    memset(_R_BUF, 0, sizeof(_R_BUF));
    return 0;
}

int NetworkServer::Write(const std::string& message)
{
    std::string str;
    if (message != "exit")
        str = MessageConstructor(message);
    else
        str = message;
    _history.push_back(str);
    strcpy_s(_R_BUF, str.c_str());

    _iResult = send(clientSock_, _R_BUF, sizeof(_R_BUF), 0);
    if (_iResult <= 0)
    {
        std::cout << "[send error]";
        return 1;
    }
    memset(_R_BUF, 0, sizeof(_R_BUF));
    if (message == "exit") return 1;
    
    return 0;
}

int NetworkServer::Shutdown()
{
    _iResult = shutdown(clientSock_, 0x01);
    _history.clear();
    if (_iResult == -1)
    {
        std::cout << "[shutdown error]" << std::endl;
        _iResult = 0;
        return 1;
    }
    return 0;
}

