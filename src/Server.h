#ifndef _SERVER_H_
#define _SERVER_H_
#include "Game.h"
#include <WinSock2.h>

class Server{
private:
    // socket vars
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    char buffer[1024];
    
    // game vars
    char STATUS[1024]; // holds -p while playing, -x/-o on win
    char __self[1024]; // holds -x/-o
    char __opp[1024]; // inverse to __self

public:
    Game player;

    Server();
    ~Server();

    bool start_playing();
    void send_move(int &move);
    bool await_move();
    bool continue_game();
    void game_over();
    bool rematch();

};

#endif // _SERVER_H_