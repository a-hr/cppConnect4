#include "Game.h"
#include <vector>
#include <string>
#include <iostream>

using std::cout;
using std::endl;

Game::Game(){};

Game::~Game(){};

int Game::count(char sym)
{
    int cnt = 0;
    for (auto &row : board)
    {
        for (auto &elem : row)
        {
            if (elem == sym)
                ++cnt;
        }
    }
    return cnt;
}

void Game::start(char player)
{
    this->turn = 1;
    this->player = player;
    this->opponent = (player == 'X') ? 'O' : 'X';

    for (size_t i{0}; i < 7; i++)
    {
        std::vector<char> row;
        for (size_t j{0}; j < 10; j++)
            row.push_back(' ');
        this->board.push_back(row);
    }
};

bool Game::make_move(int col, int who)
{
    int ncol = board[0].size();
    if (col >= ncol)
        return false;

    int idx = 0;
    for (auto &row : board)
        if (row[col] == ' ')
            ++idx;
    if (idx == 0)
        return false;

    char sym[]{' '};

    if (who == 1)
    {
        *sym = player;
        ++turn;
    }
    else
    {
        *sym = opponent;
    }

    board[idx - 1][col] = *sym;
    return true;
};

bool Game::check_win()
{
    return false;
};

bool Game::check_game_over()
{
    if (count(' ') == 0)
        return true;
    return false;
};

void Game::display()
{
    size_t ncol = this->board[0].size();

    // header
    cout << "   ";
    for (size_t col = 0; col < ncol; col++)
        cout << "  ║  " << col;

    // board
    for (auto &row : board)
    {
        cout << "\n═════";
        for (size_t j = 0; j < ncol; j++)
            cout << "╬═════";

        cout << "\n   ";
        for (auto &elem : row)
            cout << "  ║  " << elem;
    }
};

void Game::display_info()
{
    cout << "You play! Current turn: " << this->turn << endl;
}