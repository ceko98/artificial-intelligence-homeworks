#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>

class Board
{
private:
    int size;
    int * queens = nullptr;
    int * row = nullptr;
    int * diag1 = nullptr;
    int * diag2 = nullptr;

    int * tmp = nullptr;
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
    void init();
    void clear();
};

Board::Board(int size) : size(size)
{
    this->queens = new int[this->size]();
    this->row = new int[this->size]();
    this->diag1 = new int[this->size * 2 - 1]();
    this->diag2 = new int[this->size * 2 - 1]();

    this->tmp = new int[this->size];
    this->tmpIndex = 0;
}

void Board::init()
{
    srand(time(NULL));
    for (int i = 0; i < this->size; i++)
    {
        this->queens[i] = findMinConflict(i);
        this->row[queens[i]]++;
        this->diag1[i - queens[i] + this->size - 1]++;
        this->diag2[i + queens[i]]++;
    }
}

void Board::clear()
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
    while (index++ <= 2 * this->size)
    {
        int column = getMaxConflictQueen();
        if (conflictsFor(column, queens[column]) - 3)
        {
            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
            return;
        }
        int oldRow = queens[column];
        int newRow = getMinQueenConflicts(column);
        moveQueen(column, newRow, oldRow);
    }

    if (hasConflicts())
    {
        clear();
        solve();
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}

Board::~Board()
{
    clear();
}

int main()
{
    int n;
    std::cin >> n;

    Board s(n);
    s.solve();

    return 0;
}