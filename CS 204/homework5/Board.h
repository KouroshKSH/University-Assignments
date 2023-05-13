#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
using namespace std;

struct Node
{
    // [ ] When initializing, use the string "None" as the owner of a slot.
    string owner;
    Node* next;
    Node (); // [ ] write the default constructor if needed
    Node (string o, Node * p); // [ ] write the constructor
};

class Board
{
    private:
        Node * first;

    public:
    // [ ] fix the arguments for each function
        void display();
        string who_owns();
        // [ ] write extra functions if needed
};

#endif