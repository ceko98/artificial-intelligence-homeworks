#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include "utils.cpp"

enum Party
{
    Republican = 1,
    Democrat = 2
};

struct Instance
{
    std::string instanceClass;
    std::vector<char> attributes;
};

struct Model
{
    int republicansProbe;
    int democratsProbe;
    std::vector<double> attributeProbYes;
    std::vector<double> attributeProbNo;
    std::vector<double> attributeProbPass;
};

class Classifier
{
private:
    const int NUMBER_OF_ATTRIBUTES = 16;
    std::vector<Instance> dataset;
    std::vector<int> setIndexes;
    Model *model;

public:
    Classifier(const char *fileName);
    void createModel(int testIndex);
    void createModelClassProb(int start, int end);
    void createModelAttributeProb(int start, int end, int republicans, int democrats);
    double testModel(int testIndex);
    double testInstance(Instance instance, Party clas);
};

Classifier::Classifier(const char *fileName)
{
    std::ifstream file(fileName);
    char buff[1024];
    while (!file.eof())
    {
        Instance instance;
        file.getline(buff, 1024, ',');
        instance.instanceClass = buff;
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
    setIndexes.push_back(dataset.size());
}

void Classifier::createModelClassProb(int start, int end)
{
    int democrats = 1, republicans = 1, total = 2;
    for (int i = 0; i < dataset.size(); i++)
    {
        if (i < start || i >= end)
        {
            if (dataset[i].instanceClass.compare("republican") == 0)
            {
                republicans++;
            }
            else
            {
                democrats++;
            }
            total++;
        }
    }
    model->republicansProbe = (double)republicans / (double)total;
    model->democratsProbe = (double)democrats / (double)total;
    createModelAttributeProb(start, end, republicans - 1, democrats - 1);
}

void Classifier::createModelAttributeProb(int start, int end, int republicans, int democrats)
{
    republicans += 3;
    democrats += 3;
    for (int i = 0; i < 16; i++)
    {
        int yesRep = 1, noRep = 1, passRep = 1;
        int yesDem = 1, noDem = 1, passDem = 1;
        for (int i = 0; i < dataset.size(); i++)
        {
            if (i < start && i >= end)
            {
                if (dataset[i].attributes[i] == 'y')
                {
                    dataset[i].instanceClass.compare("republican") == 0 ? yesRep++ : yesDem++;
                }
                else if (dataset[i].attributes[i] == 'n')
                {
                    dataset[i].instanceClass.compare("republican") == 0 ? noRep++ : noDem++;
                }
                else
                {
                    dataset[i].instanceClass.compare("republican") == 0 ? passRep++ : passDem++;
                }
            }
        }
        std::vector<double> probsYes;
        std::vector<double> probsNo;
        std::vector<double> probsPass;
        probsYes.push_back((double)yesRep / (double)republicans);
        probsYes.push_back((double)yesDem / (double)democrats);
        probsNo.push_back((double)noRep / (double)republicans);
        probsNo.push_back((double)noDem / (double)democrats);
        probsPass.push_back((double)passRep / (double)republicans);
        probsPass.push_back((double)passDem / (double)democrats);
        model->attributeProbYes = probsYes;
        model->attributeProbNo = probsNo;
        model->attributeProbPass = probsPass;
    }
}

void Classifier::createModel(int testIndex)
{
    model = new Model();
    int start = this->setIndexes[testIndex];
    int end = this->setIndexes[testIndex + 1];
    createModelClassProb(start, end);
}

double Classifier::testInstance(Instance instance, Party party)
{
    double prob = party == Republican ? model->republicansProbe : model->democratsProbe;
    for (int i = 0; i < 16; i++)
    {
        if (instance.attributes[i] == 'y')
        {
            prob *= party == Republican ? model->attributeProbYes[0] : model->attributeProbYes[1];
        }
        else if (instance.attributes[i] == 'n')
        {
            prob *= party == Republican ? model->attributeProbNo[0] : model->attributeProbNo[1];
        }
        else
        {
            prob *= party == Republican ? model->attributeProbPass[0] : model->attributeProbPass[1];
        }
    }
    return prob;
}

double Classifier::testModel(int testIndex)
{
    int start = this->setIndexes[testIndex];
    int end = this->setIndexes[testIndex + 1];
    int total = 0, accurate = 0;
    for (int i = start; i < end; i++)
    {
        total++;
        double probRep = testInstance(dataset[i], Republican);
        double probDem = testInstance(dataset[i], Democrat);
        if (probRep > probDem)
        {
            if (dataset[i].instanceClass.compare("republican") == 0)
            {
                accurate++;
            }
        }
        else
        {
            if (dataset[i].instanceClass.compare("republican") != 0)
            {
                accurate++;
            }
        }
    }
    return (double)accurate / (double)total;
}

int main()
{
    Classifier c("house-votes-84.data");
    double sum = 0;
    for (int i = 0; i < 10; i++)
    {
        c.createModel(i);
        double accuracy = c.testModel(i);
        sum += accuracy;
        std::cout << "Model " << i << ": " << accuracy << std::endl;
    }
    std::cout << "Total " << sum / 10.0 << std::endl;

    return 0;
}
