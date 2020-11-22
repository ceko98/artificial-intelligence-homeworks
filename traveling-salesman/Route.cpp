#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "City.cpp"

struct Route
{
    std::vector<int> route;
    float length;
    Route(int n, std::vector<City>& cities)
    {
        std::mt19937 g(std::random_device{}());
        for (size_t i = 0; i < n; i++)
        {
            route.push_back(i);
        }
        std::shuffle(route.begin(), route.end(), g);
        setLength(cities);
    }

    Route(std::vector<int>& route, std::vector<City>& cities) : route(route)
    {
        setLength(cities);
    }

    void setLength(std::vector<City>& cities)
    {
        float distance = 0;
        for (int i = 0; i < route.size() - 1; i++)
        {
            distance += cities[route[i]].distance(cities[route[i + 1]]);
        }
        length = distance;
    }

    void printRoute()
    {
        std::cout << length << "   ";
        for (int g : route)
        {
            std::cout << g << " ";
        }
        std::cout << std::endl;
    }
};