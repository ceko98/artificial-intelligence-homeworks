#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>

class Board
{
private:
    int size;
    int *queens;
    int *row;
    int *diag1;
    int *diag2;

    int *tmp;
    int tmpIndex;

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
};

Board::Board(int size) : size(size)
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
    printBoard();
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
    std::cout << minConflict << std::endl;
    return getRamdomFromTmp();
}

int Board::conflictsFor(int column, int row)
{
    return this->row[row] + this->diag1[column - row + this->size - 1] + this->diag2[column + row];
}

int Board::getRamdomFromTmp()
{
    return tmp[rand() % tmpIndex];
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
    std::cout << "max: " << maxConflict << std::endl;
    // if (tmpIndex <= 1) {
    //     return
    // }
    return getRamdomFromTmp();
}

int Board::getMinQueenConflicts(int column)
{
    int minConflicts = this->size;
    this->tmpIndex = 0;

    for (int i = 0; i < this->size; i++)
    {
        int conflicts = this->conflictsFor(column, i) - 3;
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
    std::cout << "min: " << minConflicts << std::endl;
    for (int i = 0; i < tmpIndex; i++)
    {
        std::cout << tmp[i] << " ";
    }
    std::cout << std::endl;
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
    int index = 0;
    while (index++ <= 1 * this->size)
    {
        int column = getMaxConflictQueen();
        int oldRow = queens[column];
        int newRow = getMinQueenConflicts(column);
        std::cout << "col: " << column;
        std::cout << " newRow: " << newRow;
        std::cout << " oldRow: " << oldRow << std::endl;
        moveQueen(column, newRow, oldRow);
        printBoard();
    }
    std::cout << std::endl;

    if (hasConflicts())
    {
        std::cout << "sad" << std::endl;
        return;
    }
    std::cout << "heppy" << std::endl;
}

Board::~Board()
{
    delete[] queens;
    delete[] row;
    delete[] diag1;
    delete[] diag2;
    delete[] tmp;
}
