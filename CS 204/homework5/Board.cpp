#include <iostream>
#include <string>
#include <iomanip>
#include "Board.h"

using namespace std;

Board::Board(int number_of_slots)
{ // parametric constructor
    //cout << "\n\tcalled Board constructor\n";
    if (number_of_slots <= 0)
    {
        first = nullptr;
        return;
    }
    int n = number_of_slots;
    first = new Node("None", nullptr); // define the first node as None
    Node* current = first;
    for (int i = 1; i < n; i++)
    { // initialize all nodes as owned by no one
        Node* temp  = new Node("None", nullptr);
        current->next = temp;
        current = temp;
    }
    current->next = first; // make the board circular
}

// Board::Board(const Board& other)
// { // deep copy constructor
//     //cout << "\n\tcalled deep copy for Board\n";
//     // the first slot is owned by no one
//     first = new Node("None", nullptr);
//     Node* current = first; // assign the first node
//     Node* other_current = other.first;
//     // copy the owners from the other board to assign to the the current board
//     while (other_current->next != other.first)
//     {
//         current->next = new Node(other_current->owner, nullptr);
//         current = current->next;
//         other_current = other_current->next;
//     }
//     // for the last node
//     current->next = new Node(other_current->owner, nullptr);
//     current->next->next = first; // make the board circular
// }

Board::~Board()
{ // destructor
    //cout << "\n\tcalled destructor\n";
    Node* current = first;
    while (current->next != NULL)
    { // iterate until all slots are deleted
        Node* next = current->next;
        current = next; 
        delete next;
    }
    delete current;
    current = nullptr;
}

void Board::display()
{ // display the content of the board
    //cout << "\n\tcalled display for Board\n";
    Node* temp = first;
    string midRow = "^";
    string lastRow = "|";
    while(temp->next != first)
    {
        string slotOwner = temp->owner;
        cout << slotOwner;
        if(slotOwner != "None")
        { cout << " "; }
        cout << "->";

        midRow += "      ";
        lastRow += "----<-";
        temp = temp->next;
    }
    cout << temp->owner << "\n" << midRow << "  |\n" << lastRow << "--v\n";
}

string Board::who_owns(Node* node) const
{ /*cout << "\n\tcalled who owns\n";*/ return node->owner; } // returns the name of the owner of that node

Node* Board::get_first_node()
{  // a getter function for returning the first node
    //cout << "\n\tcalled getter Board\n";
    return first;
}