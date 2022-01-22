#include "Server.h"
#include "Game.h"
#include <iostream>
#include <cstdio>
#include <winsock2.h>

using std::cout, std::cin, std::endl;

// Constructors and destructor (add player class and delete on destr.)

Server::Server()
{
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    cout << "listening for connections." << endl;
    int clientAddrSize = sizeof(clientAddr);
    if ((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
    {
        cout << "Client connected!" << endl;
    }
};

Server::~Server()
{
    closesocket(client);
    WSACleanup();
    cout << "Thank you for playing!\nPress Enter to quit..." << endl;
    std::getchar();
};

// Methods

/**
 * Listen to server to decide hwho plays first.
 *
 * @return true if we play first, false else.
 */
bool Server::start_playing()
{
    float r = (static_cast<double>(rand()) / RAND_MAX);
    if (r <= 0.5)
    {
        cout << "You play first as 'X'" << endl;
        this->player.start('X');
        strcpy(__self, "-x");
        strcpy(__opp, "-o");
        send(client, __opp, sizeof(__opp), 0);
        return true;
    }
    else
    {
        cout << "You play second as 'O'" << endl;
        this->player.start('O');
        strcpy(__self, "-o");
        strcpy(__opp, "-x");
        send(client, __opp, sizeof(__opp), 0);
        return false;
    }
};

void Server::send_move(int &move)
{
    this->player.make_move(move, 1);
    this->buffer[0] = move;
    send(client, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    system("cls");
};

bool Server::await_move()
{
    cout << "Waiting for the opponent..." << endl;
    recv(client, buffer, sizeof(buffer), 0);
    int opp_move = *buffer;
    this->player.make_move(opp_move, 2);
    memset(buffer, 0, sizeof(buffer));

    system("cls");
    this->player.display_info();
    this->player.display();

    recv(client, STATUS, sizeof(STATUS), 0);
    if (STATUS == __opp || STATUS == "-d")
        return false;
    return true;
};

bool Server::continue_game()
{
    if (this->player.check_win())
    {
        strcpy(STATUS, __self);
        send(client, STATUS, sizeof(STATUS), 0);
        return false;
    }
    else if (this->player.check_game_over())
    {
        strcpy(STATUS, "-d");
        send(client, STATUS, sizeof(STATUS), 0);
        return false;
    }
    send(client, STATUS, sizeof(STATUS), 0);
    return true;
};

void Server::game_over()
{
    if (STATUS == __self)
    {
        cout << "Congrats! You won in " << this->player.turn << " turns.\n";
    }
    else if (STATUS == "-d")
    {
        cout << "It's a draw!\n";
    }
    else if (STATUS == __opp)
    {
        cout << "Sorry, the opponent won.\n";
    }
    else
    {
        cout << "The opponent disconnected, you win.\n";
    }
    return;
};

bool Server::rematch()
{
    if (STATUS == "-p")
        return false;

    cout << "\nRematch? (Y/N): ";
    char host_response[1];
    cin >> host_response;
    toupper(host_response[0]);
    send(client, host_response, sizeof(host_response), 0);

    // if the host wants a rematch, then the client is asked
    if (host_response == "N")
        return false;

    cout << "\nWaiting for the answer of the opponent...";
    char client_response[1];
    recv(client, client_response, sizeof(client_response), 0);

    // if the client wants a rematch, restart the game
    if (client_response == "N")
    {
        cout << "The opponent does not want a rematch.";
        return false;
    }

    return true;
};