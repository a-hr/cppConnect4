#ifndef _GAME_H_
#define _GAME_H_
#include <vector>

using std::vector;

class Game
{
private:
    vector<vector<char>> board;
    char player;
    char opponent;

    int count(char sym);

public:
    int turn;
    
    Game();
    ~Game();

    void start(char player);
    bool make_move(int col, int who);
    bool check_win();
    bool check_game_over();
    void display();
    void display_info();
};

#endif // _GAME_H_