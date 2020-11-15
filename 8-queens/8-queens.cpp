#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <random>

class Board
{
private:
    int size;
    int *queens = nullptr;
    int *row = nullptr;
    int *diag1 = nullptr;
    int *diag2 = nullptr;

    int *tmp = nullptr;
    int tmpIndex;

    std::mt19937 rng;

public:
    Board(int size);
    ~Board();

    int findMinConflict(int column);
    int conflictsFor(int column, int row);
    int getRamdomFromTmp();
    void printBoard();
    bool hasConflicts();
    int getMaxConflictQueen();
    int getMinQueenConflicts(int column);
    void moveQueen(int column, int newRow, int oldRow);
    void solve();
    void init();
    void clean();
};

Board::Board(int size) : size(size), rng(std::random_device{}())
{
}

void Board::init()
{
    this->queens = new int[this->size]();
    this->row = new int[this->size]();
    this->diag1 = new int[this->size * 2 - 1]();
    this->diag2 = new int[this->size * 2 - 1]();

    this->tmp = new int[this->size];
    this->tmpIndex = 0;

    srand(time(NULL));
    for (int i = 0; i < this->size; i++)
    {
        this->queens[i] = findMinConflict(i);
        this->row[queens[i]]++;
        this->diag1[i - queens[i] + this->size - 1]++;
        this->diag2[i + queens[i]]++;
    }
}

void Board::clean()
{
    delete[] queens;
    delete[] row;
    delete[] diag1;
    delete[] diag2;
    delete[] tmp;
}

int Board::findMinConflict(int column)
{
    int minConflict = this->size;
    this->tmpIndex = 0;

    for (int i = 0; i < this->size; i++)
    {
        int conflicts = this->conflictsFor(column, i);
        if (conflicts < minConflict)
        {
            tmpIndex = 0;
            minConflict = conflicts;
        }
        if (conflicts == minConflict)
        {
            tmp[tmpIndex++] = i;
        }
    }
    return getRamdomFromTmp();
}

int Board::conflictsFor(int column, int row)
{
    return this->row[row] + this->diag1[column - row + this->size - 1] + this->diag2[column + row];
}

int Board::getRamdomFromTmp()
{
    std::uniform_int_distribution<int> dist(0, tmpIndex - 1);
    return tmp[dist(rng)];
}

void Board::printBoard()
{
    for (int i = 0; i < this->size; i++)
    {
        for (int j = 0; j < this->size; j++)
        {
            std::cout << (queens[j] == i ? "x " : "_ ");
        }
        std::cout << std::endl;
    }
}

bool Board::hasConflicts()
{
    for (int i = 0; i < this->size; i++)
    {
        if (this->conflictsFor(i, queens[i]) - 3)
        {
            return true;
        };
    }
    return false;
}

int Board::getMaxConflictQueen()
{
    int maxConflict = -1;
    this->tmpIndex = 0;

    for (int i = 0; i < this->size; i++)
    {
        int conflicts = this->conflictsFor(i, queens[i]) - 3;
        if (conflicts > maxConflict)
        {
            tmpIndex = 0;
            maxConflict = conflicts;
        }
        if (conflicts == maxConflict)
        {
            tmp[tmpIndex++] = i;
        }
    }
    return getRamdomFromTmp();
}

int Board::getMinQueenConflicts(int column)
{
    int minConflicts = this->size;
    this->tmpIndex = 0;

    for (int i = 0; i < this->size; i++)
    {
        int conflicts = this->conflictsFor(column, i);
        if (conflicts < minConflicts)
        {
            tmpIndex = 0;
            minConflicts = conflicts;
        }
        if (conflicts == minConflicts)
        {
            tmp[tmpIndex++] = i;
        }
    }
    return getRamdomFromTmp();
}

void Board::moveQueen(int column, int newRow, int oldRow)
{
    this->row[newRow]++;
    this->diag1[column - newRow + this->size - 1]++;
    this->diag2[column + newRow]++;

    this->row[oldRow]--;
    this->diag1[column - oldRow + this->size - 1]--;
    this->diag2[column + oldRow]--;
    queens[column] = newRow;
}

void Board::solve()
{
    clock_t tStart = clock();
    init();

    int index = 0;
    while (index++ <= 3 * this->size)
    {
        int column = getMaxConflictQueen();
        if (conflictsFor(column, queens[column]) - 3 == 0)
        {
            if (size < 10)
            {
                printBoard();
            }
            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
            return;
        }
        int oldRow = queens[column];
        int newRow = getMinQueenConflicts(column);
        moveQueen(column, newRow, oldRow);
    }

    if (hasConflicts())
    {
        clean();
        solve();
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

Board::~Board()
{
    clean();
}

int main()
{
    int n;
    std::cin >> n;

    Board s(n);
    s.solve();

    return 0;
}