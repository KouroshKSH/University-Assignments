#include <iostream>
#include <string>
#include <iomanip>
#include "Board.h"

using namespace std;

Board::Board(int number_of_slots)
{ // parametric constructor
    first = new Node("None", nullptr); // define the first node as None
    Node* current = first;
    for (int i = 1; i < number_of_slots; i++)
    { // initialize all nodes as owned by no one
        Node* temp  = new Node("None", nullptr);
        current->next = temp;
        current = temp;
    }
    current->next = first; // make the board circular
}

Board::~Board()
{ // destructor
    // first 2 conditions for handling edge-cases
    if (first == nullptr)
    { return; }
    if (first->next == first)
    {
        delete first;
        first = nullptr;
        return;
    }
    Node* current = first->next; // start from the 2nd node
    while (current != first)
    { // iterate until all slots are deleted
        Node* next = current->next;
        delete current;
        current = next; 
    }
    delete first;
    first = nullptr;
}

void Board::display()
{ // display the content of the board via 3 rows
    Node* temp = first;
    string second_row = "^", third_row = "|";
    while(temp->next != first)
    {
        string slotOwner = temp->owner;
        cout << slotOwner;
        if(slotOwner != "None")
        { cout << " "; } // either "Ali" or "Liz"
        cout << "->";
        second_row += "      "; // 6 spaces
        third_row += "----<-";
        temp = temp->next;
    }
    cout << temp->owner << "\n" << second_row << "  |\n" << third_row << "--v\n";
}

string Board::who_owns(Node* node) const
{return node->owner; } // returns the name of the owner of that node

Node* Board::get_first_node()
{ return first; } // a getter function for returning the first node