#include <iostream>
#include <vector>

using std::vector;

int enter_move()
{
    int move{-1};

    do
    {
        std::cout << "\nEnter next move: ";
        std::cin >> move;
        if (move < 0 || move > 9)
        {
            std::cout << "\nInvalid move!";
            move = -1;
        }
    } while (move == -1);

    return move;
}

void convolute(vector<vector<int>> &output, vector<vector<int>> &input, vector<vector<int>> &kernel)
{
    int convolute = 0; // This holds the convolution results for an index.
    int x, y;          // Used for input matrix index

    // Fill output matrix: rows and columns are i and j respectively
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            x = i;
            y = j;

            // Kernel rows and columns are k and l respectively
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    // Convolute here.
                    convolute += kernel[k][l] * input[x][y];
                    y++; // Move right.
                }
                x++;   // Move down.
                y = j; // Restart column position
            }
            output[i][j] = convolute; // Add result to output matrix.
            convolute = 0;            // Needed before we move on to the next index.
        }
    }
}