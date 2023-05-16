#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
using namespace std;

struct Node
{
    string owner;
    Node* next;
    Node(); // no need for default constructor
    Node (string o, Node* p) :
        owner(o), next(p) {}
};

class Board
{
private:
    Node* first;

public:
    Board(int number_of_slots);
    ~Board();
    void display(); 
    string who_owns(Node* node) const;
    Node* get_first_node();
};

#endif