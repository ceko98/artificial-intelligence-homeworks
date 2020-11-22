#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "Route.cpp"
#include "City.cpp"

class Solver
{
private:
    const int POPULATION_SIZE = 4;
    int size;
    std::vector<City> cities;
    std::vector<Route> population;

    std::mt19937 rng;
public:
    Solver();
    ~Solver();
    void initCityies();
    void generateCity();
    void initialPopulation();
    int routeFitness(Route r);
};

Solver::Solver() : rng(std::random_device{}())
{
    std::cin >> size;
    for (size_t i = 0; i < size; i++)
    {
        generateCity();
    }
    for (size_t i = 0; i < POPULATION_SIZE; i++)
    {
        population.push_back({size});
        std::cout << routeFitness(population[i]) << std::endl;
    }

    std::sort(population.begin(), population.end(), [this](Route a, Route b){ return routeFitness(a) < routeFitness(b); } );

    for (size_t i = 0; i < cities.size(); i++)
    {
        std::cout << cities[i].x << " " << cities[i].y << std::endl;
    }
    for (Route g : population)
    {
        g.printRoute();
    }
}

void Solver::generateCity()
{
    std::uniform_int_distribution<int> dist(0, size - 1);
    cities.push_back({dist(rng), dist(rng)});
}

int Solver::routeFitness(Route r) {
    int distance = 0;
    for (int i = 0; i < r.route.size() - 1; i++)
    {
        distance += cities[r.route[i]].distance(cities[r.route[i + 1]]);
    }
    return distance;
}

Solver::~Solver()
{
}

int main()
{
    Solver s;
    return 0;
}