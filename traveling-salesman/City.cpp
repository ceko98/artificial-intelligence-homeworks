#include <algorithm>

struct City
{
    int x;
    int y;
    City(int x, int y) : x(x), y(y) {}

    int distance(City other) {
        int xDis = std::abs(x - other.x);
        int yDis = std::abs(y - other.y);
        return xDis * xDis + yDis * yDis;
    }
};