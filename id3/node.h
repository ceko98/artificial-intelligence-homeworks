#ifndef __NODE__
#define __NODE__

#include <vector>
#include <map>
#include <string>

using namespace std;

struct Node
{
    map<string, Node *> subtrees;
    int attributeIndex;
    string resultClass;

    Node();
    ~Node();
};

Node::Node() {}

Node::~Node()
{
    for (auto i : subtrees)
    {
        delete i.second;
    }
}

#endif