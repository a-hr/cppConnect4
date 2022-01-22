#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "Game.h"
#include <winsock2.h>


class Client{
private:
    // socket vars
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];
    
    // game vars
    char STATUS[1024]; // holds -p while playing, -x/-o on win
    char __self[1024]; // holds -x/-o
    char __opp[1024]; // inverse to __self

public:
    Game player;
    
    Client();
    Client(char &host_ip);
    ~Client();

    bool start_playing();
    void send_move(int &move);
    bool await_move();
    bool continue_game();
    void game_over();
    bool rematch();

};

#endif // _CLIENT_H_