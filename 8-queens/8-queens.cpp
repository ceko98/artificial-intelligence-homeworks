#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include "board.cpp"

class Solver
{
private:
    Board b;
public:
    Solver(/* args */);
    ~Solver();
};

Solver::Solver(/* args */) : b(4)
{

}

Solver::~Solver()
{
}


int main ()
{
    Board s(4);
    s.solve();
    return 0;
}