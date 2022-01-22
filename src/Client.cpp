#include "Client.h"
#include "Game.h"
#include <iostream>
#include <cstdio>
#include <winsock2.h>

using std::cout, std::cin, std::endl;

// Constructors and destructor (add player class and delete on destr.)

Client::Client()
{
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    char ip[16];
    cout << "Enter server IP: ";
    cin >> ip;

    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    cout << "Connected to server!" << endl;
};

Client::Client(char &host_ip)
{
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr(&host_ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    cout << "Connected to server!" << endl;
};

Client::~Client()
{
    closesocket(server);
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
bool Client::start_playing()
{

    strcpy(STATUS, "-p");
    char self[1024];
    recv(server, self, sizeof(self), 0);
    if (self == "-x")
    {
        cout << "You play first as 'X'" << endl;
        this->player.start('X');
        strcpy(__self, "-x");
        strcpy(__opp, "-o");
        return true;
    }
    else
    {
        cout << "You play second as 'O'" << endl;
        this->player.start('O');
        strcpy(__self, "-o");
        strcpy(__opp, "-x");
        return false;
    }
};

/**
 * Sends a move to the server.
 * 
 * @param move the column number selected by the player.
 */
void Client::send_move(int &move)
{
    this->player.make_move(move, 1);
    this->buffer[0] = move;
    send(server, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    system("cls");
};

/**
 * Receives and applies the ooponents move..
 * 
 * @returns false if the game ended with this last movement, true else.
 */
bool Client::await_move()
{
    cout << "Waiting for the opponent..." << endl;
    recv(server, buffer, sizeof(buffer), 0);
    int opp_move = *buffer;
    this->player.make_move(opp_move, 2);
    memset(buffer, 0, sizeof(buffer));

    system("cls");
    this->player.display_info();
    this->player.display();

    recv(server, STATUS, sizeof(STATUS), 0);
    if (STATUS == __opp || STATUS == "-d")
        return false;
    return true;
};

/**
 * Checks if the game is continuable and answers to the server.
 * 
 * @returns true if the game continues, false else.
 */
bool Client::continue_game()
{
    if (this->player.check_win())
    {
        strcpy(STATUS, __self);
        send(server, STATUS, sizeof(STATUS), 0);
        return false;
    }
    else if (this->player.check_game_over())
    {
        strcpy(STATUS, "-d");
        send(server, STATUS, sizeof(STATUS), 0);
        return false;
    }
    send(server, STATUS, sizeof(STATUS), 0);
    return true;
};

/**
 * Displays the final result of the game.
 */
void Client::game_over()
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

/**
 * @brief Manages the rematch decision.
 * 
 * @return true if there is rematch.
 * @return false if any player does not want to rematch.
 */
bool Client::rematch()
{
    if (STATUS == "-p")
        return false;
        
    cout << "\nWaiting for host...";
    char host_response[1];
    recv(server, host_response, sizeof(host_response), 0);

    // if the host wants a rematch, then the client is asked
    if (host_response == "N")
    {
        cout << "\nThe host does not want a rematch.";
        return false;
    }

    cout << "\nThe host would like a rematch!";
    char client_response[1];
    cout << "\nRematch? (Y/N): ";
    cin >> client_response;
    toupper(client_response[0]);

    send(server, client_response, sizeof(client_response), 0);

    // if the client wants a rematch, restart the game
    if (client_response == "N")
        return false;

    return true;
};