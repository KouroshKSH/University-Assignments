#include <iostream>
#include <string>
#include <iomanip>
#include "Player.h"
#include "Board.h"
// #include "Board.cpp"

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
    : my_board(board), player_name(name), player_balance(capital), player_position(nullptr) {cout << "\n\tcalled Player constructor\n";}

int Player::move(int steps)
{ // move the player and tell if he has passed the starting point
    cout << "\n\tcalled move for Player\n";
    Node* current_node = player_position; // where the player currently is
    cout << "\n\tassigned current node successfully\n";
    bool result = false;
    Node* first_node = my_board.get_first_node();
    cout << "\n\tassigned the first node\n";
    for (int i = 0; i < steps; i++)
    {
        current_node = current_node->next;
        if (first_node == current_node)
        { result = true; }
    }

    player_position = current_node;

    if (result == true)
    { cout << "\n\tresult of move is 1\n"; return 1; }
    else 
    { cout << "\n\tresult of move is 0\n"; return 0; }
    // for (int i = 0; i < steps; i++)
    // { // move the player 'step' amount of slots
    //     cout << "\n\ti is: " << i << "\n";
    //     current_node = current_node->next;
    // } 
    // Move the player
    // for (int i = 0; i < steps; i++)
    // {
    //     cout << "\n\ti is: " << i << "\n";
    //     current_node = current_node->next;
    //     if (current_node == nullptr)
    //     {
    //         // Reached the end of the board, wrap around to the beginning
    //         cout << "\n\treached the beginning of board in move player\n";
    //         current_node = my_board.get_first_node();
    //     }
    // }
    // player_position = current_node; // update the position

    // // Find the number of slots in the board
    // int num_slots = 0;
    // Node* temp = my_board.get_first_node();
    // cout << "\n\tabout to start the while loop for player move\n";
    // while (temp != nullptr && temp != my_board.get_first_node())
    // {
    //     cout << "\t\tstill in while\n";
    //     num_slots++;
    //     temp = temp->next;
    // }
    // num_slots++; // Account for the first node
    // cout << "\n\tfinished while loop in player move\n";

    // cout << "\n\tnumber of slots: " << num_slots << endl;

    // // Calculate the wrapped steps
    // int wrapped_steps = steps % num_slots;
    // cout << "\n\tnumber of wrapped steps: " << wrapped_steps << endl;

    // // Move the player
    // for (int i = 0; i < wrapped_steps; i++)
    // {
    //     cout << "\ti is: " << i << endl;
    //     current_node = current_node->next;
    // }

    // player_position = current_node; // update the position

    // cout << "\n\tupdated the player position successfully\n";

    // // to check whether the player has passed the starting point
    // if (current_node == my_board.get_first_node())
    // { cout << "\n\tfor player move: 1\n";return 1; }
    // else
    // { cout << "\n\tfor player move: 0\n";return 0; }
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
{ cout << "\n\tcalled get balance\n";return player_balance; } // returns the total balance of player

void Player::deposit_money(int to_add)
{ cout << "\n\tcalled deposit money\n";player_balance += to_add; } // adds the new money to update the player's balance

void Player::buy_slot(int slot_price)
{ // update the owner of bought slot and player balance
    cout << "\n\tcalled buy slot\n";
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

void Player::display() const
{
    cout << "\n\tcalled display for Player\n";
    cout << player_name << " " << player_balance << endl;
}