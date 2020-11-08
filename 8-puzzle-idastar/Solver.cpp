#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <stack>
#include <functional>
#include <limits>
#include "GameNode.cpp"

class Solver
{
private:
    int boardSize;
    int goalPostion;
    GameNode *initialState;
    GameNode *goal;
    std::vector<char *> path;

    void readInitialState();
    void setGoalState();
    int search(GameNode *node, int g, int threshhold, std::vector<GameNode *> &visited);
    bool isVisited(GameNode *node, std::vector<GameNode *> &visited);

public:
    Solver();
    void solve();
};

Solver::Solver()
{
    this->readInitialState();
    this->setGoalState();
}

void Solver::readInitialState()
{
    int tilesCount, goalIndex;
    std::cin >> tilesCount;
    this->boardSize = std::sqrt(tilesCount + 1);
    std::cin >> goalIndex;
    this->goalPostion = goalIndex == -1 ? tilesCount + 1 : goalIndex;

    int **tmpBoard = new int *[this->boardSize];
    for (size_t i = 0; i < this->boardSize; i++)
    {
        tmpBoard[i] = new int[this->boardSize];
        for (size_t j = 0; j < this->boardSize; j++)
        {
            std::cin >> tmpBoard[i][j];
        }
    }

    this->initialState = new GameNode(this->boardSize, tmpBoard);
}

void Solver::setGoalState() {
    int **goalBoard = new int*[this->boardSize];
    for (size_t i = 0; i < this->boardSize; i++)
    {
        goalBoard[i] = new int[this->boardSize];
        for (size_t j = 0; j < this->boardSize; j++)
        {
            goalBoard[i][j] = i * this->boardSize + j + 1;
        }
    }
    goalBoard[this->boardSize - 1][this->boardSize - 1] = 0;
    this->goal = new GameNode(this->boardSize, goalBoard);

    std::cout << std::endl;
    std::cout << this->boardSize << std::endl;
    this->goal->printState();
    std::cout << std::endl;
    std::cout << "manhattan: " << this->initialState->manhattan() << std::endl;
    // 5 + 4 + 4 + 4 + 3 + 3 + 3 + 2 + 3 + 1 + 3 + 2 + 3 + 4 + 2 + 4
}

void Solver::solve()
{
    if (this->initialState->isSolvable()) {
        std::cout << "not solvable" << std::endl;
    }
    int threshhold = initialState->manhattan();
    this->path.clear();
    for (;;) {
    // for(int i = 0; i < 1; i++) {
        std::vector<GameNode *> visited;
        int newThreshhold = search(initialState, 0, threshhold, visited);
        if (newThreshhold == -1) {
            return;
        }
        std::cout << "new threshhold: " << newThreshhold << std::endl; 
        threshhold = newThreshhold;   
    }
}

int Solver::search(GameNode *node, int g, int threshhold, std::vector<GameNode *> &visited) {
    visited.push_back(node);
    int f = g + node->manhattan();
    
    // std::cout << "depth: " << g << std::endl;
    // std::cout << "threshhold: " << threshhold << " vs f " << f << std::endl;
    
    if (*node == *(this->goal)) {
        std::cout << "found" << std::endl;
        return -1;
    }
    if (f > threshhold) {
        return f;
    }

    int min = std::numeric_limits<int>::max();
    std::vector<Direction> directions({Left, Right, Up, Down});
    for (auto direction : directions)
    {
        GameNode * child = node->getNeighbour(direction);
        if (child != nullptr && !this->isVisited(child, visited)) {
            // child->printState();
          
            int childF = this->search(child, g + 1, threshhold, visited);
            if (childF == -1) {
                getDirection(direction);
                return -1;
            }
            if (childF < min) {
                min = childF;
            }
        }
    }
    return min;
}

bool Solver::isVisited(GameNode *node, std::vector<GameNode *> &visited) {
    for (auto v : visited) {
        if (*node == *v) {
            return true;
        }
    }
    return false;
}

/*
8
-1
1 2 3
4 5 6
0 7 8

1 2 3 
4 5 6 
7 0 8

8
-1
8 0 5
1 6 7
3 2 4

15
-1
15 12 0 5
14 13 9 3
7 11 2 4
1 8 10 6

15
-1
1 2 3 4
5 6 0 8
9 10 7 11
13 14 15 12
*/