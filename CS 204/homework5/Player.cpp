#include <iostream>
#include <string>
#include <iomanip>
#include "Player.h"
#include "Board.h"

using namespace std;

Player::Player(Board& board, const string& name, int capital)
    : my_board(board), player_name(name), player_balance(capital)
    { player_position = board.get_first_node(); } // this line should be inside the constructor

int Player::move(int steps)
{ // move the player and tell if he has passed the starting point
    int result = 0;
    Node* current_node = player_position; // for looping
    Node* first_node = my_board.get_first_node();
    for (int i = 0; i < steps; i++)
    {
        current_node = current_node->next; // move the player
        if (first_node == current_node)
        { result = 1;} // the player has passed the starting point
    }
    player_position = current_node; // update player's position
    return result;
}

Node* Player::where_am_I() const
{ return player_position; } // return the position of the player


void Player::pay_to_player(Player& otherPlayer, int money)
{ // take away money from the current player and give to the other one
    player_balance -= money;
    otherPlayer.player_balance += money;
}

int Player::get_balance() const
{ return player_balance; } // returns the total balance of player

void Player::deposit_money(int to_add)
{ player_balance += to_add; } // adds the new money to update the player's balance

void Player::buy_slot(int slot_price)
{ // update the owner of bought slot and player balance
    player_position->owner = player_name; // possible due to using reference variable
    player_balance -= slot_price;
}

bool Player::is_bankrupt() const
{ // this function returns false when the balance in negative and vice versa
    // if (player_balance < 0)
    // { return true;}
    // else
    // { return false; }
    return (player_balance < 0);
}

void Player::display() const
{ // print the name of the player and its total balance based on position on board
    Node* temp_first_node = my_board.get_first_node();
    Node* current_node = player_position;
    while (temp_first_node != current_node)
    {
        cout << "      "; // 6 spaces long (4 for names + 2 for '->')
        temp_first_node = temp_first_node->next;
    }
    cout << player_name << " " << player_balance << endl;
}