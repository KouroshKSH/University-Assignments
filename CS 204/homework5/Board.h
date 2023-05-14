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
    Board(const number_of_slots);
    Board(const Board & other);
    ~Board();
    /***/
        void display(); // [ ] write this
    /***/
    string who_owns(Node * node) const;
};

#endif