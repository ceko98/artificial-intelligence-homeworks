#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

enum Direction
{
    Left = 1,
    Right = 2,
    Up = 3,
    Down = 4,
};

const char * getDirection(Direction direction) {
    switch (direction)
    {
    case Left:
        return "right";
    case Right:
        return "left";
    case Up:
        return "down";
    case Down:
        return "up";
    default:
        break;
    }
    return nullptr;
}

class GameNode
{
    int **board;
    int size;
    std::pair<int, int> *empty;

public:
    long long id;

    GameNode(int size, int **board);
    ~GameNode();
    GameNode * getNeighbour(Direction direction);
    int **boardCopy();
    int *flattenBoard();
    int manhattan();
    bool isSolvable();
    bool operator==(GameNode &other);
};

GameNode::GameNode(int size, int **board) : size(size), board(board), id(0)
{
    int mask = 15;
    for (size_t i = 0; i < this->size; i++)
    {
        for (size_t j = 0; j < this->size; j++)
        {
            if (this->board[i][j] == 0)
            {
                this->empty = new std::pair<int, int>(i, j);
            }
            id = ((id << 4) | (this->board[i][j] & mask));
        }
    }
}

GameNode::~GameNode() {
    for (int i = 0; i < this->size; i++)
    {
        delete [] this->board[i];
    }
    delete [] this->board;
}

GameNode *GameNode::getNeighbour(Direction direction)
{
    int **copy = this->boardCopy();
    bool canGet = false;
    switch (direction)
    {
    case Left:
        if (empty->second - 1 >= 0) {
            std::swap(copy[empty->first][empty->second], copy[empty->first][empty->second - 1]);
            canGet = true;
        }
        break;
    case Right:
        if (empty->second + 1 < this->size) {
            std::swap(copy[empty->first][empty->second], copy[empty->first][empty->second + 1]);
            canGet = true;
        }
        break;
    case Up:
        if (empty->first - 1 >= 0) {
            std::swap(copy[empty->first][empty->second], copy[empty->first - 1][empty->second]);
            canGet = true;
        }
        break;
    case Down:
        if (empty->first + 1 < this->size) {
            std::swap(copy[empty->first][empty->second], copy[empty->first + 1][empty->second]);
            canGet = true;
        }
        break;
    }

    if (!canGet) {
        return nullptr;
    }
    return new GameNode(this->size, copy);
}

bool GameNode::isSolvable() {
    int * flatted = this->flattenBoard();
    int inversions = 0;
    for (size_t i = 0; i < this->size * this->size - 1; i++)
    {
        for (size_t j = i + 1; j < this->size * this->size; j++)
        {
            if (flatted[i] > flatted[j] && flatted[i] != 0 && flatted[j] != 0) {
                inversions++;
            }
        }
    }
    delete [] flatted;
    return this->size % 2
        ? inversions % 2 == 0
        : (inversions + this->empty->first) % 2 == 1;
}

int GameNode::manhattan() {
    int sum = 0;
    for (size_t i = 0; i < this->size; i++)
    {
        for (size_t j = 0; j < this->size; j++)
        {
            int boardValue = this->board[i][j] == 0 ? this->size * this->size : this->board[i][j];
            int x = (boardValue - 1) / this->size, y = (boardValue - 1) % this->size; 
            sum += abs(x-i) + abs(y-j);
        }
    }
    return sum;
}

int **GameNode::boardCopy()
{
    int **newBoard = new int *[this->size];
    for (size_t i = 0; i < this->size; i++)
    {
        newBoard[i] = new int[this->size];
        for (size_t j = 0; j < this->size; j++)
        {
            newBoard[i][j] = this->board[i][j];
        }
    }
    return newBoard;
}

int *GameNode::flattenBoard() {
    int * flatted = new int[this->size * this->size];
    for (size_t i = 0; i < this->size; i++)
    {
        for (size_t j = 0; j < this->size; j++)
        {
            flatted[i * this->size + j] = this->board[i][j];
        }
    }
    return flatted;
}

bool GameNode::operator==(GameNode &other) {
    for (size_t i = 0; i < this->size; i++)
    {
        for (size_t j = 0; j < this->size; j++)
        {
            if (this->board[i][j] != other.board[i][j]) {
                return false;
            }
        }
    }
    return true;
}
