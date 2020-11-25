#include <algorithm>
#include <cmath>

struct City
{
    int x;
    int y;
    City(int x, int y) : x(x), y(y) {}

    float distance(City other) {
        int xDis = std::abs(x - other.x);
        int yDis = std::abs(y - other.y);
        return sqrt(xDis * xDis + yDis * yDis);
    }
};