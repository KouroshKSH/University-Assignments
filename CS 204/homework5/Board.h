#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
using namespace std;

struct Node
{
    string owner;
    Node* next;
    Node () :
        owner("None"), next(nullptr) {}
    Node (string o, Node * p) :
        owner(o), next(p) {}
};

class Board
{
private:
    Node * first;

public:
// [ ] fix the arguments for each function
    Board(const number_of_slots); // parametric constructor
    Board(const Board & other); // deep copy constructor
    ~Board(); // destructor
    void display();
    string who_owns(Node * node) const; // returns the name of the owner of that node
    // [ ] write extra functions if needed
};

#endif