#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "Player.h"
#include "Board.h"

using namespace std;

Player::Player(Board & board, const string & name, int capital)
{ // parametric constructor
    my_board = & board;
    player_name = name;
    player_balance = capital;
    player_position = nullptr; // by default
} 

int Player::move(const int steps)
{ // move the player and tell if he has passed the starting point
    Node* current_node = player_position; // where the player currently is
    for (int i = 0; i < steps; i++)
    { current_node = current_node->next; } // move the player 'step' amount of slots
    player_position = current_node; // update the position

    // to check whether the player has passed the starting point
    int result = -1;
    if (current_node == my_board.first)
    { result = 1; }
    else if (current_node != my_board.first)
    { result = 0; }
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

void Player::deposit_money(const int to_add)
{ player_balance += to_add; } // adds the new money to update the player's balance

void Player::buy_slot(const int slot_price)
{ // update the owner of bought slot and player balance
    player_position->owner = player_name; // possible due to using reference variable
    player_balance -= slot_price;
}

bool Player::is_bankrupt() const
{ // this function returns false when the balance in negative and vice versa
    bool result = false;
    if (player_balance < 0)
    { result = true;}
    else if (player_balance >= 0)
    { result = false; }
    return result;
}

