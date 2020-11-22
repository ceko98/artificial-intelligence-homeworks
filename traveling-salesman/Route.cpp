#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>

struct Route
{
    std::vector<int> route;

    Route(int n)
    {
        std::mt19937 g(std::random_device{}());
        for (size_t i = 0; i < n; i++)
        {
            route.push_back(i);
        }
        std::shuffle(route.begin(), route.end(), g);
    }

    void printRoute()
    {
        for (int g : route)
        {
            std::cout << g << " ";
        }
        std::cout << std::endl;
    }
};