#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include "utils.cpp"

template<class T>
void printVector(std::vector<T> &v) {
    std::cout << v.size() << " ";
    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

struct Instance
{
    std::string instanceClass;
    std::vector<char> attributes;
};

struct Model
{
    std::vector<std::vector<float>> attributeProb;
    std::vector<float> classProb;
};

class Classifier
{
private:
    const int NUMBER_OF_ATTRIBUTES = 16;
    std::vector<Instance> dataset;
    std::vector<int> setIndexes;

public:
    Classifier(const char *fileName);
    void createModel(int testIndex);
};

Classifier::Classifier(const char *fileName)
{
    std::ifstream file(fileName);
    char buff[1024];
    while (!file.eof())
    {
        Instance instance;
        file.getline(buff, 1024, ',');

        for (int i = 0; i < NUMBER_OF_ATTRIBUTES - 1; i++)
        {
            file.getline(buff, 64, ',');
            instance.attributes.push_back(buff[0]);
        }
        file.getline(buff, 64);
        instance.attributes.push_back(buff[0]);

        dataset.push_back(instance);
    }
    dataset.pop_back();

    int setSize = this->dataset.size() / 10;
    int reminder = this->dataset.size() % 10;
    int increment = 0;
    for (int i = 0; i < 10; i++)
    {
        int val = i * setSize + increment;
        if (i < reminder)
        {
            increment++;
        }
        setIndexes.push_back(val);
    }

    printVector(setIndexes);
}

void Classifier::createModel(int testIndex)
{
    
}

int main()
{
    Classifier c("house-votes-84.data");
    return 0;
}
