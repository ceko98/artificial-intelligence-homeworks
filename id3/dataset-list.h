#include <map>
#include <set>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using std::cin;
using std::cout;
using std::map;
using std::set;
using std::string;
using std::vector;

#ifndef __DATASET_LIST__
#define __DATASET_LIST__

void printVector(vector<string> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << ",";
    }
    cout << "\n";
}

struct Instance
{
    string className;
    vector<string> attributes;
};

struct Dataset
{
    const int NUMBER_OF_ATTRIBUTES = 4;
    vector<Instance> dataset;
    map<int, set<string>> attributesIndexToValues;
    Dataset(const char *fileName);
};

Dataset::Dataset(const char *fileName)
{
    std::ifstream file(fileName);
    char buff[1024];
    while (!file.eof())
    {
        Instance instance;
        file.getline(buff, 1024, ',');
        instance.className = buff;
        for (int i = 0; i < NUMBER_OF_ATTRIBUTES - 1; i++)
        {
            file.getline(buff, 64, ',');
            instance.attributes.push_back(buff);
            if (buff[0])
            {
                attributesIndexToValues[i].insert(buff);
            }
        }
        file.getline(buff, 64);
        instance.attributes.push_back(buff);
        if (buff[0])
        {
            attributesIndexToValues[NUMBER_OF_ATTRIBUTES - 1].insert(buff);
        }
        dataset.push_back(instance);
        // printVector(instance.attributes);
    }
    dataset.pop_back();

    for (auto i : attributesIndexToValues)
    {
        cout << i.first << " ";
        for (auto a : i.second)
        {
            cout << a << ",";
        }
        cout << "\n";
    }
}

#endif
