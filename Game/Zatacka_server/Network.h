#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <system_error>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <utility>

#define PORT 2010

class Adatok{
public:
    bool W, E, SPACE, alive;
    short points;
    short bullet_type;
    char addr[20];
    double x, y, angle;

    Adatok(){W=false; E=false; SPACE=false; x=0; y=0; angle=0;
        points=0; bullet_type=0; addr[0]='a'; alive=true;}

};


enum State{
    waiting_players=1,
    warmup=2,
    start=3,
    game=4,
    ended=5
};

class WSASession
{
public:
    WSASession()
    {
        int ret = WSAStartup(MAKEWORD(2, 2), &data);
        if (ret != 0)
            throw std::system_error(WSAGetLastError(), std::system_category(), "WSAStartup Failed");
    }
    ~WSASession()
    {
        WSACleanup();
    }
private:
    WSAData data;
};

class UDPSocket
{
public:
    UDPSocket()
    {
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == INVALID_SOCKET)
            throw std::system_error(WSAGetLastError(), std::system_category(), "Error opening socket");
    }
    ~UDPSocket()
    {
        closesocket(sock);
    }

    void SendTo(const std::string& address, unsigned short port, const char* buffer, int len, int flags = 0)
    {
        sockaddr_in add;
        add.sin_family = AF_INET;
        add.sin_addr.s_addr = *(u_long *)gethostbyname(address.c_str())->h_addr_list[0];
        //std::cout<<inet_ntoa(add.sin_addr);
        add.sin_port = htons(port);
        int ret = sendto(sock, buffer, len, flags, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
        if (ret < 0)
            throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
    }
    void SendTo(sockaddr_in& address, const char* buffer, int len, int flags = 0)
    {
        int ret = sendto(sock, buffer, len, flags, reinterpret_cast<SOCKADDR *>(&address), sizeof(address));
        if (ret < 0)
            throw std::system_error(WSAGetLastError(), std::system_category(), "sendto failed");
    }
    sockaddr_in RecvFrom(char* buffer, int len, int flags = 0)
    {
        sockaddr_in from;
        int size = sizeof(from);
        int ret = recvfrom(sock, buffer, len, flags, reinterpret_cast<SOCKADDR *>(&from), &size);
        if (ret < 0)
            throw std::system_error(WSAGetLastError(), std::system_category(), "recvfrom failed");

        // make the buffer zero terminated
        buffer[ret] = 0;
        return from;
    }
    void Bind(unsigned short port)
    {
        sockaddr_in add;
        add.sin_family = AF_INET;
        add.sin_addr.s_addr = htonl(INADDR_ANY);
        add.sin_port = htons(port);


        int ret = bind(sock, reinterpret_cast<SOCKADDR *>(&add), sizeof(add));
        if (ret < 0)
            throw std::system_error(WSAGetLastError(), std::system_category(), "Bind failed");
    }

private:
    SOCKET sock;
};

void Network(int warmup_sec, State &state, UDPSocket &Socket, WSASession &Session,
             Adatok jatekosok[8], int &player_count, bool send);

int char_to_num(char a);
int get_one_num(char *buff, int idx);
int get_three_num(char *buff, int idx);
int get_four_num(char *buff, int idx);
bool get_bool(char *buff, int idx);


#endif // NETWORK_H_INCLUDED
