#include "Utils.h"
#include <vector>

using std::vector;

int main() 
{
    vector<vector<int>> board {
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 1},
        {0, 1, 0, 0, 1},
        {0, 1, 0, 0, 1},
        {0, 1, 0, 0, 1},
    };
    vector<vector<int>> vert_kernel {
        {1},
        {1},
        {1},
        {1},
    };

    vector<vector<int>> output;
}
