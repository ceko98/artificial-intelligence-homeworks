#include "node.h"
#include "dataset-list.h"
#include <cmath>
#include <string>

using namespace std;

float entropy(vector<Instance> &instances)
{
    int possitive = 0;
    for (auto i : instances)
    {
        if (i.className.compare("yes") == 0)
        {
            possitive++;
        };
    }
    float p = (float)possitive / (float)instances.size();
    float q = (float)(instances.size() - possitive) / (float)instances.size();
    if (p == 0) {
        return - q * log2(q);
    }
    if (q == 0)
    {
        return - p * log2(p);
    }
    return ((-p * log2(p)) - q * log2(q));
}

vector<Instance> filterIntances(vector<Instance> &instances, int index, string attribute)
{
    vector<Instance> result;
    for (int i = 0; i < instances.size(); i++)
    {
        if (instances[i].attributes[index] == attribute)
        {
            result.push_back(instances[i]);
        }
    }
    return result;
}

string classMajority(vector<Instance> &instances)
{
    int possitive = 0;
    for (auto i : instances)
    {
        if (i.className == "yes")
        {
            possitive++;
        };
    }
    return possitive > instances.size() ? "yes" : "no";
}

class Classifier
{
private:
    const int NUMBER_OF_ATTRIBUTES = 4;

    Dataset dataset;
    Node *root;

public:
    Classifier(const char *fileName);
    void buildTree(Node *root, vector<Instance> &instances, set<int> used);
    int getBestAttributeIndex(vector<Instance> &instances, set<int> used);
    ~Classifier();
};

Classifier::Classifier(const char *fileName) : dataset(fileName)
{
    this->root = new Node();
    set<int> used;
    buildTree(this->root, dataset.dataset, used);
};

void Classifier::buildTree(Node *root, vector<Instance> &instances, set<int> used)
{
    float setEntropy = entropy(instances);
    if (setEntropy == 0)
    {
        root->attributeIndex = -1;
        root->resultClass = classMajority(instances);
    }
    if (instances.size() <= 5)
    {
        root->attributeIndex = -1;
        root->resultClass = classMajority(instances);
    }
    int bestIndex = getBestAttributeIndex(instances, used);
    root->attributeIndex = bestIndex;
    used.insert(bestIndex);

    for (auto i : used)
    {
        cout << i << ",";
    }
        cout << "\n";
    

    // for (auto value : dataset.attributesIndexToValues[bestIndex])
    // {
    //     vector<Instance> filtered = filterIntances(instances, bestIndex, value);
    //     Node *subtree = new Node();
    //     buildTree(subtree, filtered, used);
    //     root->subtrees[value] = subtree;
    // }
};

int Classifier::getBestAttributeIndex(vector<Instance> &instances, set<int> used)
{
    float setEntropy = entropy(instances);
    float bestInfoGain = 0;
    int maxAttributeIndex = -1;
    for (int i = 0; i < NUMBER_OF_ATTRIBUTES; i++)
    {
        if (used.find(i) == used.end())
        {
            float currentGain = setEntropy;
            for (auto value : dataset.attributesIndexToValues[i])
            {
                vector<Instance> filtered = filterIntances(instances, i, value);
                // printVector(filtered);
                float attributeEntropy = entropy(instances);
                cout << attributeEntropy << " ";
                currentGain -= filtered.size() / instances.size() * attributeEntropy;
            }
            cout << "gain for " << i << ": " << currentGain << "\n";
            if (currentGain > bestInfoGain)
            {
                bestInfoGain = currentGain;
                maxAttributeIndex = i;
            }
        }
    }
    return maxAttributeIndex;
}

Classifier::~Classifier()
{
    delete root;
}

int main()
{
    // Classifier c("test_data.txt");
    Classifier c("test_data.txt");
    return 0;
}