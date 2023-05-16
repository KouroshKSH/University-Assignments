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
    : my_board(board), player_name(name), player_balance(capital){
    player_position = board.get_first_node();        
        /*cout << "\n\tcalled Player constructor\n";*/
}

int Player::move(int steps)
{ // move the player and tell if he has passed the starting point
    // cout << "\n\tcalled move for Player\n";
    Node* current_node = player_position; // where the player currently is
    //cout << "Owner: " << current_node->owner << ", Next: " << current_node->next << endl;
    //cout << "\n\tassigned current node successfully\n";
    int result = 0;
    Node* first_node = my_board.get_first_node();
    // Node* first_node = my_board.first; // gives error
    //cout << "\n\tassigned the first node\n";
    for (int i = 0; i < steps; i++)
    {
        current_node = current_node->next;
        if (first_node == current_node)
        { result = 1;}
    }
    player_position = current_node;
    return result;
}

Node* Player::where_am_I() const
{ /*cout << "\n\tcalled where am I\n";*/return player_position; } // return the position of the player


void Player::pay_to_player(Player& otherPlayer, int money)
{ // take away money from the current player and give to the other one
    // cout << "\n\tcalled pay to player\n";
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
    // cout << "\n\tcalled is bankrupt\n";    
    if (player_balance < 0)
    { return true;}
    else
    { return false; }
}


/***
this needs revision
***/
void Player::display() const
{
    //cout << "\n\tcalled display for Player\n";
    Node* temp_first_node = my_board.get_first_node();
    Node* current_node = player_position;
    int distance = 1;
    while (temp_first_node != current_node)
    {
        distance += 1;
        temp_first_node = temp_first_node->next;
    }
    cout << "\n\t\t=== The distance was: " << distance << endl;
    cout << player_name << " " << player_balance << endl;
}