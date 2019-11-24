#pragma once

class Board
{
    public:
        Board() = default;
        virtual ~Board() = default;
        void Draw(char board[0][3]);
};
