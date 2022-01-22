#include "Client.h"
#include "Game.h"
#include "Utils.h"
#include <iostream>
// 192.168.86.112
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    Client client{};
    while (true)
    {
        bool first = client.start_playing();
        if (first)
        {
            client.player.display_info();
            client.player.display();
        }
        else
        {
            client.await_move();
        }
        while (true)
        {
            int move = enter_move();
            client.send_move(move);
            if (!client.continue_game())
                break;
            if (!client.await_move())
                break;
        }

        client.game_over();

        if (!client.rematch())
            break;
    }
}