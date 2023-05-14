#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "Board.h"

using namespace std;

Board::Board(const int number_of_slots)
{ // parametric constructor
    int n = number_of_slots;
    first = new Node("None", nullptr); // define the first node as None
    Node* current = first;
    for (int i = 1; i < n; i++)
    { // initialize all nodes as owned by no one
        current->next = new Node("None", nullptr);
        current = current->next;
    }
    current->next = first; // make the board circular
}

Board::Board(const Board & other)
{ // deep copy constructor
    // the first slot is owned by no one
    first = new Node("None", nullptr);
    Node* current = first; // assign the first node
    Node* other_current = other.first;
    // copy the owners from the other board to assign to the the current board
    while (other_current->next != other.first)
    {
        current->next = new Node(other_current->owner, nullptr);
        current = current->next;
        other_current = other_current->next;
    }
    // for the last node
    current->next = new Node(other_current->owner, nullptr);
    current->next->next = first; // make the board circular
}

Board::~Board()
{ // destructor
    Node* current = first;
    while (current != nullptr)
    { // iterate until all slots are deleted
        Node* next = current->next;
        delete current;
        current = next; 
    }
}

string Board::who_owns(Node* node) const
{ return node->owner; } // returns the name of the owner of that node

