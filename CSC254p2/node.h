#include <iostream>
#include <cstdlib>
#include "scan.h"

using namespace std;

class Node{
public:
    string image;
    Node *parent;
    Node *child[4];
    token type;
    Node(string i, Node *p, token t);
    void setChildren(Node*, Node*, Node*, Node*);
};

Node::Node(string i, Node* p, token t)
{
    image = i;
    parent = p;
    type = t;
    child[0] = 0; //initializa all the children to be 0
    child[1] = 0;
    child[2] = 0;
    child[3] = 0;
}

void Node::setChildren(Node* c1, Node* c2, Node* c3, Node* c4)
{
    if(c1 != 0)
        child[0] = c1;
    if(c2 != 0)
        child[1] = c2;
    if(c3 != 0)
        child[2] = c3;
    if(c4 != 0)
        child[3] = c4;
}
