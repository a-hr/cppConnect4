#include "Server.h"
#include "Game.h"
#include "Utils.h"
#include <iostream>

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    Server server{};
    while (true)
    {
        bool first = server.start_playing();
        if (first)
        {
            server.player.display_info();
            server.player.display();
        }
        else
            server.await_move();

        while (true)
        {
            int move = enter_move();
            server.send_move(move);
            if (!server.continue_game())
                break;
            if (!server.await_move())
                break;
        }

        server.game_over();

        if (!server.rematch())
            break;
    }
}