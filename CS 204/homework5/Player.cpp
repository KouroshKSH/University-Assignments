#include <iostream>
#include <string>
#include <iomanip>
#include "Player.h"
#include "Board.h"

using namespace std;

// Player::Player(Board& board, const string& name, int capital)
// { // parametric constructor
//     // my_board = & board;
//     my_board = board;
//     player_name = name;
//     player_balance = capital;
//     player_position = nullptr; // by default
// } 

Player::Player(Board& board, const string& name, int capital)
    : my_board(board), player_name(name), player_balance(capital), player_position(nullptr) {/*cout << "\n\tcalled Player constructor\n";*/}

int Player::move(int steps)
{ // move the player and tell if he has passed the starting point
    // cout << "\n\tcalled move for Player\n";
    Node* current_node = player_position; // where the player currently is
    //cout << "Owner: " << current_node->owner << ", Next: " << current_node->next << endl;
    //cout << "\n\tassigned current node successfully\n";
    bool result = false;
    Node* first_node = my_board.get_first_node();
    // Node* first_node = my_board.first; // gives error
    //cout << "\n\tassigned the first node\n";
    for (int i = 0; i < steps; i++)
    {
        //cout << "\t\tin for loop of move and i is: " << i << endl;
        // current_node = current_node->next;
        //cout << "\t\tin for loop of move and i is: " << i << endl;
        if (current_node == nullptr)
        {
            // Handle the case where the player's position is not set
            // Set the player's position to the first node of the board
            current_node = my_board.get_first_node();
        }
        else
        { current_node = current_node->next; }

        // if (current_node != nullptr)
        // { current_node = current_node->next;}

        //cout << "\t\tassigned next node to current node.\n";
        if (first_node == current_node)
        { result = true; /*cout << "\t\tresult is true";*/}
    }

    //cout << "\n\tgot out of for loop\n";

    player_position = current_node;

    if (result == true)
    { /*cout << "\n\tresult of move is 1\n";*/ return 1; }
    else 
    { /*cout << "\n\tresult of move is 0\n";*/ return 0; }
}

Node* Player::where_am_I() const
{ cout << "\n\tcalled where am I\n";return player_position; } // return the position of the player


void Player::pay_to_player(Player& otherPlayer, int money)
{ // take away money from the current player and give to the other one
    cout << "\n\tcalled pay to player\n";
    player_balance -= money;
    otherPlayer.player_balance += money;
}

int Player::get_balance() const
{ /*cout << "\n\tcalled get balance\n";*/ return player_balance; } // returns the total balance of player

void Player::deposit_money(int to_add)
{ /*cout << "\n\tcalled deposit money\n";*/ player_balance += to_add; } // adds the new money to update the player's balance

void Player::buy_slot(int slot_price)
{ // update the owner of bought slot and player balance
    // cout << "\n\tcalled buy slot\n";
    player_position->owner = player_name; // possible due to using reference variable
    player_balance -= slot_price;
}

bool Player::is_bankrupt() const
{ // this function returns false when the balance in negative and vice versa
    cout << "\n\tcalled is bankrupt\n";    
    bool result = false;
    if (player_balance < 0)
    { result = true;}
    else if (player_balance >= 0)
    { result = false; }
    return result;
}


/***
this needs revision
***/
void Player::display() const
{
    //cout << "\n\tcalled display for Player\n";
    cout << player_name << " " << player_balance << endl;
}