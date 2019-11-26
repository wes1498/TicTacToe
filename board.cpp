#include <cstdio>
#include "board.h"
#include <iostream> 

// Game will use an OOP MVC pattern -- View(Board) Model(Tic-Tac-Toe) Controller(Game)
void Board::draw_board(char board[][3])
{
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("------------\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("------------\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);

}

