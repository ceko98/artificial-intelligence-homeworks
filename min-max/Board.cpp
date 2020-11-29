#ifndef __BOARD__
#define __BOARD__

#include <iostream>
#include <vector>

class Board
{
private:

public:
    char board[3][3] = {{'_', '_', '_'}, {'_', '_', '_'}, {'_', '_', '_'}};

    Board();
    void setPosition(int i, int j, char marker);
    void printBoard();
    char winner();
    bool checkFor(char c);
    bool boardFull();
    std::vector<std::vector<int>> getFree(); 
};

void Board::setPosition(int i, int j, char marker) {
    board[i][j] = marker;
};

bool Board::checkFor(char c)
{
    bool mainDiag = true, offDiag = true;       
    for (size_t i = 0; i < 3; i++)
    {
        bool hasRow = true, hasColumn = true;
        for (size_t j = 0; j < 3; j++)
        {
            hasRow = board[i][j] == c;
            hasColumn = board[i][j] == c;
        }
        if (hasRow || hasColumn) {
            return true;
        }
        mainDiag = board[i][i] == c;
        offDiag = board[i][i] == c;
    }
    return mainDiag || offDiag;
}

bool Board::boardFull()
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if (board[i][j] == '_') return false;
        }
    }
    return true;
}

char Board::winner()
{
    if (checkFor('X')) return 'X';    
    if (checkFor('O')) return 'O';
    return boardFull() ? 'D' : '_';
}

std::vector<std::vector<int>> Board::getFree()
{
    std::vector<std::vector<int>> result;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result.push_back({i, j});
        }
    }
    return result;
}

void Board::printBoard(){
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

#endif