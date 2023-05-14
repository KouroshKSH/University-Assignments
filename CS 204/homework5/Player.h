#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class Player
{
private:
    Board & my_board; // reference variable
    string player_name;
    int player_balance;
    Node * player_position;

public:
// [ ] fix the arguments for each function
    Player(Board & my_board, const string & player_name, int player_balance);
    int move(const int steps);
    Node* where_am_I() const;
    void pay_to_player(Player & otherPlayer, int money);
    int get_balance() const;
    void deposit_money(const int to_add)
    void buy_slot(const int slot_price);
    bool is_bankrupt() const;
    /***/
        void display(); // [ ] left to write
    /***/
};

#endif