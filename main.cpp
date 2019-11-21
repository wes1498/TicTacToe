#include <iostream>
#include "board.h"

using namespace std;

int Play();

int main() {
    cout << "Welcome to Tic Tac Toe!\n\n";
    Play();
}
int Play() {
    char board[3][3] = { {' ', ' ', ' '}, /* Game Board */ 
                         {' ', ' ', ' '}, 
                         {' ', ' ', ' '} };
    Board b;
    b.Draw(board);
    return 0;
}