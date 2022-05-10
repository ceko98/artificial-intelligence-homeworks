#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include "Route.cpp"

// for testing only
void printv(std::vector<int> &v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

class Solver
{
private:
    const int POPULATION_SIZE = 20;
    const int MATING_POOL_SIZE = 20;
    int size;
    std::vector<City> cities;
    std::vector<Route> population;
    std::vector<Route> matingPool;
    std::vector<Route> nextGeneration;

    std::mt19937 rng;

public:
    Solver();
    void initCityies();
    void generateCity();
    void initialPopulation();
    int routeFitness(Route r);
    void selection();
    void breed(Route parent1, Route parent2);
    void prepareNextGeneration();
    void mutate();
    void start();
};

Solver::Solver() : rng(std::random_device{}())
{
    start();
    std::cout << std::endl;
}

void Solver::initCityies()
{
    std::cin >> size;
    for (size_t i = 0; i < size; i++)
    {
        generateCity();
    }
}

void Solver::initialPopulation()
{
    for (size_t i = 0; i < POPULATION_SIZE; i++)
    {
        population.push_back({size, cities});
    }
    std::sort(population.begin(), population.end(), [](Route a, Route b) { return a.length < b.length; });
}

void Solver::generateCity()
{
    // not size
    std::uniform_int_distribution<int> dist(0, 1000);
    cities.push_back({dist(rng), dist(rng)});
}

void Solver::breed(Route parent1, Route parent2)
{
    std::vector<int> child1(size, -1), child2(size, -1);
    std::uniform_int_distribution<int> dist(0, size - 1);
    int start = dist(rng), end = dist(rng);
    if (start > end)
    {
        std::swap(start, end);
    }
    for (int i = start; i <= end; i++)
    {
        child1[i] = parent1.route[i];
        child2[i] = parent2.route[i];
    }

    std::vector<int> tmp1, tmp2;
    for (int i = end + 1; i < size; i++)
    {
        tmp1.push_back(parent1.route[i]);
        tmp2.push_back(parent2.route[i]);
    }

    for (int i = 0; i <= end; i++)
    {
        tmp1.push_back(parent1.route[i]);
        tmp2.push_back(parent2.route[i]);
    }

    int childIndex = (end + 1) % size;
    for (int elem : tmp2)
    {
        if (std::find(child1.begin(), child1.end(), elem) == child1.end())
        {
            child1[childIndex] = elem;
            childIndex = (childIndex + 1) % size;
        }
    }

    childIndex = (end + 1) % size;
    for (int elem : tmp1)
    {
        if (std::find(child2.begin(), child2.end(), elem) == child2.end())
        {
            child2[childIndex] = elem;
            childIndex = (childIndex + 1) % size;
        }
    }
    nextGeneration.push_back(Route(child1, cities));
    nextGeneration.push_back(Route(child2, cities));
}

void Solver::prepareNextGeneration()
{
    nextGeneration.clear();
    for (size_t i = 0; i < population.size(); i += 2)
    {
        breed(population[i], population[i + 1]);
    }
    std::sort(nextGeneration.begin(), nextGeneration.end(), [](Route a, Route b) { return a.length < b.length; });
}

void Solver::mutate()
{
    for (Route &r : nextGeneration)
    {
        std::uniform_int_distribution<int> dist(0, size - 1);
        int i = dist(rng), j = dist(rng);
        std::swap(r.route[i], r.route[j]);
        r.setLength(cities);
    }
    nextGeneration.pop_back();
    nextGeneration.push_back(population[0]);
    std::sort(nextGeneration.begin(), nextGeneration.end(), [](Route a, Route b) { return a.length < b.length; });

    population = nextGeneration;
}

void Solver::start()
{
    initCityies();
    initialPopulation();
    int iterationsWithoutImprovement = 0;
    int iterations = 0;
    Route lastBest = population[0];
    for (size_t i = 0; true; i++)
    {
        // if (i % 100 == 0)
        // {
        //     population[0].printRoute();
        //     std::cout << iterationsWithoutImprovement << std::endl;
        //     std::cout << std::endl;
        // }

        if (iterations == 10)
        {
            population[0].printRoute();
            std::cout << std::endl;
        }

        if (iterations == 100 || iterations == 200 || iterations == 300)
        {
            population[0].printRoute();
            std::cout << std::endl;
        }

        selection();
        prepareNextGeneration();
        mutate();
        iterationsWithoutImprovement++;
        if (population[0].length < lastBest.length)
        {
            lastBest = population[0];
            iterationsWithoutImprovement = 0;
        }
        if (iterationsWithoutImprovement >= size * 50)
        {
            break;
        }
        iterations++;
    }

    population[0].printRoute();
}

void Solver::selection()
{
    matingPool.clear();
    std::vector<float> prob;
    float lenghtSum = 0;
    for (Route r : population)
    {
        lenghtSum += r.length;
        prob.push_back(1.0 / lenghtSum);
    }
    for (size_t i = 0; i < MATING_POOL_SIZE; i++)
    {
        std::uniform_int_distribution<int> dist(0, lenghtSum);
        float number = 1.0 / dist(rng);
        for (int j = 0; j < prob.size(); j++)
        {
            if (number > prob[j])
            {
                matingPool.push_back(population[j]);
                break;
            }
        }
    }
}

int main()
{
    Solver s;
    return 0;
}