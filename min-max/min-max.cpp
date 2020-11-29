// #include "Board.cpp"
#include <iostream>
#include <vector>

class Board
{
private:
// X X O 
// _ O _ 
// _ O _ 

// X O X 
// _ O X 
// O _ _

public:
    char board[3][3] = {{'_', '_', '_'}, {'_', '_', '_'}, {'_', '_', '_'}};
    // char board[3][3] = {{'X', 'O', 'X'}, {'O', 'O', 'X'}, {'_', '_', '_'}};
    // char board[3][3] = {{'X', 'O', 'X'}, {'O', 'O', 'X'}, {'_', '_', 'X'}};
    // char board[3][3] = {{'X', 'O', 'X'}, {'_', 'O', '_'}, {'O', '_', '_'}};

    Board() {};
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
            hasRow = board[i][j] == c && hasRow;
            hasColumn = board[j][i] == c && hasColumn;
        }
        if (hasRow || hasColumn) {
            return true;
        }
        mainDiag = board[i][i] == c && mainDiag;
        offDiag = board[2 - i][i] == c && offDiag;
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
            if (board[i][j] == '_')
            {
                result.push_back({i, j});
            }
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

class Game
{
private:
    const char ai = 'X';
    const char human = 'O';
    Board playBoard;

public:
    Game() {};
    int evalMove(Board board, char player, char perspective);
    void play();
};

int Game::evalMove(Board board, char player, char perspective)
{
    // board.printBoard();
    const char winner = board.winner();
    if (board.winner() != '_')
    {
        // std::cout << winner << std::endl;
        return winner == 'D' ? 0 : winner == perspective ? 1 : -1;
    }

    if (player == ai)
    {
        int result = -2;
        for (auto cord : board.getFree())
        {
            Board next(board);
            next.setPosition(cord[0], cord[1], player);
            result = std::max(result, evalMove(next, human, perspective));
        }
        return result;
    }
    else
    {
        int result = 2;
        for (auto cord : board.getFree())
        {
            Board next(board);
            next.setPosition(cord[0], cord[1], player);
            result = std::min(result, evalMove(next, ai, perspective));
        }
        return result;
    }
}

void Game::play()
{
    for (;;)
    {
        int i, j;
        std::cin >> i >> j;

        playBoard.setPosition(i, j, human);
        playBoard.printBoard();
        // std::cout << playBoard.winner() << std::endl;
        if (playBoard.winner() != '_')
            return;

        int maxMoveValue = -2;
        int maxI = -1, maxJ = -1;
        int index = 2;
        for (auto cord : playBoard.getFree())
        {
            // std::vector<int> cord = {1,2};
            Board next(playBoard);
            // std::cout << cord[0] << cord[1] << std::endl;
            next.setPosition(cord[0], cord[1], ai);
            int moveValue = evalMove(next, human, ai);
            // std::cout << moveValue << std::endl;
            // next.printBoard();
            if (moveValue > maxMoveValue)
            {
                maxMoveValue = moveValue;
                maxI = cord[0];
                maxJ = cord[1];
            }
            index--;
            // if (index == 0) break;
            // std::cout << "-------------------\n";
        }
        playBoard.setPosition(maxI, maxJ, ai);
        playBoard.printBoard();
        if (playBoard.winner() != '_')
            return;
    }
}

int main()
{

    Game g;
    g.play();
    return 0;
}